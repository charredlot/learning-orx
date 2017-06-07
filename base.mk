include common.mk

ORX=/home/user/install/orx-1.8/dev-linux64

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

####

SRC=$(SRC_DIR)/$(TARGET).c
OBJS=$(OBJ_DIR)/$(TARGET).o
DEPS=$(DEP_DIR)/$(TARGET).d

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(LDLIBS)

deps: $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d
	$(CC) $(CFLAGS) -c -o $@ $<

$(DEP_DIR)/%.d: $(SRC_DIR)/%.c
	$(CC) $(INCLUDES) -MM -MT $(OBJ_DIR)/$(basename $(notdir $@)).d -o $@ $<
