# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))
WRVX_COMPBASE := $(subst \,/,$(WRVX_COMPBASE))
WIND_KRNL_MK := $(subst \,/,$(WIND_KRNL_MK))
WIND_USR_MK := $(subst \,/,$(WIND_USR_MK))
WIND_BUILD := $(subst \,/,$(WIND_BUILD))
WIND_BUILD_TOOL := $(subst \,/,$(WIND_BUILD_TOOL))
WIND_LAYER_PATHS := $(subst \,/,$(WIND_LAYER_PATHS))

all : check_vsb pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

JOBS?=1
TARGET_JOBS?=$(JOBS)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = VSBraspi_CORTEX_A72llvm_LP64_ld
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/WindRiver/TFG_1S2025/RaspberryPi
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/GNC_TEST



#Global Build Macros
PROJECT_TYPE = RTP
DEFINES = 
DO_STRIP = 0
EXPAND_DBG = 0
APICHECK_OPTIONS = -warning


#BuildSpec specific Build Macros
VX_CPU_FAMILY = arm
CPU = CORTEX_A72
TOOL_FAMILY = llvm
TOOL_VERSION = llvm_2016_04
_CC_VERSION = llvm_11_1_0_1
TOOL = llvm
TOOL_PATH = $(WIND_HOME)/compilers/llvm-11.1.0.1/$(WIND_VX7_LLVM_HOST_TYPE)/bin
CC_ARCH_SPEC = -fno-builtin --target=arm64 -ffixed-x18 
LIBPATH = 
LIBS = --as-needed  -lc   -lc_internal   -lllvm   -lcplusplus   -lllvmcplus   -ldl 
VSB_DIR = $(PRJ_ROOT_DIR)/../VSBraspi
VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
BDGEN_VSB_INCLUDE = $(VSB_DIR)/h/config/auto.conf
APICHECK_CMD = apicheck.sh -r "$(VSB_DIR)/usr/apicheck/forbidden_api.lst" -n "nmarm64" -u -s "$<" "$@" $(APICHECK_OPTIONS)
APICHECK = 0

IDE_INCLUDES = -I$(VSB_DIR)/share/h -isystem$(VSB_DIR)/usr/h/public -IC:/WindRiver/TFG_1S2025/RaspberryPi/USB_DKM -I$(WS_ROOT_DIR)/VxMAESLib 

IDE_LIBRARIES = 

IDE_DEFINES = -D_VX_CPU=_VX_$(CPU) -D_VX_TOOL_FAMILY=$(TOOL_FAMILY) -D_VX_TOOL=$(TOOL) -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\" -DARMEL  



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -gdwarf-3
DEBUGFLAGS_C++-Compiler = -gdwarf-3
DEBUGFLAGS_Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -gdwarf-3
else
DEBUGFLAGS_C-Compiler =  -O2
DEBUGFLAGS_C++-Compiler =  -O2
DEBUGFLAGS_Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler =  -O2
endif


#Project Targets
PROJECT_TARGETS = GNC_TEST/$(MODE_DIR)/GNC_TEST.vxe


#Rules

# GNC_TEST
ifeq ($(DEBUG_MODE),1)
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -gdwarf-3
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -gdwarf-3
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_Linker = 
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -gdwarf-3
else
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler =  -O2
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler =  -O2
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_Linker = 
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
GNC_TEST/$(MODE_DIR)/% : DEBUGFLAGS_Assembler =  -O2
endif
GNC_TEST/$(MODE_DIR)/% : IDE_INCLUDES = -I$(VSB_DIR)/share/h -isystem$(VSB_DIR)/usr/h/public -IC:/WindRiver/TFG_1S2025/RaspberryPi/USB_DKM -I$(WS_ROOT_DIR)/VxMAESLib 
GNC_TEST/$(MODE_DIR)/% : IDE_LIBRARIES = 
GNC_TEST/$(MODE_DIR)/% : IDE_DEFINES = -D_VX_CPU=_VX_$(CPU) -D_VX_TOOL_FAMILY=$(TOOL_FAMILY) -D_VX_TOOL=$(TOOL) -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\" -DARMEL  
GNC_TEST/$(MODE_DIR)/% : PROJECT_TYPE = RTP
GNC_TEST/$(MODE_DIR)/% : DEFINES = 
GNC_TEST/$(MODE_DIR)/% : DO_STRIP = 0
GNC_TEST/$(MODE_DIR)/% : EXPAND_DBG = 0
GNC_TEST/$(MODE_DIR)/% : APICHECK_OPTIONS = -warning
GNC_TEST/$(MODE_DIR)/% : VX_CPU_FAMILY = arm
GNC_TEST/$(MODE_DIR)/% : CPU = CORTEX_A72
GNC_TEST/$(MODE_DIR)/% : TOOL_FAMILY = llvm
GNC_TEST/$(MODE_DIR)/% : TOOL_VERSION = llvm_2016_04
GNC_TEST/$(MODE_DIR)/% : _CC_VERSION = llvm_11_1_0_1
GNC_TEST/$(MODE_DIR)/% : TOOL = llvm
GNC_TEST/$(MODE_DIR)/% : TOOL_PATH = $(WIND_HOME)/compilers/llvm-11.1.0.1/$(WIND_VX7_LLVM_HOST_TYPE)/bin
GNC_TEST/$(MODE_DIR)/% : CC_ARCH_SPEC = -fno-builtin --target=arm64 -ffixed-x18 
GNC_TEST/$(MODE_DIR)/% : LIBPATH = 
GNC_TEST/$(MODE_DIR)/% : LIBS = --as-needed  -lc   -lc_internal   -lllvm   -lcplusplus   -lllvmcplus   -ldl 
GNC_TEST/$(MODE_DIR)/% : VSB_DIR = $(PRJ_ROOT_DIR)/../VSBraspi
GNC_TEST/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
GNC_TEST/$(MODE_DIR)/% : BDGEN_VSB_INCLUDE = $(VSB_DIR)/h/config/auto.conf
GNC_TEST/$(MODE_DIR)/% : APICHECK_CMD = apicheck.sh -r "$(VSB_DIR)/usr/apicheck/forbidden_api.lst" -n "nmarm64" -u -s "$<" "$@" $(APICHECK_OPTIONS)
GNC_TEST/$(MODE_DIR)/% : APICHECK = 0
GNC_TEST/$(MODE_DIR)/% : OBJ_DIR := GNC_TEST/$(MODE_DIR)

GNC_TEST/$(MODE_DIR)/Objects/GNC_TEST/gnctest.o : $(PRJ_ROOT_DIR)/gnctest.c $(FORCE_FILE_BUILD) GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)/clang $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -std=c11 -fno-strict-aliasing  -fno-builtin -D__vxworks -D__VXWORKS__ -D__RTP__ -D__ELF__ -nostdlibinc -nostdinc++ -mllvm -two-entry-phi-node-folding-threshold=2 -D_USE_INIT_ARRAY -fasm -Wall -Wconversion -Wno-sign-conversion    -MD -MP  $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) \@GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list $(ADDED_INCLUDES) -o "$@" -c "$<" && if [ "$(APICHECK)" = "1" ]; then $(APICHECK_CMD);fi


OBJECTS_GNC_TEST = GNC_TEST/$(MODE_DIR)/Objects/GNC_TEST/gnctest.o \
	 $(WS_ROOT_DIR)/VxMAESLib/VSBraspi_CORTEX_A72llvm_LP64_ld/libVxMAESLib/$(MODE_DIR)/libVxMAESLib.so


GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list.new:checkDirGNC_TEST
	$(file >$@,$(IDE_INCLUDES))

GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list.flag:GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list.new
	@cmp -s GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list.new GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list:GNC_TEST/$(MODE_DIR)/GNC_TEST.includes.list.flag
	$(file >$@,$(IDE_INCLUDES))

checkDirGNC_TEST : 
	mkdir -p GNC_TEST/$(MODE_DIR)/

GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list.new:checkDirGNC_TEST
	$(file >$@,$(OBJECTS_GNC_TEST))

GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list.flag:GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list.new
	@cmp -s GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list.new GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list:$(OBJECTS_GNC_TEST) GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list.flag
	$(file >$@,$(OBJECTS_GNC_TEST))

ifeq ($(TARGET_JOBS),1)
GNC_TEST/$(MODE_DIR)/GNC_TEST.vxe : GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list | check_objdirs
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";   $(TOOL_PATH)/ldarm64 -m aarch64elf $(DEBUGFLAGS_Linker) --defsym __wrs_rtp_base=0x80000000 -u __wr_need_frame_add -u __tls__ -T$(VSB_DIR)/usr/ldscripts/rtp.ld -EL $(VSB_DIR)/usr/lib/common/crt0.o -o "$@" @GNC_TEST/$(MODE_DIR)/.GNC_TESTObject.list $(ADDED_OBJECTS) $(LIBPATH) -L$(VSB_DIR)/usr/lib/common $(ADDED_LIBPATH) --start-group $(LIBS) $(IDE_LIBRARIES) $(ADDED_LIBS) --end-group && if [ "$(DO_STRIP)" = "1" ]; then mv -f "$@" "$@.unstripped" && $(TOOL_PATH)/striparm64 -g -o "$@" "$@.unstripped";fi && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi  && $(if $(_WRS_CONFIG_SECURE_LOADER),signtool  --key=$(_WRS_CONFIG_SECURE_LOADER_PRIVATE_SIGNING_KEY_FILE) --cert=$(_WRS_CONFIG_SECURE_LOADER_PUBLIC_SIGNING_CERT_FILE) --type=ELF $@,true) && true

else
GNC_TEST/$(MODE_DIR)/GNC_TEST.vxe : GNC_TEST/$(MODE_DIR)/GNC_TEST.vxe_jobs

endif
GNC_TEST/$(MODE_DIR)/GNC_TEST_compile_file : $(FILE) ;

_clean :: GNC_TEST/$(MODE_DIR)/GNC_TEST_clean

GNC_TEST/$(MODE_DIR)/GNC_TEST_clean : 
	$(TRACE_FLAG)if [ -d "GNC_TEST" ]; then cd "GNC_TEST"; rm -rf $(MODE_DIR); fi

$(WS_ROOT_DIR)/VxMAESLib/VSBraspi_CORTEX_A72llvm_LP64_ld/libVxMAESLib/$(MODE_DIR)/libVxMAESLib.so : force_referenced_rebuilds_VxMAESLib ;

force_referenced_rebuilds_VxMAESLib : 
	$(TRACE_FLAG)+cd $(WS_ROOT_DIR)/VxMAESLib/VSBraspi_CORTEX_A72llvm_LP64_ld;\
	make --no-print-directory BUILD_SPEC=VSBraspi_CORTEX_A72llvm_LP64_ld DEBUG_MODE=$(DEBUG_MODE) libVxMAESLib/$(MODE_DIR)/libVxMAESLib.so

force : 

TARGET_JOBS_RULE?=echo "Update the makefile template via File > Import > Build Settings : Update makefile template";exit 1
%_jobs : 
	$(TRACE_FLAG)$(TARGET_JOBS_RULE)

DEP_FILES := GNC_TEST/$(MODE_DIR)/Objects/GNC_TEST/gnctest.d
-include $(DEP_FILES)

OBJ_DIRS := GNC_TEST/$(MODE_DIR)/Objects/GNC_TEST
check_objdirs : 
	$(TRACE_FLAG)echo "Creating object dirs";\
	for dir in $(OBJ_DIRS); do\
		if [ ! -d "$$dir" ]; then mkdir -p "$$dir"; fi;\
	done


ifdef BDGEN_VSB_INCLUDE
include $(BDGEN_VSB_INCLUDE)
endif

-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

TARGET_JOBS_RULE=$(MAKE) -f $(MAKEFILE) --jobs $(TARGET_JOBS) $(MFLAGS) $* TARGET_JOBS=1
ifeq ($(JOBS),1)
main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"
else
main_all : external_build 
	@$(MAKE) -f $(MAKEFILE) --jobs $(JOBS) $(MFLAGS) $(PROJECT_TARGETS) TARGET_JOBS=1 &&\
	echo "make: built targets of `pwd`"
endif 

check_vsb :
	@if [ "$(SKIP_VSB_CHECK)" = "" ]; then\
		if [ -f $(WIND_BASE)/build/mk/scripts/check_vsb.sh ]; then\
			jobsoption="";\
			if [ "$(JOBS)" != "" ]; then\
				jobsoption="-j $(JOBS)";\
			fi;\
			sh $(WIND_BASE)/build/mk/scripts/check_vsb.sh $$jobsoption $(VSB_DIR);\
		fi;\
	fi

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: check_objdirs $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
