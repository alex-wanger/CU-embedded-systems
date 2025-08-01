CC      = avr-gcc
CFLAGS += \
    -DF_CPU=16000000ULL \
    -O0 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -g \
    -mmcu=atmega328p \
    -std=gnu17 \

.PHONY: all
all: digital.elf

.PHONY: clean
clean:
	@rm -rvf *.d *.elf *.o

digital.elf: digital.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MM -MF $*.d $<
	$(CC) $(CFLAGS) -c $<

-include $(wildcard *.d)

