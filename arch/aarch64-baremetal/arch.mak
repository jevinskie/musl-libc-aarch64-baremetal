
BASE_SRCS := $(filter-out $(srcdir)/src/malloc/%.c $(srcdir)/src/malloc/$(MALLOC_DIR)/%.c,$(BASE_SRCS))
BASE_SRCS := $(filter-out $(srcdir)/src/env/__libc_start_main.c,$(BASE_SRCS))
BASE_SRCS := $(filter-out $(srcdir)/src/exit/exit.c,$(BASE_SRCS))

LDFLAGS_ALL += -Wl,--unresolved-symbols=ignore-all

# CFLAGS_ALL += -Os
# CFLAGS_ALL += -O0
CFLAGS_ALL += -ggdb3
CFLAGS_ALL += -fPIC
# CFLAGS_ALL += -flto=full
CFLAGS_ALL += -Oz

# nuke and build everything at our chosen opt level above
OPTIMIZE_GLOBS :=

STATIC_LIBS += lib/libc.o

LIBC_O_LD := $(CC)
NOSTDLIB_FLAG := -nostdlib
RELOCATABLE_FLAG := -Wl,-r
WHOLE_ARCHIVE_FLAG_START := -Wl,--whole-archive
WHOLE_ARCHIVE_FLAG_END := -Wl,--no-whole-archive
ifeq ($(BINFMT),macho)
	LIBC_O_LD := $(shell xcrun -f ld) -arch arm64
	NOSTDLIB_FLAG :=
	RELOCATABLE_FLAG := -r
	WHOLE_ARCHIVE_FLAG_START := -force_load
	WHOLE_ARCHIVE_FLAG_END :=
endif

lib/libc.o: lib/libc.a
	$(LIBC_O_LD) $(NOSTDLIB_FLAG) $(RELOCATABLE_FLAG) -o $@ $(WHOLE_ARCHIVE_FLAG_START) lib/libc.a $(WHOLE_ARCHIVE_FLAG_END) $(LIBCC)

print-%:
	@echo $* = $($*)
