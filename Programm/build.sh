#!/bin/bash

avr-gcc -g -mmcu=attiny25 -c main.c -Wall -Os
avr-gcc -g -mmcu=attiny25 -o ladestandsanzeige.elf main.o
avr-objcopy -O ihex ladestandsanzeige.elf ladestandsanzeige.hex
avr-size ladestandsanzeige.elf
