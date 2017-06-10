include common.mk

MKFILES=01_Object.mk 02_Clock.mk

.PHONY:
all: $(MKFILES)

.PHONY: $(MKFILES)
$(MKFILES):
	$(MAKE) -f $(MKFILES_DIR)/$@ $(MAKECMDGOALS)

.PHONY: deps
deps: $(MKFILES)
	$(MAKE) -f $@ $(MAKECMDGOALS)

.PHONY: clean
clean:
	# don't accidentally rm /
ifdef BIN_DIR
	find $(BIN_DIR)/* ! -name cfg ! -name .gitignore -delete
endif
ifdef OBJ_DIR
	rm -vf $(OBJ_DIR)/*
endif
ifdef DEP_DIR
	rm -vf $(DEP_DIR)/*
endif
