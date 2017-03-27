
# Copyright (c) 2017 The Gupta Empire - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
#
# Written by Aryan Gupta <me@theguptaempire.net>
# 
# ==============================================================================
# @author 			Aryan Gupta
# @project 			Archespace
# @file 			Makefile
# @date				2017-03-06 (YYYY-MM-DD)
# @version			1.0.0
# @brief 			This is the Makefile for The Tic Tac Toe Project
# ==============================================================================
.DEFAULT_GOAL := all
# ==========================  CONST MACROS  ====================================
CC = "C:\Compiler\MinGW-w64\mingw32\bin\g++.exe"
7Z = "C:\Program Files (Portable)\7-Zip\7z.exe"
RES = "C:\Compiler\MinGW-w64\mingw32\bin\windres.exe"
DOXY = "C:\Compiler\doxygen\bin\doxygen.exe"
OBJDIR = .\obj
BINDIR = .\bin
DATDIR = .\dat

DEBUG = -g -DDEBUG=true

# ============================  SDL LIBS  ======================================
GRAPHICS = -w -Wl,-subsystem,windows
# Standard SDL libs
L_SDLC = -IC:\Compiler\SDL\include\SDL2 
L_SDLL = -LC:\Compiler\SDL\lib -lmingw32 -lSDL2main -lSDL2 \
							   -lSDL2_mixer  -lSDL2_ttf  -lSDL2_image
L_SDLS = -lmingw32 -lSDL2main -lSDL2  -mwindows  -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

# ==============================  MACROS  ======================================
CFLAGS = $(DEBUG) -Wall -std=c++17 -c
LFLAGS = $(DEBUG) -Wall
OBJ = $(OBJDIR)\main.o $(OBJDIR)\res.o $(OBJDIR)\Ship.o $(OBJDIR)\Window.o \
		$(OBJDIR)\Bullet.o $(OBJDIR)\Powerup.o $(OBJDIR)\Resources.o
#		$(OBJDIR)\cppMemDbg.o

# ============================ RECEPIES ========================================

$(OBJDIR)\main.o: .\main.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) $(L_SDLC)

$(OBJDIR)\Ship.o: .\Ship.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) $(L_SDLC)

$(OBJDIR)\Window.o: .\Window.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) $(L_SDLC)

$(OBJDIR)\Bullet.o: .\Bullet.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) $(L_SDLC)

$(OBJDIR)\Powerup.o: .\Powerup.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) $(L_SDLC)

$(OBJDIR)\Resources.o: .\Resources.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) $(L_SDLC)
	
#$(OBJDIR)\cppMemDbg.o:.\debug\cppMemDbg.cpp
#	$(CC) .\$^ -o .\$@ $(CFLAGS) 
	
$(OBJDIR)\%.o: .\%.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) 

$(OBJDIR)\res.o: .\res.rc .\info.h
	$(RES) .\res.rc  .\$@
	
# Link	
$(BINDIR)\main.exe: $(OBJ)
	$(CC) .\$^ -o .\$@ $(LFLAGS) $(L_SDLL)
	
# ============================= PHONY RECEPIES =================================
.PHONY: all
all: clean $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) $(L_SDLL) -o $(BINDIR)\final.exe

.PHONY: link
link:
	$(CC) .\$^ $(LFLAGS) $(L_SDLL) -o $(BINDIR)\main.exe	
	
.PHONY: clean
clean:
	del $(OBJDIR)\*.o
	del $(BINDIR)\*.exe
	del $(DATDIR)\*.dat

.PHONY: archive
archive:
	$(7Z) a -tzip .\arc\"%DATE:~-4%%DATE:~4,2%%DATE:~7,2%".zip * -xr!obj -xr!bin -xr!arc
	
.PHONY: doxy
doxygen doxy:
	$(DOXY) doxygen.ini