require 'albacore'
require_relative './tools/rake_tools/versioning.rb'

$project_name = 'CoolShell'
$solution_file = 'CoolShell.sln'
$artifacts_dir = 'artifacts'
$staging_area_dir = 'staging_area'

$configuration = ENV['config'] || 'Release'
$platform = ENV['platform'] || 'Win32'
$toolset = ENV['toolset'] || 'v110'

$project_resource_file = File.join('src', $project_name, $project_name + '.rc')
def GetBuildOutputDir()
  File.join('bin', $configuration + '_' + $platform)
end

build_number = ENV['BUILD_NUMBER'] || '1'
$version = "1.0.0" + ".#{build_number}"
  
#puts "Configuration: #{$configuration}"
#puts "Platform: #{$platform}"
#puts "Toolset: #{$toolset}"
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
  $platform = 'x64'
end

# Albacore configuration task run before any other task: configure default task properties
Albacore.configure do |config|
  # setting log level to :verbose causes the full command line call for each task to be logged
  # config.log_level = :verbose
  # override default msbuild verbosity ('normal') so the output of post-build steps / unit tests is forwarded to stdout
  # config.msbuild.verbosity = 'detailed'
  config.msbuild.solution = $solution_file
end

################################################################################

desc 'Run a build of the whole solution'
msbuild :build do |t|
  t.properties :configuration => $configuration, :Platform => $platform, :PlatformToolset => $toolset
  t.targets :Build

  puts "Building #{$solution_file}..."
end

desc 'Clean the whole solution'
msbuild :clean do |t|
  t.properties :configuration => $configuration, :Platform => $platform, :PlatformToolset => $toolset
  t.targets :Clean
end

desc 'Rebuild the whole solution'
task :rebuild => [:clean, :build]

desc 'Update version into c++ resource file'
task :versioning do
  Versioning.write_cpp_version($project_resource_file, $version)
end

################################################################################

desc 'Create artifacts from built files'
task :artifacts => [:artifacts_prepare, :artifacts_generate, :artifacts_cleanup]

task :artifacts_prepare do
  FileUtils.mkdir_p($artifacts_dir) unless File.directory? $artifacts_dir
  FileUtils.remove_dir($staging_area_dir) if File.directory? $staging_area_dir
  FileUtils.mkdir_p($staging_area_dir)

  FileList[ GetBuildOutputDir() + '/CoolShell.exe',
            'license.txt',
            'credits.txt'].exclude(/.*Tests.*/).each do |file|
    FileUtils.cp file, $staging_area_dir
    puts
    puts file
  end
end

zip :artifacts_generate do |zip|
  zip.directories_to_zip $staging_area_dir
  zip.output_file = $project_name + '-' + $version + '-' + $platform + '.zip'
  zip.output_path = $artifacts_dir
  puts "generating #{zip.output_file}..."
end

task :artifacts_cleanup do
  FileUtils.remove_dir($staging_area_dir) if File.directory? $staging_area_dir
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
  sh "cloc --out=#{xml_result} --xml --quiet --skip-uniqueness --force-lang=C++,h #{analysed_folders}"
  `xml tr tools\\cloc\\pretty_cloc.xsl #{xml_result} > #{html_report}`
  FileUtils.rm(xml_result) if File.exist?(xml_result)
end

desc 'Run a CppCheck analysis'
task :cppcheck do
  analysed_folders = 'src'
  xml_result = 'cppcheck_results.xml'
  html_report = 'cppcheck_report.html'
  sh "cppcheck -j#{ENV['NUMBER_OF_PROCESSORS']} --enable=all --xml -D_MFC_VER #{analysed_folders} 2> #{xml_result}"
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
