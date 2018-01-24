SHARED = /home/kyle/shared
MPART = uc3c0512c
MAIN = krombopulos.elf

BUILDDIR = build

CC = avr32-gcc
CFLAGS = -Wall -mpart=$(MPART)

PERIPHERALS = peripherals
INCLUDES = -I$(PERIPHERALS)
SOURCES += main.c \
			$(PERIPHERALS)/flash.c \
			$(PERIPHERALS)/pm.c \
			$(PERIPHERALS)/scif.c \
			$(PERIPHERALS)/twi.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

all: $(MAIN) 
	@echo Compiled

$(MAIN): $(OBJECTS)
	$(CC) $(INCLUDES) $(CFLAGS) -o $(MAIN) $(OBJECTS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

flash: all
	cp $(MAIN) $(SHARED)

clean:
	rm $(BUILD)/*
