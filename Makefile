
SHELL=/bin/bash
ifndef .VERBOSE
    .SILENT:
endif
ROOT=$(shell pwd)
ROOT=/home/arden/Programming/cpp/opero
CXX=clang++
CXXFLAGS=-std=c++17 -fPIC -Wall -Wpedantic
LINK=
SOURCE=src
INCLUDE_DIR=include
BUILD=build
COMMON_INCLUDE=-I$(ROOT)/$(INCLUDE_DIR) $(INCLUDE)

SCAN_COLOR=\033[1;35m
BUILD_COLOR=\033[32m
CLEAN_COLOR=\033[1;33m
LINK_COLOR=\033[1;32m
INSTALL_COLOR=\033[1;36m
CMD_COLOR=\033[1;34m
HELP_COLOR=\033[1;34m

define scan_target
printf "%b%s%b\n" "$(SCAN_COLOR)" "Scaning dependencies for target $(1)" "\033[0m"
endef
define complete_target
printf "%s\n" "Built target $(1)"
endef
define clean_target
printf "%b%s%b\n" "$(CLEAN_COLOR)" "Cleaning target $(1)" "\033[0m"
endef
define install_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Installing target $(1)" "\033[0m"
endef
define uninstall_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Unnstalling target $(1)" "\033[0m"
endef
define print_build_c
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C object $$str" "\033[0m"
endef
define print_build_cpp
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C++ object $$str" "\033[0m"
endef
define print_link_lib
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking static library $$str" "\033[0m"
endef
define print_link_exe
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking executable $$str" "\033[0m"
endef
define print_run_cmd
printf "%b%s%b\n" "$(CMD_COLOR)" "Running '$(1)'" "\033[0m"
endef
define help
printf "%b%*s%b: %s\n" "$(HELP_COLOR)" 20 "$(1)" "\033[0m" "$(2)"
endef

all: build-opero

clean: clean-opero

# OPERO {{{

OPERO=$(ROOT)/opero
OPERO_FILES=src/main.cpp
OPERO_OBJS=$(OPERO_FILES:%=$(ROOT)/$(BUILD)/%.o)
-include $(OPERO_OBJS:.o=.d)

build-opero:  pre-opero $(OPERO)
	$(call complete_target,$(shell basename $(OPERO)))

clean-opero:
	$(call clean_target,$(shell basename $(OPERO)))
	if [ -e "$(OPERO)" ]; then rm $(OPERO); fi

pre-opero:
	$(call scan_target,$(shell basename $(OPERO)))

$(OPERO): $(OPERO_OBJS) FORCE
	$(call print_link_exe,$(shell basename $(OPERO)))
	$(CXX) $(OPERO_OBJS)  $(LINK) $(COMMON_INCLUDE) -o $(OPERO)

install-opero: build-opero
	$(call install_target,$(shell basename $(OPERO)))
	mkdir -p $(INSTALL_PATH)/bin/
	cp $(OPERO) $(INSTALL_PATH)/bin

uninstall-opero:
	$(call uninstall_target,$(shell basename $(OPERO)))
	if [ -e "$(INSTALL_PATH)/bin/$(shell basename $(OPERO))" ]; then rm $(INSTALL_PATH)/bin/$(shell basename $(OPERO)); fi

# }}}

$(ROOT)/$(BUILD)/%.cpp.o: %.cpp
	$(call print_build_cpp,$@)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $(COMMON_INCLUDE) $< -o $@

FORCE:
