include common.mk

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
