include common.mk

SRC=$(SRC_DIR)/$(TARGET).cpp
OBJS=$(OBJ_DIR)/$(TARGET).o
DEPS=$(DEP_DIR)/$(TARGET).d

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(LDLIBS)

deps: $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	$(CXX) $(CFLAGS) -c -o $@ $<

$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
	$(CXX) $(INCLUDES) -MM -MT $(OBJ_DIR)/$(basename $(notdir $@)).d -o $@ $<
