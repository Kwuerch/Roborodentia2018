SHARED = /home/kyle/shared
MPART = uc3c0512c
MAIN = krombopulos.elf

BUILDDIR = build

CC = avr32-gcc
CFLAGS = -Wall -mpart=$(MPART) $(INCLUDES)

PERIPHERALS = peripherals
CONFIG = config
VL53L0X = drivers/VL53L0X
DRIVERS = drivers
ASF = asf
BUILD = build

INCLUDES = -I$(PERIPHERALS) \
			  -I$(VL53L0X)/core/inc \
			  -I$(VL53L0X)/platform/inc \
			  -I$(DRIVERS) \
			  -I$(CONFIG)

SOURCES = main.c

SOURCES += $(PERIPHERALS)/flash.c \
			$(PERIPHERALS)/pm.c \
			$(PERIPHERALS)/scif.c \
			$(PERIPHERALS)/twi.c \
			$(PERIPHERALS)/usart.c \
			$(PERIPHERALS)/spi_master.c \
			$(PERIPHERALS)/tc.c \
			$(PERIPHERALS)/pwm.c \
			$(PERIPHERALS)/gpio.c \
			$(PERIPHERALS)/spi.c


SOURCES += $(CONFIG)/board.c

SOURCES += $(DRIVERS)/console.c \
			$(DRIVERS)/drv8711.c \
			$(DRIVERS)/delay.c \
			$(DRIVERS)/motors.c \
			$(DRIVERS)/led.c \
			$(DRIVERS)/btn.c \
			$(DRIVERS)/shootControl.c \
			$(DRIVERS)/brushless.c

SOURCES += $(VL53L0X)/core/src/vl53l0x_api.c \
			$(VL53L0X)/core/src/vl53l0x_api_calibration.c \
			$(VL53L0X)/core/src/vl53l0x_api_core.c \
			$(VL53L0X)/core/src/vl53l0x_api_ranging.c \
			$(VL53L0X)/core/src/vl53l0x_api_strings.c

SOURCES += $(VL53L0X)/platform/src/vl53l0x_platform.c \
			$(VL53L0X)/platform/src/vl53l0x.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

all: $(MAIN) 
	@echo Compiled

$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJECTS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

flash: all
	cp $(MAIN) $(SHARED)

debug:
	sudo screen /dev/ttyUSB0 41118

clean:
	rm -r $(BUILD)/*
