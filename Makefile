#
# This file should not be changed.	If you need to make alterations to the configuration, you really ought to edit config.mk instead.
#

all: configure bins

include config.mk
include nrmf.mk

ALL_BINS			:=
ALL_CLEAN			:=
ALL_CONFIG			:=
ALL_INSTALLS		:=
ALL_LIBS			:=

LD_LIBS				:=

$(call subdir,config)
$(call subdir,src)

.c.o:
	$(CC) $(CCFLAGS) -o $@ $<

configure: $(ALL_CONFIG)

bins: $(ALL_BINS)

libs: $(ALL_LIBS)

clean: $(ALL_CLEAN)

install: $(ALL_INSTALLS)

