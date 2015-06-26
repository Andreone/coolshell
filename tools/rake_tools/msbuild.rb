module Msbuild
  MSBUILD_EXE = "MSBuild.exe"

  class Properties
    Configuration = 'Configuration'
    Platform = 'Platform'
    Toolset = 'PlatformToolset'
    OutputDir = 'OutDir'
  end
  
  def Msbuild.command_line(file, targets, properties)
    cmdline = Msbuild::MSBUILD_EXE + " \"#{file}\""
    targets.each { |target| cmdline += " /t:#{target}" } unless targets.nil?
    properties.each { |name, value| cmdline += " /p:#{name}=#{value}" } unless properties.nil?
    cmdline
  end

  def Msbuild.env_command_line(toolset, platform)
    # this is the correct command line, but we can't use it because VS2010 has not been correctly installed on the TeamCity build agents
    if toolset == 'v120'
        cmdline = "\"%VS#{toolset.gsub('v', '')}COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat\" amd64_x86"
    else
        cmdline = "\"%VS#{toolset.gsub('v', '')}COMNTOOLS%\\vsvars32.bat\""
    end
  end
end
