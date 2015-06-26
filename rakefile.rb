require 'zip'
require_relative './tools/rake_tools/versioning.rb'
require_relative './tools/rake_tools/msbuild.rb'

$project_name = 'CoolShell'
$solution_file = 'CoolShell.sln'
$artifacts_dir = 'artifacts'
$staging_area_dir = 'staging_area'

$build_configuration = ENV['config'] || 'Release'
$build_platform = ENV['platform'] || 'Win32'
$build_toolset = ENV['toolset'] || 'v120'
$project_resource_file = File.join('src', $project_name, $project_name + '.rc')

def GetBuildOutputDir()
  File.join('bin', $build_configuration + '_' + $build_platform)
end

build_number = ENV['BUILD_NUMBER'] || '1'
$version = "1.0.0" + ".#{build_number}"
  
#puts "Configuration: #{$build_configuration}"
#puts "Platform: #{$build_platform}"
#puts "Toolset: #{$build_toolset}"
#puts "Version: #{$version}"
#puts

################################################################################

task :default do
  puts `rake -T`
end

desc 'Update version, build and create artifacts'
task :all => [:versioning, :build, :artifacts]

desc 'Update x64 version, build and create artifacts'
task :all64 => [:set64, :all]

task :set64 do
  $build_platform = 'x64'
end

################################################################################

def make_msbuild_command_line(msbuild_targets)
  properties = {
    Msbuild::Properties::Configuration => $build_configuration,
    Msbuild::Properties::Platform => $build_platform,
    Msbuild::Properties::Toolset => $build_toolset
    }

  msbuild_script = File.expand_path($solution_file).gsub('/', '\\')

  cmdline = Msbuild::env_command_line($build_toolset, $build_platform)
  cmdline += " & "
  cmdline += Msbuild::command_line(msbuild_script, msbuild_targets, properties)

  unless !$number_of_processors
    cmdline += " /m"
    cmdline += ":#{$number_of_processors}" if $number_of_processors.to_i > 0
  end
  
  cmdline
end

# desc 'Run a build of the whole solution'
task :build do
  cmdline = make_msbuild_command_line(['Build'])
  sh cmdline
end

# desc 'Clean the whole solution'
task  :clean do
  cmdline = make_msbuild_command_line(['Clean'])
  sh cmdline
end

desc 'Rebuild the whole solution'
task :rebuild => [:clean, :build]

desc 'Update version into c++ resource file'
task :versioning do
  Versioning.write_cpp_version($project_resource_file, $version)
end

desc 'Create artifacts from built files'
task :artifacts do
  zipfile_name = $project_name + '-' + $version + '-' + $build_platform + '.zip'
  puts "generating #{zipfile_name}..."
  
  FileUtils.rm(zipfile_name) if File.exist?(zipfile_name)
    
  Zip::File.open(zipfile_name, Zip::File::CREATE) do |zipfile|
    zipfile.add('CoolShell.exe', GetBuildOutputDir() + '/CoolShell.exe')
    zipfile.add('license.txt', 'license.txt')
    zipfile.add('credits.txt', 'credits.txt')
  end
end

################################################################################

desc 'Generate Doxygen documentation'
task :doc do
  `doxygen doxyfile`
end

desc 'Run a CLOC analysis'
task :cloc do
  analysed_folders = 'src'
  xml_result = 'cloc.xml'
  html_report = 'cloc_report.html'
  sh "tools\\cloc\\cloc --out=#{xml_result} --xml --quiet --skip-uniqueness --force-lang=C++,h #{analysed_folders}"
  `xml tr tools\\cloc\\pretty_cloc.xsl #{xml_result} > #{html_report}`
  FileUtils.rm(xml_result) if File.exist?(xml_result)
end

desc 'Run a CppCheck analysis'
task :cppcheck do
  analysed_folders = 'src'
  xml_result = 'cppcheck_results.xml'
  html_report = 'cppcheck_report.html'
  sh "tools\\cppcheck\\cppcheck -j#{ENV['NUMBER_OF_PROCESSORS']} --enable=all --xml -D_MFC_VER #{analysed_folders} 2> #{xml_result}"
  `xml tr tools\\cppcheck\\pretty_html_report.xslt #{xml_result} > #{html_report}`
  FileUtils.rm(xml_result) if File.exist?(xml_result)
end

################################################################################

def clean_directory(root_dir)
  FileList.new(File.join(root_dir, '**/bin'),
               File.join(root_dir, '**/obj'),
               File.join(root_dir, '**/artifacts'),
               File.join(root_dir, '**/doxydoc'),
               File.join(root_dir, '**/ipch'),
               File.join(root_dir, '**/logs'),
               File.join(root_dir, '**/*.bak'),
               File.join(root_dir, '**/*.dmp'),
               File.join(root_dir, '**/*.ilk'),
               File.join(root_dir, '**/*.orig'),
               File.join(root_dir, '**/*.obj'),
               File.join(root_dir, '**/*.pch'),
               File.join(root_dir, '**/*.sdf'),
               File.join(root_dir, '**/*.suo'),
               File.join(root_dir, '**/_ReSharper.*')
               ).each do |item|
    if File.file?(item)
      File.delete(item)
      puts "deleted file #{item}"
    elsif File.directory?(item)
      FileUtils.remove_dir(item)
      puts "deleted directory #{item}"
    end
  end
end

desc 'Delete temp items (bin, obj, ...) from the repo'
task :clean_repo do
  clean_directory(File.join(Rake.application.original_dir, 'src'))
end
