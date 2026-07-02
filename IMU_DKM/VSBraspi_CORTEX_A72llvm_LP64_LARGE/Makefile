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
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/IMU_DKM



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
PROJECT_TARGETS = IMU_DKM/$(MODE_DIR)/IMU_DKM.out \
	 IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.o


#Rules

# IMU_DKM
ifeq ($(DEBUG_MODE),1)
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
IMU_DKM/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2
endif
IMU_DKM/$(MODE_DIR)/% : IDE_INCLUDES = -I$(VSB_DIR)/share/h -isystem$(VSB_DIR)/krnl/h/system -isystem$(VSB_DIR)/krnl/h/public 
IMU_DKM/$(MODE_DIR)/% : IDE_LIBRARIES = 
IMU_DKM/$(MODE_DIR)/% : IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\" -DARMEL  -U__USER_LABEL_PREFIX__ -D__USER_LABEL_PREFIX__= 
IMU_DKM/$(MODE_DIR)/% : PROJECT_TYPE = DKM
IMU_DKM/$(MODE_DIR)/% : DEFINES = 
IMU_DKM/$(MODE_DIR)/% : EXPAND_DBG = 0
IMU_DKM/$(MODE_DIR)/% : APICHECK_OPTIONS = -warning
IMU_DKM/$(MODE_DIR)/% : VX_CPU_FAMILY = arm
IMU_DKM/$(MODE_DIR)/% : CPU = CORTEX_A72
IMU_DKM/$(MODE_DIR)/% : TOOL_FAMILY = llvm
IMU_DKM/$(MODE_DIR)/% : TOOL_VERSION = llvm_2016_04
IMU_DKM/$(MODE_DIR)/% : _CC_VERSION = llvm_11_1_0_1
IMU_DKM/$(MODE_DIR)/% : TOOL = llvm
IMU_DKM/$(MODE_DIR)/% : TOOL_PATH = $(WIND_HOME)/compilers/llvm-11.1.0.1/$(WIND_VX7_LLVM_HOST_TYPE)/bin
IMU_DKM/$(MODE_DIR)/% : CC_ARCH_SPEC = -fno-builtin --target=arm64 -ffixed-x18 
IMU_DKM/$(MODE_DIR)/% : VSB_DIR = $(PRJ_ROOT_DIR)/../VSBraspi
IMU_DKM/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
IMU_DKM/$(MODE_DIR)/% : LIBPATH = 
IMU_DKM/$(MODE_DIR)/% : LIBS = 
IMU_DKM/$(MODE_DIR)/% : DKM_LONGCALL =  
IMU_DKM/$(MODE_DIR)/% : BDGEN_VSB_INCLUDE = $(VSB_DIR)/h/config/auto.conf
IMU_DKM/$(MODE_DIR)/% : APICHECK_CMD = apicheck.sh -r "$(VSB_DIR)/krnl/apicheck/forbidden_api.lst" -n "nmarm64" -u -s "$<" "$@" $(APICHECK_OPTIONS)
IMU_DKM/$(MODE_DIR)/% : APICHECK = 0
IMU_DKM/$(MODE_DIR)/% : OBJ_DIR := IMU_DKM/$(MODE_DIR)

OBJECTS_IMU_DKM = IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.o


IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list.new:checkDirIMU_DKM
	$(file >$@,$(IDE_INCLUDES))

IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list.flag:IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list.new
	@cmp -s IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list.new IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list:IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list.flag
	$(file >$@,$(IDE_INCLUDES))

checkDirIMU_DKM : 
	mkdir -p IMU_DKM/$(MODE_DIR)/

IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list.new:checkDirIMU_DKM
	$(file >$@,$(OBJECTS_IMU_DKM))

IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list.flag:IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list.new
	@cmp -s IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list.new IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list:$(OBJECTS_IMU_DKM) IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list.flag
	$(file >$@,$(OBJECTS_IMU_DKM))

ifeq ($(TARGET_JOBS),1)
IMU_DKM/$(MODE_DIR)/IMU_DKM.out : IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list  IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list | check_objdirs
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";$(TOOL_PATH)/nmarm64 @IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list | tclsh $(WIND_BASE)/build/mk/tcl/munch.tcl -c arm64 -tags $(VSB_DIR)/krnl/tags/dkm.tags > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)/clang $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -fdollars-in-identifiers -Wall -Wconversion -Wno-sign-conversion   $(ADDED_CFLAGS) \@IMU_DKM/$(MODE_DIR)/IMU_DKM.includes.list $(ADDED_INCLUDES)  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c;  $(TOOL_PATH)/ldarm64  -X -r -EL  --eh-frame-hdr  --force-group-allocation -T $(VSB_DIR)/krnl/ldscripts/dkm.ld -o "$@" $(OBJ_DIR)/ctdt.o @IMU_DKM/$(MODE_DIR)/.IMU_DKMObject.list $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi && $(if $(_WRS_CONFIG_SECURE_LOADER),signtool  --key=$(_WRS_CONFIG_SECURE_LOADER_PRIVATE_SIGNING_KEY_FILE) --cert=$(_WRS_CONFIG_SECURE_LOADER_PUBLIC_SIGNING_CERT_FILE) --type=ELF $@,true) && true

else
IMU_DKM/$(MODE_DIR)/IMU_DKM.out : IMU_DKM/$(MODE_DIR)/IMU_DKM.out_jobs

endif
IMU_DKM/$(MODE_DIR)/IMU_DKM_compile_file : $(FILE) ;

_clean :: IMU_DKM/$(MODE_DIR)/IMU_DKM_clean

IMU_DKM/$(MODE_DIR)/IMU_DKM_clean : 
	$(TRACE_FLAG)if [ -d "IMU_DKM" ]; then cd "IMU_DKM"; rm -rf $(MODE_DIR); fi


# IMU_DKM_partialImage
ifeq ($(DEBUG_MODE),1)
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
IMU_DKM_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2
endif
IMU_DKM_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(VSB_DIR)/share/h -isystem$(VSB_DIR)/krnl/h/system -isystem$(VSB_DIR)/krnl/h/public 
IMU_DKM_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = 
IMU_DKM_partialImage/$(MODE_DIR)/% : IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\" -DARMEL  -U__USER_LABEL_PREFIX__ -D__USER_LABEL_PREFIX__= 
IMU_DKM_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
IMU_DKM_partialImage/$(MODE_DIR)/% : DEFINES = 
IMU_DKM_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
IMU_DKM_partialImage/$(MODE_DIR)/% : APICHECK_OPTIONS = -warning
IMU_DKM_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = arm
IMU_DKM_partialImage/$(MODE_DIR)/% : CPU = CORTEX_A72
IMU_DKM_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = llvm
IMU_DKM_partialImage/$(MODE_DIR)/% : TOOL_VERSION = llvm_2016_04
IMU_DKM_partialImage/$(MODE_DIR)/% : _CC_VERSION = llvm_11_1_0_1
IMU_DKM_partialImage/$(MODE_DIR)/% : TOOL = llvm
IMU_DKM_partialImage/$(MODE_DIR)/% : TOOL_PATH = $(WIND_HOME)/compilers/llvm-11.1.0.1/$(WIND_VX7_LLVM_HOST_TYPE)/bin
IMU_DKM_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -fno-builtin --target=arm64 -ffixed-x18 
IMU_DKM_partialImage/$(MODE_DIR)/% : VSB_DIR = $(PRJ_ROOT_DIR)/../VSBraspi
IMU_DKM_partialImage/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
IMU_DKM_partialImage/$(MODE_DIR)/% : LIBPATH = 
IMU_DKM_partialImage/$(MODE_DIR)/% : LIBS = 
IMU_DKM_partialImage/$(MODE_DIR)/% : DKM_LONGCALL =  
IMU_DKM_partialImage/$(MODE_DIR)/% : BDGEN_VSB_INCLUDE = $(VSB_DIR)/h/config/auto.conf
IMU_DKM_partialImage/$(MODE_DIR)/% : APICHECK_CMD = apicheck.sh -r "$(VSB_DIR)/krnl/apicheck/forbidden_api.lst" -n "nmarm64" -u -s "$<" "$@" $(APICHECK_OPTIONS)
IMU_DKM_partialImage/$(MODE_DIR)/% : APICHECK = 0
IMU_DKM_partialImage/$(MODE_DIR)/% : OBJ_DIR := IMU_DKM_partialImage/$(MODE_DIR)

IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cHandler.o : $(PRJ_ROOT_DIR)/i2cHandler.c $(FORCE_FILE_BUILD) IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)/clang $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -Wall -Wconversion -Wno-sign-conversion    -MD -MP  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) \@IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list $(ADDED_INCLUDES) -o "$@" -c "$<" && if [ "$(APICHECK)" = "1" ]; then $(APICHECK_CMD);fi


IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cLauncher.o : $(PRJ_ROOT_DIR)/i2cLauncher.c $(FORCE_FILE_BUILD) IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)/clang $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -Wall -Wconversion -Wno-sign-conversion    -MD -MP  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) \@IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list $(ADDED_INCLUDES) -o "$@" -c "$<" && if [ "$(APICHECK)" = "1" ]; then $(APICHECK_CMD);fi


IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cMessageHandler.o : $(PRJ_ROOT_DIR)/i2cMessageHandler.c $(FORCE_FILE_BUILD) IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)/clang $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -std=c11 -D__vxworks -D__VXWORKS__ -D__ELF__ -D_HAVE_TOOL_XTORS  -nostdlibinc -nostdinc++ -ftls-model=local-exec -fno-builtin -fno-strict-aliasing -D_USE_INIT_ARRAY -mllvm -two-entry-phi-node-folding-threshold=2 -mno-implicit-float -Wall -Wconversion -Wno-sign-conversion    -MD -MP  $(DKM_LONGCALL) $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) \@IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list $(ADDED_INCLUDES) -o "$@" -c "$<" && if [ "$(APICHECK)" = "1" ]; then $(APICHECK_CMD);fi


OBJECTS_IMU_DKM_partialImage = IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cHandler.o \
	 IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cLauncher.o \
	 IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cMessageHandler.o


IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list.new:checkDirIMU_DKM_partialImage
	$(file >$@,$(IDE_INCLUDES))

IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list.flag:IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list.new
	@cmp -s IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list.new IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list:IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.includes.list.flag
	$(file >$@,$(IDE_INCLUDES))

checkDirIMU_DKM_partialImage : 
	mkdir -p IMU_DKM_partialImage/$(MODE_DIR)/

IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list.new:checkDirIMU_DKM_partialImage
	$(file >$@,$(OBJECTS_IMU_DKM_partialImage))

IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list.flag:IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list.new
	@cmp -s IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list.new IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list; RETVAL=$$?;\
	if [ "$$RETVAL" -ne "0" ] || [ ! -f $@ ]; then \
		touch $@;\
	fi;

IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list:$(OBJECTS_IMU_DKM_partialImage) IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list.flag
	$(file >$@,$(OBJECTS_IMU_DKM_partialImage))

ifeq ($(TARGET_JOBS),1)
IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.o : IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list | check_objdirs
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";  $(TOOL_PATH)/ldarm64  -X -r -EL  --eh-frame-hdr   -o "$@" @IMU_DKM_partialImage/$(MODE_DIR)/.IMU_DKM_partialImageObject.list $(ADDED_OBJECTS) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

else
IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.o : IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage.o_jobs

endif
IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage_compile_file : $(FILE) ;

_clean :: IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage_clean

IMU_DKM_partialImage/$(MODE_DIR)/IMU_DKM_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "IMU_DKM_partialImage" ]; then cd "IMU_DKM_partialImage"; rm -rf $(MODE_DIR); fi

force : 

TARGET_JOBS_RULE?=echo "Update the makefile template via File > Import > Build Settings : Update makefile template";exit 1
%_jobs : 
	$(TRACE_FLAG)$(TARGET_JOBS_RULE)

DEP_FILES := IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cHandler.d IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cLauncher.d IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM/i2cMessageHandler.d
-include $(DEP_FILES)

OBJ_DIRS := IMU_DKM_partialImage/$(MODE_DIR)/Objects/IMU_DKM
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
