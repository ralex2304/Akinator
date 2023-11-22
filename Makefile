.PHONY: all clean

CC = g++
CFLAGS = -fdiagnostics-color=always -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef			 \
		 -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs 		 \
		 -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual	 \
		 -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers \
		 -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual 			 \
		 -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel 		 \
		 -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

LIB_DIR = lib
LIB_FILES = TreeDebug/TreeDebug.lib

SRC_DIR = src
BUILD_DIR = build
DOCS_DIR = docs
NON_CODE_DIRS = $(BUILD_DIR) $(DOCS_DIR) .vscode .git
TARGET = main.exe

CD = $(shell cd)
DOCS_TARGET = $(DOCS_DIR)\\docs_generated

NESTED_CODE_DIRS_CD = $(shell dir $(SRC_DIR)\ /S /B /AD) # | findstr /V /I "$(NON_CODE_DIRS)"
NESTED_CODE_DIRS = $(NESTED_CODE_DIRS_CD:$(CD)\$(SRC_DIR)\\%=%)

FILES_FULL = $(shell dir "$(SRC_DIR)\*.cpp" /s /a /b)
FILES = $(FILES_FULL:$(CD)\$(SRC_DIR)\\%=%)

MAKE_DIRS = $(NESTED_CODE_DIRS:%=$(BUILD_DIR)\\%)
OBJ = $(FILES:%=$(BUILD_DIR)\\%)
DEPENDS = $(OBJ:%.cpp=%.d)
OBJECTS = $(OBJ:%.cpp=%.o)

LIBS = $(LIB_FILES:%=$(LIB_DIR)\\%)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) -I$(LIB_DIR) $^ -o $@ $(LIBS)

$(BUILD_DIR):
	@if not exist $@ md $@

$(MAKE_DIRS): | $(BUILD_DIR)
	@if not exist $@ md $@

-include $(DEPENDS)

$(BUILD_DIR)\\%.o: $(SRC_DIR)\\%.cpp | $(MAKE_DIRS)
	@$(CC) $(CFLAGS) -I$(LIB_DIR) -MMD -MP -c $< -o $@

.PHONY: doxygen dox

doxygen dox: $(DOCS_TARGET)

$(DOCS_TARGET): $(FILES:/%=%) | $(DOCS_DIR)
	@echo "Doxygen generated %date% %time%" > $(DOCS_TARGET)
	@doxygen docs/Doxyfile

$(DOCS_DIR):
	@if not exist $@ md $@

clean:
	@del $(BUILD_DIR)\* /a /s /q
	@del $(TARGET)
	@del $(DOCS_TARGET)
