require 'formula'

class Cvtracking < Formula
  url 'https://github.com/walchko/cvTracking.git', :branch => "Tracker"
  version "0.1.0"
  homepage 'http://github.com/walchko/cvTracking'
  
  depends_on 'cmake' => :build
  depends_on 'opencv' => :build
  #depends_on 'doxygen' => :optional

  def install
	 system "mkdir build"
	 Dir.chdir 'build' do
	 	system "cmake .. -DCMAKE_INSTALL_PREFIX=#{prefix}"
    	system "make install"
    end
  end
end
