include mono2-before.mak

# file manager needs to know app name for calculating file paths
DEBUG_CC_FLAGS += -D cdwAppDataName=\"grtools\"
RELEASE_CC_FLAGS += -D cdwAppDataName=\"grtools\"

MY_BINS = \
	$(OUT_DIR)/debug/grtools.exe \
	$(OUT_DIR)/release/grtools.exe \

# add client components to all
all: $(MY_BINS) mono2_all

cleanLocal:
	rm -rf $(MY_BINS)
	rm -rf bin/obj/debug/grtools
	rm -rf bin/obj/release/grtools

.PHONY: all cleanLocal

include mono2-after.mak

# write rules for client components using those in mono2-after as examples

# ----------------------------------------------------------------------
# grtools

GRTOOLS_SRC = \
	src/grtools/help.cpp \
	src/grtools/main.cpp \

GRTOOLS_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(GRTOOLS_SRC)))

$(OUT_DIR)/debug/grtools.exe: $(GRTOOLS_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -o $@ $(GRTOOLS_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(GRTOOLS_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/grtools
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

GRTOOLS_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(GRTOOLS_SRC)))

$(OUT_DIR)/release/grtools.exe: $(GRTOOLS_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -o $@ $(GRTOOLS_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(GRTOOLS_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/grtools
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
