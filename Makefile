include common.mk

MKFILES=01_Object.mk

.PHONY: $(MKFILES)
$(MKFILES):
	$(MAKE) -f $@ $(MAKECMDGOALS)

.PHONY: deps
deps: $(MKFILES)
	$(MAKE) -f $@ $(MAKECMDGOALS)

.PHONY: clean
clean:
	# don't accidentally rm /
ifdef BIN_DIR
	rm -vf $(BIN_DIR)/*
endif
ifdef OBJ_DIR
	rm -vf $(OBJ_DIR)/*
endif
ifdef DEP_DIR
	rm -vf $(DEP_DIR)/*
endif
