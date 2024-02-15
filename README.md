main.cpp - C++ program source code.
map.txt - map file, in text format.
prototype.py - Python program prototype.

my_first_text.txt & my_second_text.txt - Test of using text files in object linker.
test_ram_1.o & test_mem_2.o - Test of using binary files in object linker.

output.rom - Result of linking test_ram_1.o & test_mem_2.o using the following map file:
  EXENAME "output.rom"
  SEGMENT "test_ram_1.o" 0x0000-0x8000
  SEGMENT "test_ram_2.o" 0xA000-0xFFFF

output.txt - Result of linking my_first_text.txt & my_second_text.txt using the following map file:
  EXENAME "output.txt"
  SEGMENT "my_first_text.txt" 0x0000-0x1000
  SEGMENT "my_second_text.txt" 0x2000-0x3000
