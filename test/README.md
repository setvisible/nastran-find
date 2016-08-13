# Tests


 - `/auto`    
        Contains the automatic unit tests (requires the Qt framework, i.e. QtTest)

 - `/manual`     
        Contains the manual tests
 
 - `/shared`     
        Contains the dummy test files. Note that they are  copied by the Qt Test Engine to:

        %build_path%/share/


where the target folder name is *share* (not *share**d***), in order to make the difference between the source directory and the destination directory.

