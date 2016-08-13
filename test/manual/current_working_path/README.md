'Current PWD' Manual Test
-------------------------

### Overview

This manual test must be done to verify that the **path of the file** is used as 
**current working directory**, instead of the path of the application.

### Steps

1. Open the *.DAT testfile using this command:

        /%APP_PATH%/nastranfind ./model-win/test.dat
        /%APP_PATH%/nastranfind ./model-unix/test.dat

    ...where `%APP_PATH%` is the full path to the application, for instance `/usr/nastranfinder/`.


2. Verify that the INCLUDE file `included.dat` appears in the GUI, **without** any error message.

3. That's all!

