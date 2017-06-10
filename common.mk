ORX=/home/user/install/orx-1.8/dev-linux64

CC=gcc
CXX=g++
AR=ar

SRC_DIR=src
OBJ_DIR=obj
DEP_DIR=dep
BIN_DIR=bin

CFLAGS=-g -Wall -Werror -O3

CFLAGS+=-D__orxDEBUG__

INCLUDES=-I$(ORX)/include
CFLAGS+=$(INCLUDES)

# arch specific?
CFLAGS+=-ffast-math -msse2 -m64

# C++ ?
CFLAGS+=-fno-exceptions

LDFLAGS=-L$(ORX)/lib -Wl,-rpath,$(ORX)/lib -Wl,--export-dynamic -m64

LDLIBS=-lorxd -ldl -lm -lrt
