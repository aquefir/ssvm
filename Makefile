##############################################################################
##                       SundaScript Virtual Machine™                       ##
##                                                                          ##
##                         Copyright © 2020 Aquefir                         ##
##                       Released under BSD-2-Clause.                       ##
##############################################################################

include $(AQ)/lib/slick/base.mk

PROJECT := ssvm

# put a ‘1’ for the desired target types to compile
EXEFILE :=
SOFILE  := 1
AFILE   := 1

# space-separated path list for #includes
# <system> includes
INCLUDES := include
# "local" includes
INCLUDEL := src

# space-separated library name list
LIBS    :=
LIBDIRS :=

# frameworks (macOS target only)
FWORKS :=

# ‘3P’ are in-tree 3rd-party dependencies
# 3PLIBDIR is the base directory
# 3PLIBS is the folder names in the base directory for each library
3PLIBDIR := 3rdparty
3PLIBS   :=

# sources
CFILES   :=
HFILES   :=
CPPFILES :=
HPPFILES :=

# test suite sources
TES_CFILES   :=
TES_HFILES   :=
TES_CPPFILES :=
TES_HPPFILES :=

# force C++ linker because of 3rd-party library
CCLD := $(CXX)

# this defines all our usual targets
include $(AQ)/lib/slick/targets.mk