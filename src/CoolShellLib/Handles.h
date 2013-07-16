#pragma once

#include <Windows.h>
#include <stdio.h>

// RAII Windows handles
namespace Handles
{
    namespace Internals
    {
        template <typename THandle, void(*CloseFunc)(THandle&)>
        class ScopedHandle
        {
        public:
            typedef THandle handle_type;

            ScopedHandle() throw():
                m_value(nullptr)
            {
            }

            explicit ScopedHandle(handle_type value) throw():
                m_value(value)
            {
            }

            explicit ScopedHandle(ScopedHandle&& other) throw() :
                m_value(other.release())
            {
            }

            ~ScopedHandle() throw()
            {
                close();
            }

            ScopedHandle& operator=(ScopedHandle&& other) throw()
            {
                reset(other.release());
                return *this;
            }

            operator bool () const throw()
            {
                return is_valid();
            }

            void reset(handle_type value = null_handle) throw()
            {
                if(m_value != value)
                {
                    close();
                    m_value = value;
                }
            }

            handle_type release() const throw()
            {
                handle_type current = get();
                m_value = nullptr;
                return current;
            }

            handle_type get() const throw()
            {
                return m_value;
            }

            handle_type& get_ref() throw()
            {
                return m_value;
            }

            handle_type* get_ptr() throw()
            {
                return &m_value;
            }

            operator handle_type() const throw()
            {
                return m_value;
            }

        private:
            bool is_valid() const throw()
            {
                return m_value != nullptr;
            }

            void close() throw()
            {
                CloseFunc(m_value);
            }

            handle_type m_value;
        };

        inline void handle_close(HANDLE& h) { ::CloseHandle(h); }
        inline void hlocal_close(HANDLE& h) { ::LocalFree(h); }
        inline void hglobal_close(HANDLE& h) { ::GlobalFree(h); }
        inline void hmodule_close(HMODULE& h) { ::FreeLibrary(h); }
        inline void hdc_close(HDC& h) { ::DeleteDC(h); }
        inline void hcursor_close(HCURSOR& h) { ::DestroyCursor(h); }
        inline void hmenu_close(HMENU& h) { ::DestroyMenu(h); }
        inline void hgdiobj_close(HGDIOBJ& h) { ::DeleteObject(h); }
        inline void hkey_close(HKEY& h) { ::RegCloseKey(h); }
        inline void hicon_close(HICON& h) { ::DestroyIcon(h); }
        inline void file_close(FILE*& p) { ::fclose(p); }
    } // namespace Internals

    typedef Internals::ScopedHandle<HANDLE, Internals::handle_close> handle;
    typedef Internals::ScopedHandle<HLOCAL, Internals::hlocal_close> hlocal;
    typedef Internals::ScopedHandle<HGLOBAL, Internals::hglobal_close> hglobal;
    typedef Internals::ScopedHandle<HMODULE , Internals::hmodule_close> hmodule;
    typedef Internals::ScopedHandle<HDC, Internals::hdc_close> hdc;
    typedef Internals::ScopedHandle<HCURSOR, Internals::hcursor_close> hcursor;
    typedef Internals::ScopedHandle<HMENU, Internals::hmenu_close> hmenu;
    typedef Internals::ScopedHandle<HGDIOBJ, Internals::hgdiobj_close> hgdiobj;
    typedef Internals::ScopedHandle<HKEY, Internals::hkey_close> hkey;
    typedef Internals::ScopedHandle<HICON, Internals::hicon_close> hicon;
    typedef Internals::ScopedHandle<FILE*, Internals::file_close> file;
} // namespace Handles