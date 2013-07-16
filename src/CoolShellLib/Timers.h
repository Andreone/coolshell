//  This file is part of CoolShell
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/.

#pragma once

#include <unordered_map>
#include <functional>
#include "boost/noncopyable.hpp"

/**
 * \class timers::win_timer
 * \brief Timers that use a std::function<bool ()> to notify timer events.
 * 
 * A standard Windows timer uses a static function pointer to notify elapsed time.
 * This timer class allows to notify a std::function<bool ()>.
 *
 * \par Usage
 * \code
 *  CFoo
 *  {
 *      timers::win_timer m_timer;
 *
 *      void CFoo::initialyze()
 *      {
 *          // call OnTimer every 10 seconds
 *          m_timer.start(10000, boost::bind<&CFoo::OnTimer, this>());
 *      }
 *
 *      bool CFoo::OnTimer() throw()
 *      {
 *          cout << "10 seconds has elapsed" << endl;
 *
 *          if(I_want_to_be_notified_again())
 *               return true;
 *          else
 *              return false;
 *      }
 *  };
 * \endcode
 */

namespace timers
{
    namespace internals
    {
        template<typename T>
        struct timer_window_trait
        {
            typedef T timer_type;
            typedef UINT_PTR id_type;
            typedef std::unordered_map<id_type, timer_type*> map_type;

            /**
            * \brief	timer callback routine of our Windows timer
            * \param	hWnd: window handle (unused)
            * \param	nMsg: msg ID (unused)
            * \param	nIDEvent: timer ID
            * \param	dwTime: system time (unused)
            */
            static void CALLBACK timer_proc(HWND /*hWnd*/, UINT /*nMsg*/, UINT_PTR nIDEvent, DWORD /*dwTime*/)
            {
                auto i = m_timers.find(nIDEvent);
                ASSERT(i != m_timers.end()); // bad: a timer is still running whilst being not registered
                if(i != m_timers.end())
                {
                    i->second->on_timer();
                }
            }

            static id_type start_timer(timer_type* pTimer, UINT uElapse) throw()
            {
                ASSERT(pTimer);
                ASSERT(uElapse);

                id_type timerId = ::SetTimer(NULL, NULL, uElapse, (TIMERPROC)timer_proc);
                if(timerId)
                {
                    m_timers.insert(map_type::value_type(timerId, pTimer));
                }
                return timerId;
            }

            static void stop_timer(id_type timerId) throw()
            {
                ASSERT(timerId);

                auto i = m_timers.find(timerId);
                if (i != m_timers.end())
                {
                    m_timers.erase(i);
                    ::KillTimer(NULL, timerId);
                }
            }

            static map_type m_timers;
        };

        template<typename timer_type>
        typename timer_window_trait<timer_type>::map_type timer_window_trait<timer_type>::m_timers;


        template<typename TimerTraits> 
        class timer_base
        {
            friend TimerTraits; // allow Traits function to call our on_timer() method.

        protected:
            timer_base() throw() :
                m_timerId(0)
            {
            }

            ~timer_base() throw()
            {
                if(is_running())
                    TimerTraits::stop_timer(m_timerId);
            }

        public:
            bool start(UINT nElapse, std::function<bool ()> callback) throw()
            {
                ASSERT(nElapse > 0);
            
                if(is_running())
                    return false;

                m_callback = callback;
                m_timerId = TimerTraits::start_timer(reinterpret_cast<TimerTraits::timer_type*>(this), nElapse);

                return is_running();
            }

            void stop() throw()
            {
                if(is_running())
                {
                    TimerTraits::stop_timer(m_timerId);
                    reset();
                }
            }

            bool is_running() const throw()
            { 
                return (m_timerId != 0);
            }

            UINT id() const throw()
            {
                return m_timerId;
            }

        private:
            void reset() throw()
            {
                m_timerId = 0;
            }

            void on_timer() throw()
            {
                if(!m_callback())
                    stop();
            }
        
            // non copyable
            timer_base(const timer_base&);
            void operator&(const timer_base&);

        private:
            typename TimerTraits::id_type m_timerId;	/**< Windows API timer identifier */
            std::function<bool ()> m_callback;          /**< the function to call back */
        };
    } // namespace internals

    struct win_timer : public internals::timer_base<internals::timer_window_trait<win_timer>>,
                       private boost::noncopyable                   
    { };
} // namespace timers