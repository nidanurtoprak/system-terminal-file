CC = gcc
CFLAGS = -Wall -I./include
SRCDIR = src
BINDIR = bin
OBJDIR = obj
LOGFILE = file_manager.log

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXECUTABLE = $(BINDIR)/file_manager.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@if not exist $(BINDIR) mkdir $(BINDIR)
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo Cleaning up...
	@if exist $(BINDIR) rmdir /s /q $(BINDIR)
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(LOGFILE) del $(LOGFILE)
	@echo Clean complete.

run: $(EXECUTABLE)
	@echo Running the file manager...
	$(EXECUTABLE)

.PHONY: all clean run