# Subversion client
# It's base on subversion commmand line tools and assume they are in the system PATH.
class Subversion
  attr_accessor :user, :pwd
  
  def initialize(user=nil, pwd=nil)
    @user = user
    @pwd = pwd
  end

  def checkout(url, dir)
    command = "svn co #{url} #{dir}" + get_auth_params()
    `#{command}`
  end

  def commit(dir, message)
    `svn commit #{dir} -m #{message}`
  end

  def get_log_msg(svn_rev)
    info = `svn log -r #{svn_rev}`
    log_entry = nil
    if info =~ /r(\d+)\s+\|\s+(.*?)\s+\|\s+(.*?)\s+\|.*?line[s]?(.*)/m
      rev = $1
      by = $2
      date = $3
      message = $4.gsub(/-*/, '') # remove dash line
      log_entry = SubversionLogEntry.new(rev, by, date, message.strip)
    end
  end
  
  def get_local_revision()
    info = `svnversion`
    rev = $1 if info =~ /(?:\d+\:)*(\d+)/
  end
  
  def get_auth_params()
    command = ''
    command += " --username=#{@user}" unless @user.nil?
    command += " --password=#{@pwd}" unless @pwd.nil?
  end
end

# Describe information of a change-log entry
class SubversionLogEntry
  attr_accessor :revision, :by, :date, :message

  def initialize(revision, by, date, message)
    @revision = revision
    @by = by
    @date = date
    @message = message
  end
  
  def to_s
    s = '<SubversionLogEntry'
    s += " {rev:#{@revision}}" unless @revision.nil?
    s += " {by:#{@by}}" unless @by.nil?
    s += " {date:#{@date}}" unless @date.nil?
    s += " {message:#{@message}}" unless @message.nil?
    s += '>'
  end
end