#check for platforms (either Darwing for mac or Linux)
platform=$(shell uname)

ifeq ($(platform),Linux)
	# FLAGS FOR LINUX, LIKE ON CDF.

	# The most recent VXL library is available on CDF in the following path:
	VXLDIR = /h/u1/mangas/public/vxl

	# Include directories for vxl
	CFLAGS = -I$(VXLDIR)/ -I$(VXLDIR)/vxl-1.17.0 -I$(VXLDIR)/vxl-1.17.0/core -I$(VXLDIR)/vxl-1.17.0/vcl -I$(VXLDIR)/bin/vcl -I$(VXLDIR)/bin/core	-I$(VXLDIR)/vxl-1.17.0/core -Wno-deprecated 

	#fltk library paths and libs
	FLTKLDFLAGS = -L/local/lib -L/usr/lib/x86_64-linux-gnu/ -Wl,-rpath,/local/lib -lfltk_gl -lGLU -lGL -lfltk -L/usr/X11R6/lib -lpthread -lm -lXext -lX11 -lsupc++

	#vxl linking components
	VXLLDFLAGS = -L$(VXLDIR)/bin/lib -lvil -lvcl -lvnl_algo -lvil_algo -lvnl_io -lvnl -lv3p_netlib -lvul -lpng -ltiff -lopenjpeg2 -lgeotiff
else
	# FLAGS FOR MACOS
	
	# The standard include directory for vxl is the following
	VXLDIR = /usr/local/include/vxl

	# Include directories for vxl
	CFLAGS = -I$(VXLDIR)/ -I$(VXLDIR)/vcl -I$(VXLDIR)/core -I$(VXLDIR)/core/vil -Wno-deprecated -DJPEG_LIB_VERSION=80

	#fltk library paths and libs	
	FLTKLDFLAGS = -L/usr/local/lib/ -Wl,-rpath,/local/lib -lfltk_gl -lfltk -framework OpenGL -framework GLUT -framework Cocoa -framework AGL
	
	#vxl linking
	VXLLDFLAGS = -L$(VXLDIR)/core -L$(VXLDIR)/vcl -L/opt/X11/lib/ -lvil -lvnl_algo -lvnl_io -lvnl -lv3p_netlib -lvcl -lvul -ltiff -lz -lopenjpeg2 -lgeotiff -ljpeg -lpng
endif

LDFLAGS = -lstdc++ $(FLTKLDFLAGS) $(VXLLDFLAGS)

CC = g++

FLUID = fluid

%.o: %.cxx
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<


%.cxx: %.fl
	$(FLUID) -c $<

BASIC_OBJ   = gl/glutils.o gl/Texture.o main.o file/load_image.o

IMDRAW_OBJ  = imdraw/imdraw_utils.o imdraw/imdraw_init.o imdraw/imdraw_draw.o imdraw/imdraw_handle.o imdraw/read_drawing.o imdraw/imdraw_object.o

UI_OBJ      = VisCompUI.o

MISSING_VXL_OBJ = vil_trace_8con_boundary.o

UI_CPP      = VisCompUI.cxx VisCompUI.h

MATTING_OBJ = matting/matting.o matting/matting_algorithm.o

INPAINTING_OBJ = inpainting/inpainting.o inpainting/inpainting_algorithm.o inpainting/inpainting_debug.o inpainting/psi.o  inpainting/inpainting_eval.o inpainting/patch_db.o

BLENDING_OBJ = 

STUDENT_OBJ = pyramid/pyramid.o pyramid/blend.o

MORPHING_OBJ = morphing/morphing.o morphing/morph_algorithm.o morphing/morphing_ui.o morphing/linepairs.o



imdraw-code/viscomp:	$(UI_CPP) $(BASIC_OBJ) $(UI_OBJ) $(MISSING_VXL_OBJ) $(MATTING_OBJ) $(IMDRAW_OBJ) $(INPAINTING_OBJ) $(MORPHING_OBJ) $(BLENDING_OBJ) $(STUDENT_OBJ)

	$(CC) -o ../bin/viscomp $(BASIC_OBJ) $(UI_OBJ) $(MISSING_VXL_OBJ) $(MATTING_OBJ) $(IMDRAW_OBJ) $(INPAINTING_OBJ) $(MORPHING_OBJ) $(BLENDING_OBJ) $(STUDENT_OBJ) $(LDFLAGS)



clean:		

	rm -rf $(BASIC_OBJ) $(UI_OBJ) $(STUDENT_OBJ) $(UI_CPP) $(MORPHING_OBJ) $(IMDRAW_OBJ) 


