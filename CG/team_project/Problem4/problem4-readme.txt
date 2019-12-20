---------------
GENERAL REMARKS
---------------

A. SOLUTION EXECUTABLE

  You are being supplied with a full implementation of the assignment:

    bin/viscomp_full	      linux executable compiled on CDF


  The executable operates in three modes:

  1. Non-interactive (ie. batch) mode:

     You supply all images as command-line arguments. The following is
     a run will all morphing options specified

         viscomp_full -no_gui -morphing 
                  -msource0 ../test_images/couple0.jpg 
                  -msource1 ../test_images/couple1.jpg 
                  -mlines ../test_images/lines_couple_better.txt  
                  -mbase ../test_images/result 
                  -mwarp -mnum 1  -mt 0.3 -ma  0.4 -mb 1.6  -mp 0.6
		  [other command line arguments]

     This computes a single morph between the two source images using
     a set of corresponding lines specified in a separate file

     To see the various command-line options, type 
	 viscomp_full -help

  2. Fully interactive mode:

     You just type 'viscomp_full' and a window opens up
     with two panels for displaying images. You can now use the 
     menus or keyboard shortcuts to load images and run the code.

     Some tips:
       * Select the "Morph" mode from the drop-down box in the main 
         window to see any images/lines loaded via the command line
       * Select Morphing->Display Control from the menubar to open up
         a panel that controls all aspects of the morphing implementation
       * For precise line placement it is best to work on zoomed-in views
         of the images

  3. Combination:

     If you don't supply the '-no_gui' option on the command line but
     supply command-line options for loading some (or all) of the 
     images. In this case, the images will be loaded, the morphing
     algorithm will be run if all the required images have been 
     supplied and, finally, the UI will be displayed.

  4. To quickly get a feel for the morphing procedure & the implementation:

     - Type the command line shown in 1. above, without the -no_gui option
     - Select "Morph" from the drop-down menu on the left side of the
       window
     - Type <Ctrl>-d to display the Morphing Control Window
     - Select Warped I0 or Warped I1 to see the morphing result
     - Edit some of the lines and re-run the algorithm
     - To save the results toggle the "Morph->Disk" and/or "Warp->Disk"
       buttons on the morphing control window
     - Note: if you set the -mnum parameter to a value > 1 or choose a number
       greater than 1 in the "number of images" box on the control window,
       the setting of the t parameter is ignored. Also note that the 
       "number of images" text box on the control panel is not updated with
       the value of the "-mnum" parameter passed via the command line, 
       so it might show "1" even if the mnum parameter was set to a 
       value > 1. Finally, to update the number of images via the 
       control window text box, you must press ENTER after typing in the
       desired number of images.



---------------------
STRUCTURE OF THE CODE
---------------------

1. NOTES

  * All source and header files are in the src directory
  * You should not modify ANY files in src/ except for the 
    file src/morphing/morphing_algorithm.cxx
  * All your code should go in the src/morphing directory.
    You are free to distribute your code in as many 
    additional files as you want, as long as they remain in that
    directory. We will be running scripts to compile and run your
    code automatically, and code outside those directories will be
    ignored.


2. FILES IN THE DIRECTORY src/ that were changed/added since Assignment 2

  bin/viscomp      Executable of the "starter" implementation. This is
		   created by compiling the code you already have.
                   You will need to extend it so that it matches the exact
		   behavior of the full implementation. Run this after
		   becoming familiar with the full implementation.

  Makefile	   Makefile that buids the starter executable by 
		   running 'make' in the src/ directory

  main.cxx:	   Top-level function. The only addition is code in
		   function process_args() for processing the command-line
                   arguments for the morphing algorithm.

  fltk_includes.h  Include files used by the FULL implementation. Ordinarily,
  vxl_includes.h   you should not need to include any more VXL or FLTK 
                   include files to get your full implementation to compile
		   correctly. A few more entries are added to vxl_includes.h
                   for Assignment 3.

5. FILES IN THE DIRECTORY src/morphing

  morphing.h               You must read this file carefully. It contains
		           a list of all top-level methods related to the 
                           morphing algorithm, along with all the variables
                           of the morphing class. The two methods you must
                           implement are indicated here, along with the
                           top-level methods that call them.

  linepairs.h              You must read this file very carefully, as it describes
                           the data structure used for representing pairs
                           of corresponding lines between two images, and ways
                           to access/represent pairs of corresponding lines.

  morphing_algorithm.cxx   This is where your code goes. The file also contains
                           the top-level routines of the morphing implementation,
                           along with the specs of the methods you must
                           implement.

  morphing.cxx             This file contains lots of little nitty-gritty
			   routines for accessing the elements of the
			   morphing class. You may want to read it.

  morphing_ui.cxx          This file provides the interface between the UI
                           and the morphing application. It supports display
                           of images/lines in the two panels and interactive
                           editing of lines. Read this file only if interested.

  linepairs.cxx            This file implements the linepairs class and its
                           methods. You may want to read this file.


6. CHANGES IN THE DIRECTORY src/imdraw

  imdraw.h         The class description now contains a pointer to the morphing
                   data structure. This enables direct two-way communication
                   between the UI and the morphing implementation.

  imdraw_objects.h   Extended the vector objects so allow for control of line
                           thickness.

  imdraw_handle.cpp  The event handler in this file was extended to provide the
                                line editing functionalities used in Assignment 3. You 
                               do not need to read or understand this file.