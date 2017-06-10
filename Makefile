include common.mk

MKFILES=01_Object.mk\
    02_Clock.mk\
    03_Frame.mk\
    04_Anim.mk\
    05_Viewport.mk\
    06_Sound.mk\
    07_FX.mk\
    08_Physics.mk\
    09_Scrolling.mk\
    10_Locale.mk\
    11_Spawner.mk\
    12_Lighting.mk

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
