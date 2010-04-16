###############################################
# SavageWheels Makefile
#
# $Date: 
# $Author: p.petrov
#
# $Log:
#
#
###############################################


CC = g++
OBJ_DIR = obj
TARGET = SavageWheels


##################################
# Variables
##################################

CFLAGS = -pipe -Wall -g3 -c $(shell sdl-config --cflags --libs)
LFLAGS = $(shell sdl-config --libs)

LIB_DIRS = -L"/usr/lib"
LIBS = "/usr/lib/libSDL.a" "/usr/lib/libSDLmain.a" "/usr/lib/libfmod-3.75.so"
INC_DIRS = -I"/usr/include/SDL" -I"fmodapi375linux/api/inc"
PREPROC = -DLINUX_BUILD -DDEBUG -D__DEBUG -D_DEBUG


SRC_OBJS = \
	$(OBJ_DIR)/CAnimation.o \
	$(OBJ_DIR)/CDeadToy.o \
	$(OBJ_DIR)/Cgame.o \
	$(OBJ_DIR)/CKdf.o \
	$(OBJ_DIR)/CMainMenu.o \
	$(OBJ_DIR)/Csdl.o \
	$(OBJ_DIR)/CSounds.o \
	$(OBJ_DIR)/Cswv_module.o \
	$(OBJ_DIR)/CTimer.o \
	$(OBJ_DIR)/Cvehicle.o \
	$(OBJ_DIR)/Main.o \
	$(OBJ_DIR)/Utils.o


define compile_src
@echo Compiling $< ...
$(CC) $(CFLAGS) $(PREPROC) $(INC_DIRS) -o $@ $<
endef

##################################
# Build rules
##################################

$(TARGET): $(SRC_OBJS)
	$(CC) $(LFLAGS) $(LIB_DIRS) $(LIBS) -o $@ $^

all: $(TARGET)

clean:
	-rm -f $(SRC_OBJS) $(TARGET)


##################################
# Dependencies
##################################

$(OBJ_DIR)/CAnimation.o: CAnimation.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/CDeadToy.o: CDeadToy.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/Cgame.o: Cgame.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/CKdf.o: CKdf.cpp \
	CKdf.h
	$(compile_src)

$(OBJ_DIR)/CMainMenu.o: CMainMenu.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/Csdl.o: Csdl.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/CSounds.o: CSounds.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/Cswv_module.o: Cswv_module.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/CTimer.o: CTimer.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/Cvehicle.o: Cvehicle.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/Main.o: Main.cpp \
	main.h
	$(compile_src)

$(OBJ_DIR)/Utils.o: Utils.cpp \
	main.h
	$(compile_src)

