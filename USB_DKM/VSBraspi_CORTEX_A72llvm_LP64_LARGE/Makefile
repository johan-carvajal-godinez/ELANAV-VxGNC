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

BUILD_SPEC = VSBraspi_CORTEX_A72llvm_LP64_LARGE
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/WindRiver/TFG_1S2025/RaspberryPi
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/USB_DKM



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
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
VSB_DIR = $(PRJ_ROOT_DIR)/../VSBraspi
VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
LIBPATH = 
LIBS = 
DKM_LONGCALL =  
BDGEN_VSB_INCLUDE = $(VSB_DIR)/h/config/auto.conf
APICHECK_CMD = apicheck.sh -r "$(VSB_DIR)/krnl/apicheck/forbidden_api.lst" -n "nmarm64" -u -s "$<" "$@" $(APICHECK_OPTIONS)
APICHECK = 0

IDE_INCLUDES = -I$(VSB_DIR)/share/h -isystem$(VSB_DIR)/krnl/h/system -isystem$(VSB_DIR)/krnl/h/public 

IDE_LIBRARIES = 

IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\" -DARMEL  -U__USER_LABEL_PREFIX__ -D__USER_LABEL_PREFIX__= 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -O2
DEBUGFLAGS_C++-Compiler = -O2
DEBUGFLAGS_Linker = -O2
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2
endif


#Project Targets
PROJECT_TARGETS = USB_DKM/$(MODE_DIR)/USB_DKM.out \
	 USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.o


#Rules

# USB_DKM
ifeq ($(DEBUG_MODE),1)
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
USB_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2
endif
USB_DKM/$(MODE_DIR)/% : IDE_INCLUDES = -I$(VSB_DIR)/share/h -isystem$(VSB_DIR)/krnl/h/system -isystem$(VSB_DIR)/krnl/h/public 
USB_DKM/$(MODE_DIR)/% : IDE_LIBRARIES = 
USB_DKM/$(MODE_DIR)/% : IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\" -DARMEL  -U__USER_LABEL_PREFIX__ -D__USER_LABEL_PREFIX__= 
USB_DKM/$(MODE_DIR)/% : PROJECT_TYPE = DKM
USB_DKM/$(MODE_DIR)/% : DEFINES = 
USB_DKM/$(MODE_DIR)/% : EXPAND_DBG = 0
USB_DKM/$(MODE_DIR)/% : APICHECK_OPTIONS = -warning
USB_DKM/$(MODE_DIR)/% : VX_CPU_FAMILY = arm
USB_DKM/$(MODE_DIR)/% : CPU = CORTEX_A72
USB_DKM/$(MODE_DIR)/% : TOOL_FAMILY = llvm
USB_DKM/$(MODE_DIR)/% : TOOL_VERSION = llvm_2016_04
USB_DKM/$(MODE_DIR)/% : _CC_VERSION = llvm_11_1_0_1
USB_DKM/$(MODE_DIR)/% : TOOL = llvm
USB_DKM/$(MODE_DIR)/% : TOOL_PATH = $(WIND_HOME)/compilers/llvm-11.1.0.1/$(WIND_VX7_LLVM_HOST_TYPE)/bin
USB_DKM/$(MODE_DIR)/% : CC_ARCH_SPEC = -fno-builtin --target=arm64 -ffixed-x18 
USB_DKM/$(MODE_DIR)/% : VSB_DIR = $(PRJ_ROOT_DIR)/../VSBraspi
USB_DKM/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
USB_DKM/$(MODE_DIR)/% : LIBPATH = 
USB_DKM/$(MODE_DIR)/% : LIBS = 
USB_DKM/$(MODE_DIR)/% : DKM_LONGCALL =  
USB_DKM/$(MODE_DIR)/% : BDGEN_VSB_INCLUDE = $(VSB_DIR)/h/config/auto.conf
USB_DKM/$(MODE_DIR)/% : APICHECK_CMD = apicheck.sh -r "$(VSB_DIR)/krnl/apicheck/forbidden_api.lst" -n "nmarm64" -u -s "$<" "$@" $(APICHECK_OPTIONS)
USB_DKM/$(MODE_DIR)/% : APICHECK = 0
USB_DKM/$(MODE_DIR)/% : OBJ_DIR := USB_DKM/$(MODE_DIR)

OBJECTS_USB_DKM = USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.o


USB_DKM/$(MODE_DIR)/USB_DKM.includes.list.new:checkDirUSB_DKM
	$(file >$@,$(IDE_INCLUDES))

USB_DKM/$(MODE_DIR)/USB_DKM.includes.list.flag:USB_DKM/$(MODE_DIR)/USB_DKM.includes.list.new
	@cmp -s USB_DKM/$(MODE_DIR)/USB_DKM.includes.list.new USB_DKM/$(MODE_DIR)/USB_DKM.includes.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

USB_DKM/$(MODE_DIR)/USB_DKM.includes.list:USB_DKM/$(MODE_DIR)/USB_DKM.includes.list.flag
	$(file >$@,$(IDE_INCLUDES))

checkDirUSB_DKM : 
	mkdir -p USB_DKM/$(MODE_DIR)/

USB_DKM/$(MODE_DIR)/.USB_DKMObject.list.new:checkDirUSB_DKM
	$(file >$@,$(OBJECTS_USB_DKM))

USB_DKM/$(MODE_DIR)/.USB_DKMObject.list.flag:USB_DKM/$(MODE_DIR)/.USB_DKMObject.list.new
	@cmp -s USB_DKM/$(MODE_DIR)/.USB_DKMObject.list.new USB_DKM/$(MODE_DIR)/.USB_DKMObject.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

USB_DKM/$(MODE_DIR)/.USB_DKMObject.list:$(OBJECTS_USB_DKM) USB_DKM/$(MODE_DIR)/.USB_DKMObject.list.flag
	$(file >$@,$(OBJECTS_USB_DKM))

ifeq ($(TARGET_JOBS),1)
USB_DKM/$(MODE_DIR)/USB_DKM.out : USB_DKM/$(MODE_DIR)/.USB_DKMObject.list  USB_DKM/$(MODE_DIR)/USB_DKM.includes.list | check_objdirs
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";$(TOOL_PATH)/nmarm64 @USB_DKM/$(MODE_DIR)/.USB_DKMObject.list | tclsh $(WIND_BASE)/build/mk/tcl/munch.tcl -c arm64 -tags $(VSB_DIR)/krnl/tags/dkm.tags > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)/clang $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -fdollars-in-identifiers -Wall -Wconversion -Wno-sign-conversion   $(ADDED_CFLAGS) \@USB_DKM/$(MODE_DIR)/USB_DKM.includes.list $(ADDED_INCLUDES)  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c;  $(TOOL_PATH)/ldarm64  -X -r -EL  --eh-frame-hdr  --force-group-allocation -T $(VSB_DIR)/krnl/ldscripts/dkm.ld -o "$@" $(OBJ_DIR)/ctdt.o @USB_DKM/$(MODE_DIR)/.USB_DKMObject.list $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi && $(if $(_WRS_CONFIG_SECURE_LOADER),signtool  --key=$(_WRS_CONFIG_SECURE_LOADER_PRIVATE_SIGNING_KEY_FILE) --cert=$(_WRS_CONFIG_SECURE_LOADER_PUBLIC_SIGNING_CERT_FILE) --type=ELF $@,true) && true

else
USB_DKM/$(MODE_DIR)/USB_DKM.out : USB_DKM/$(MODE_DIR)/USB_DKM.out_jobs

endif
USB_DKM/$(MODE_DIR)/USB_DKM_compile_file : $(FILE) ;

_clean :: USB_DKM/$(MODE_DIR)/USB_DKM_clean

USB_DKM/$(MODE_DIR)/USB_DKM_clean : 
	$(TRACE_FLAG)if [ -d "USB_DKM" ]; then cd "USB_DKM"; rm -rf $(MODE_DIR); fi


# USB_DKM_partialImage
ifeq ($(DEBUG_MODE),1)
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
USB_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2
endif
USB_DKM_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(VSB_DIR)/share/h -isystem$(VSB_DIR)/krnl/h/system -isystem$(VSB_DIR)/krnl/h/public 
USB_DKM_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = 
USB_DKM_partialImage/$(MODE_DIR)/% : IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\" -DARMEL  -U__USER_LABEL_PREFIX__ -D__USER_LABEL_PREFIX__= 
USB_DKM_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
USB_DKM_partialImage/$(MODE_DIR)/% : DEFINES = 
USB_DKM_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
USB_DKM_partialImage/$(MODE_DIR)/% : APICHECK_OPTIONS = -warning
USB_DKM_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = arm
USB_DKM_partialImage/$(MODE_DIR)/% : CPU = CORTEX_A72
USB_DKM_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = llvm
USB_DKM_partialImage/$(MODE_DIR)/% : TOOL_VERSION = llvm_2016_04
USB_DKM_partialImage/$(MODE_DIR)/% : _CC_VERSION = llvm_11_1_0_1
USB_DKM_partialImage/$(MODE_DIR)/% : TOOL = llvm
USB_DKM_partialImage/$(MODE_DIR)/% : TOOL_PATH = $(WIND_HOME)/compilers/llvm-11.1.0.1/$(WIND_VX7_LLVM_HOST_TYPE)/bin
USB_DKM_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -fno-builtin --target=arm64 -ffixed-x18 
USB_DKM_partialImage/$(MODE_DIR)/% : VSB_DIR = $(PRJ_ROOT_DIR)/../VSBraspi
USB_DKM_partialImage/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
USB_DKM_partialImage/$(MODE_DIR)/% : LIBPATH = 
USB_DKM_partialImage/$(MODE_DIR)/% : LIBS = 
USB_DKM_partialImage/$(MODE_DIR)/% : DKM_LONGCALL =  
USB_DKM_partialImage/$(MODE_DIR)/% : BDGEN_VSB_INCLUDE = $(VSB_DIR)/h/config/auto.conf
USB_DKM_partialImage/$(MODE_DIR)/% : APICHECK_CMD = apicheck.sh -r "$(VSB_DIR)/krnl/apicheck/forbidden_api.lst" -n "nmarm64" -u -s "$<" "$@" $(APICHECK_OPTIONS)
USB_DKM_partialImage/$(MODE_DIR)/% : APICHECK = 0
USB_DKM_partialImage/$(MODE_DIR)/% : OBJ_DIR := USB_DKM_partialImage/$(MODE_DIR)

USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbHandler.o : $(PRJ_ROOT_DIR)/usbHandler.c $(FORCE_FILE_BUILD) USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)/clang $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -Wall -Wconversion -Wno-sign-conversion    -MD -MP  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) \@USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list $(ADDED_INCLUDES) -o "$@" -c "$<" && if [ "$(APICHECK)" = "1" ]; then $(APICHECK_CMD);fi


USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbLauncher.o : $(PRJ_ROOT_DIR)/usbLauncher.c $(FORCE_FILE_BUILD) USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)/clang $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -Wall -Wconversion -Wno-sign-conversion    -MD -MP  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) \@USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list $(ADDED_INCLUDES) -o "$@" -c "$<" && if [ "$(APICHECK)" = "1" ]; then $(APICHECK_CMD);fi


USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbMessageHandler.o : $(PRJ_ROOT_DIR)/usbMessageHandler.c $(FORCE_FILE_BUILD) USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)/clang $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -Wall -Wconversion -Wno-sign-conversion    -MD -MP  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) \@USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list $(ADDED_INCLUDES) -o "$@" -c "$<" && if [ "$(APICHECK)" = "1" ]; then $(APICHECK_CMD);fi


OBJECTS_USB_DKM_partialImage = USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbHandler.o \
	 USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbLauncher.o \
	 USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbMessageHandler.o


USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list.new:checkDirUSB_DKM_partialImage
	$(file >$@,$(IDE_INCLUDES))

USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list.flag:USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list.new
	@cmp -s USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list.new USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list:USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.includes.list.flag
	$(file >$@,$(IDE_INCLUDES))

checkDirUSB_DKM_partialImage : 
	mkdir -p USB_DKM_partialImage/$(MODE_DIR)/

USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list.new:checkDirUSB_DKM_partialImage
	$(file >$@,$(OBJECTS_USB_DKM_partialImage))

USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list.flag:USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list.new
	@cmp -s USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list.new USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list:$(OBJECTS_USB_DKM_partialImage) USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list.flag
	$(file >$@,$(OBJECTS_USB_DKM_partialImage))

ifeq ($(TARGET_JOBS),1)
USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.o : USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list | check_objdirs
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";  $(TOOL_PATH)/ldarm64  -X -r -EL  --eh-frame-hdr   -o "$@" @USB_DKM_partialImage/$(MODE_DIR)/.USB_DKM_partialImageObject.list $(ADDED_OBJECTS) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

else
USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.o : USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage.o_jobs

endif
USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage_compile_file : $(FILE) ;

_clean :: USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage_clean

USB_DKM_partialImage/$(MODE_DIR)/USB_DKM_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "USB_DKM_partialImage" ]; then cd "USB_DKM_partialImage"; rm -rf $(MODE_DIR); fi

force : 

TARGET_JOBS_RULE?=echo "Update the makefile template via File > Import > Build Settings : Update makefile template";exit 1
%_jobs : 
	$(TRACE_FLAG)$(TARGET_JOBS_RULE)

DEP_FILES := USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbHandler.d USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbLauncher.d USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM/usbMessageHandler.d
-include $(DEP_FILES)

OBJ_DIRS := USB_DKM_partialImage/$(MODE_DIR)/Objects/USB_DKM
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
