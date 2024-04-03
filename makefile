ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifeq ($(detected_OS),Windows)
	RM = del /Q /S
	FixPath = $(subst /,\,$1)
	MD = if not exist $1 mkdir $1
	FixQuotes = $(subst ",,$1)
endif
ifeq ($(detected_OS),Linux)
	RM = rm -rf
 	FixPath = $1
	MD = mkdir -p $1
	PECHO = echo $1
	FixQuotes = $1
endif

CXX         := g++
CXXVERSION  := -std=c++17
CXXSTRICT   := -pedantic-errors -Werror
CXXWARN     := -Wall -Wextra -Wshadow -Wsign-conversion
CXXFLAGS    := $(CXXVERSION) $(CXXWARN) $(CXXSTRICT)
DEBUG_DIR   := $(call FixPath,./build/debug)
RELEASE_DIR := $(call FixPath,./build/release)
DBG_OBJ_DIR := $(call FixPath,$(DEBUG_DIR)/objects)
DBG_APP_DIR := $(call FixPath,$(DEBUG_DIR)/app)
REL_OBJ_DIR := $(call FixPath,$(RELEASE_DIR)/objects)
REL_APP_DIR := $(call FixPath,$(RELEASE_DIR)/app)
TARGET      := solver
INCLUDE     := -I $(call FixPath,include/)
SRC         :=                								  \
	$(call FixPath,$(wildcard src/*.cpp))         \
	$(call FixPath,$(wildcard src/utils/*.cpp))   \
	$(call FixPath,$(wildcard src/y2023/*.cpp))

REL_OBJECTS      := $(SRC:%.cpp=$(REL_OBJ_DIR)/%.o)
REL_DEPENDENCIES := $(REL_OBJECTS:.o=.d)

DBG_OBJECTS      := $(SRC:%.cpp=$(DBG_OBJ_DIR)/%.o)
DBG_DEPENDENCIES := $(DBG_OBJECTS:.o=.d)

# default build
all: release

# release objects
$(REL_OBJ_DIR)/%.o: %.cpp
	$(call MD,$(call FixPath,$(@D)))
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(call FixPath,$<) -MMD -o $(call FixPath,$@)

# release binary
$(REL_APP_DIR)/$(TARGET): $(REL_OBJECTS)
	$(call MD,$(call FixPath,$(@D)))
	$(CXX) $(CXXFLAGS) -o $(call FixPath,$(REL_APP_DIR)/$(TARGET)) $^

# debug objects
$(DBG_OBJ_DIR)/%.o: %.cpp
	$(call MD,$(call FixPath,$(@D)))
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(call FixPath,$<) -MMD -o $(call FixPath,$@)

# debug binary
$(DBG_APP_DIR)/$(TARGET): $(DBG_OBJECTS)
	$(call MD,$(call FixPath,$(@D)))
	$(CXX) $(CXXFLAGS) -o $(call FixPath,$(DBG_APP_DIR)/$(TARGET)) $^

# only needed if more makefiles are added to this project
# -include $(REL_DEPENDENCIES)

# do not interpret these names as files
.PHONY:
	all build clean release info
	build-debug clean-debug debug info-debug

build:
	$(call MD,$(call FixPath,$(REL_APP_DIR)))
	$(call MD,$(call FixPath,$(REL_OBJ_DIR)))

build-debug:
	$(call MD,$(call FixPath,$(DBG_APP_DIR)))
	$(call MD,$(call FixPath,$(DBG_OBJ_DIR)))

release: CXXFLAGS += -O2
release: build $(REL_APP_DIR)/$(TARGET)

debug: CXXFLAGS += -g
debug: build-debug $(DBG_APP_DIR)/$(TARGET)

clean:
	$(RM) $(call FixPath,$(REL_OBJ_DIR)/*)
	$(RM) $(call FixPath,$(REL_APP_DIR)/*)

clean-debug:
	$(RM) $(call FixPath,$(DBG_OBJ_DIR)/*)
	$(RM) $(call FixPath,$(DBG_APP_DIR)/*)

info:
	@echo $(call FixQuotes,"[*] Application dir: ${REL_APP_DIR}")
	@echo $(call FixQuotes,"[*] Objects dir:     ${REL_OBJ_DIR}")
	@echo $(call FixQuotes,"[*] Sources:         ${SRC}")
	@echo $(call FixQuotes,"[*] Objects:         ${REL_OBJECTS}")
	@echo $(call FixQuotes,"[*] Dependencies:    ${REL_DEPENDENCIES}")
	@echo $(call FixQuotes,"[*] Detected OS:     ${detected_OS}")

info-debug:
	@echo $(call FixQuotes,"Debug Information")
	@echo $(call FixQuotes,"[*] Application dir: ${DBG_APP_DIR}")
	@echo $(call FixQuotes,"[*] Objects dir:     ${DBG_OBJ_DIR}")
	@echo $(call FixQuotes,"[*] Sources:         ${SRC}")
	@echo $(call FixQuotes,"[*] Objects:         ${DBG_OBJECTS}")
	@echo $(call FixQuotes,"[*] Dependencies:    ${DBG_DEPENDENCIES}")
	@echo $(call FixQuotes,"[*] Detected OS:     ${detected_OS}")
