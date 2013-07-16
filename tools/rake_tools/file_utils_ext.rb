###############################################################################
# FileUtils class extensions
###############################################################################

FileUtils.class_eval do
  # if path exists, removes it and recreates it
  # if path doesn't exist and :create options is true, creates it
  def FileUtils.empty_dir(path, options = {})
    o = { :create  => false }.merge options
    
    if File.directory? path
      FileUtils.remove_dir(path)
      FileUtils.mkdir_p(path)
    elsif o[:create]
      FileUtils.mkdir_p(path)
    end
  end
  
  # Copy src directory and all its content recursively to dest, accepting
  # a predicate that will be evaluated against each item to copy.
  # dest is created if it doesn't exist
  # sample: FileUtils.cp_dir_predicate(src, dest, lambda{|i| i != 'file_name_to_not_copy'})
  def FileUtils.cp_dir_predicate(src, dest, predicate)
    current_dest_dir = dest
    
    FileUtils.mkdir(dest) if !File.directory?(dest)

    Dir.foreach(src) do |item|
      next if item == '..' || item == '.' || !predicate.call(item)
      
      s = File.join(src, item)
      d = File.join(dest, item)

      if File.directory?(s)
        FileUtils.mkdir(d) if !File.directory?(d)
        FileUtils.cp_dir_predicate(s, d, predicate)
      else
        FileUtils.cp(s, d)
      end
    end
  end
end
