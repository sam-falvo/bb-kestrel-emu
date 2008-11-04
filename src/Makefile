UNITS		:= decoder		\
			   io			\
			   irqc			\
			   kimo			\
			   main			\
			   mgia			\
			   ram			\
			   rom			\
			   sdl-cache	\
			   serbus		\
			   timers

SRCS		:= $(UNITS:%=$D/%.c)
OBJS		:= $(UNITS:%=$D/%.o)

K			:= k2
K2BIN		:= $D/$K
CLEAN		:= $D/clean
INSTALL		:= $D/install


$(K2BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LD_LIBS)

ALL_BINS	+= $(K2BIN)

.PHONY: $(CLEAN)

$(CLEAN):
	rm -f $(OBJS) $(K2BIN)

ALL_CLEAN	+= $(CLEAN)

.PHONY: $(INSTALL)

$(INSTALL):
	mkdir -p $(BINPATH)
	cp $(K2BIN) $(BINPATH)

ALL_INSTALLS += $(INSTALL)
