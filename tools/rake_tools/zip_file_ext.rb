###############################################################################
# ZipFile class extensions
###############################################################################

Zip::ZipFile.class_eval do
  def add_folder(folder, preserveRootFolder = true)
    Dir[folder + '/**/*'].each do |item|
      if preserveRootFolder == true
        name = item
      else
        name = item.sub(File.join(folder, '/'),'')
      end

      if File.directory? item
        mkdir(name)
      else
        add(name, item)
      end
    end
  end

  # zip a folder and all its subitems (including subfolders)
  def ZipFile.zip_folder(folder, zipfile)
      ZipFile.open(zipfile, 'w') do |zipfile|
        zipfile.add_folder(folder, false)
      end
  end
end
