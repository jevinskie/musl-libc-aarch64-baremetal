
BASE_SRCS := $(filter-out $(srcdir)/src/malloc/%.c $(srcdir)/src/malloc/$(MALLOC_DIR)/%.c,$(BASE_SRCS))
BASE_SRCS := $(filter-out $(srcdir)/src/env/__libc_start_main.c,$(BASE_SRCS))
BASE_SRCS := $(filter-out $(srcdir)/src/exit/exit.c,$(BASE_SRCS))

LDFLAGS_ALL += -Wl,--unresolved-symbols=ignore-all

# CFLAGS_ALL += -Os
# CFLAGS_ALL += -O0
CFLAGS_ALL += -g
CFLAGS_ALL += -fPIC
# CFLAGS_ALL += -flto=full

# nuke and build everything at our chosen opt level above
OPTIMIZE_GLOBS =

STATIC_LIBS += lib/libc.o

lib/libc.o: lib/libc.a
	$(CC) -nostdlib -Wl,-r -o $@ -Wl,--whole-archive lib/libc.a -Wl,--no-whole-archive $(LIBCC)

print-%:
	@echo $* = $($*)
