define colorecho
	@tput setaf 6
	@echo $1
	@tput sgr0
endef

PROG   = Brick
CC     = clang
LINKER = clang

INCDIR = include
SRCDIR = src
OBJDIR = obj
BINDIR = bin

CFLAGS = -I$(INCDIR)
LFLAGS = -lSDL2-2.0.0 -lSDL2_image-2.0.0 -lSDL2_ttf-2.0.0

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(PROG): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	$(call colorecho,"Linking complete!")

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call colorecho,"Compiled "$<" successfully!")

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	$(call colorecho,"Cleanup complete!")

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(PROG)
	$(call colorecho,"Executable removed!")
