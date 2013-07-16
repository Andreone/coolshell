module Versioning
  # ('1.2.3.4', '5') => '1.2.3.5'
  def self.set_build_number(version, build)
    version.gsub(/((\d+\.){3})\d+/, '\1' + build)
  end

  ###############################################################################
  # Reads the version property from an AssemblyInfo.cs file
  def self.read_dotnet_version(assemblyinfo_filename)
      text = File.read(assemblyinfo_filename)
      version = $1 if text =~ /AssemblyVersion\s*\("(\d+\.\d+\.\d+.\d+)/
  end

  ###############################################################################
  
  # VS_VERSION_INFO VERSIONINFO
  #  FILEVERSION 1,2,3,4
  #  PRODUCTVERSION 1,2,3,4
  #  FILEFLAGSMASK 0x3fL
  # #ifdef _DEBUG
  #  FILEFLAGS 0x1L
  # #else
  #  FILEFLAGS 0x0L
  # #endif
  #  FILEOS 0x40004L
  #  FILETYPE 0x0L
  #  FILESUBTYPE 0x0L
  # BEGIN
  #     BLOCK "StringFileInfo"
  #     BEGIN
  #         BLOCK "040004b0"
  #         BEGIN
  #             VALUE "FileDescription", "CoolShell application"
  #             VALUE "FileVersion", "1.2.3.4"
  #             VALUE "InternalName", "CoolShell.exe"
  #             VALUE "OriginalFilename", "CoolShell.exe"
  #             VALUE "ProductName", "CoolShell"
  #             VALUE "ProductVersion", "1.2.3.4"
  #         END
  #     END
  # END
  
  # Update C++ version info into resource file with a build number.
  def self.update_cpp_build_number(resource_file_name, build_number)
    puts "updating #{resource_file_name} with #{build_number}"
    text = File.read(resource_file_name)
    # update StringFileInfo block
    text.gsub!(/(\"(File|Product)Version\",\s+\"(\d+(,|\.)\s*){3})(\d+)\"/, '\1' + build_number + '"')
    # update VERSIONINFO block
    text.gsub!(/((FILE|PRODUCT)VERSION\s+(\d+,){3})(\d+)/, '\1' + build_number)
    File.open(resource_file_name, 'w') { |f| f.write(text) }
  end

  # Reads the version from a c++ resource file (the FILEVERSION property is used by default)
  def self.read_cpp_version(resource_file_name, property_name='FILEVERSION')
    text = File.read(resource_file_name)
    version = "#{$1}.#{$2}.#{$3}.#{$4}" if text =~ /^\s+#{property_name}\s+(\d+),(\d+),(\d+),(\d+)/
  end

  # Updates all versions found in a c++ resource file with a given version
  def self.write_cpp_version(resource_file_name, version)
    text = File.read(resource_file_name)
    # update StringFileInfo block
    text.gsub!(/(\"(File|Product)Version\",\s+\")(\d+(\.\d+){3})\"/, '\1' + version + '"')
    # update VERSIONINFO block
    text.gsub!(/((FILE|PRODUCT)VERSION\s+)(\d+,){3}(\d+)/, '\1' + version.gsub(/\./, ','))
    File.open(resource_file_name, 'w') { |f| f.write(text) }
  end
end
