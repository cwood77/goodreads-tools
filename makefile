include mono2-before.mak

# file manager needs to know app name for calculating file paths
DEBUG_CC_FLAGS += -D cdwAppDataName=\"grtools\"
RELEASE_CC_FLAGS += -D cdwAppDataName=\"grtools\"

MY_BINS = \
	$(OUT_DIR)/debug/db.dll \
	$(OUT_DIR)/debug/db.test.dll \
	$(OUT_DIR)/debug/grtools.exe \
	$(OUT_DIR)/debug/tag.dll \
	$(OUT_DIR)/debug/title.dll \
	$(OUT_DIR)/release/db.dll \
	$(OUT_DIR)/release/db.test.dll \
	$(OUT_DIR)/release/grtools.exe \
	$(OUT_DIR)/release/tag.dll \
	$(OUT_DIR)/release/title.dll \

# add client components to all
all: $(MY_BINS) mono2_all

cleanLocal:
	rm -rf $(MY_BINS)
	rm -rf bin/obj/debug/db
	rm -rf bin/obj/debug/grtools
	rm -rf bin/obj/debug/tag
	rm -rf bin/obj/debug/title
	rm -rf bin/obj/release/db
	rm -rf bin/obj/release/grtools
	rm -rf bin/obj/release/tag
	rm -rf bin/obj/release/title

.PHONY: all cleanLocal

include mono2-after.mak

# write rules for client components using those in mono2-after as examples

# ----------------------------------------------------------------------
# db

DB_SRC = \
	src/db/api.cpp \
	src/db/main.cpp \
	src/db/parser.test.cpp \

DB_TEST_SRC = \
	src/db/main.test.cpp \

DB_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(DB_SRC)))

$(OUT_DIR)/debug/db.dll: $(DB_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(DB_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(DB_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/db
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

DB_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(DB_SRC)))

$(OUT_DIR)/release/db.dll: $(DB_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(DB_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(DB_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/db
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

DB_TEST_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(DB_TEST_SRC)))

$(OUT_DIR)/debug/db.test.dll: $(DB_TEST_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(DB_TEST_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(DB_TEST_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/db.test
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

DB_TEST_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(DB_TEST_SRC)))

$(OUT_DIR)/release/db.test.dll: $(DB_TEST_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(DB_TEST_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(DB_TEST_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/db.test
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# grtools

GRTOOLS_SRC = \
	src/grtools/help.cpp \
	src/grtools/main.cpp \
	src/grtools/verb.prepare.cpp \
	src/grtools/verb.split.cpp \

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

# ----------------------------------------------------------------------
# tag

TAG_SRC = \
	src/tag/api.cpp \
	src/tag/main.cpp \
	src/tag/wildcardExpert.cpp \

TAG_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(TAG_SRC)))

$(OUT_DIR)/debug/tag.dll: $(TAG_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(TAG_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(TAG_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/tag
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

TAG_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(TAG_SRC)))

$(OUT_DIR)/release/tag.dll: $(TAG_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(TAG_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(TAG_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/tag
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# title

TITLE_SRC = \
	src/title/api.cpp \
	src/title/default.cpp \
	src/title/main.cpp \
	src/title/sanitizer.cpp \

TITLE_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(TITLE_SRC)))

$(OUT_DIR)/debug/title.dll: $(TITLE_DEBUG_OBJ) $(OUT_DIR)/debug/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/debug
	@$(LINK_CMD) -shared -o $@ $(TITLE_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -ltcatlib

$(TITLE_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/debug/title
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

TITLE_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(TITLE_SRC)))

$(OUT_DIR)/release/title.dll: $(TITLE_RELEASE_OBJ) $(OUT_DIR)/release/tcatlib.lib
	$(info $< --> $@)
	@mkdir -p $(OUT_DIR)/release
	@$(LINK_CMD) -shared -o $@ $(TITLE_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -ltcatlib

$(TITLE_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@mkdir -p $(OBJ_DIR)/release/title
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
