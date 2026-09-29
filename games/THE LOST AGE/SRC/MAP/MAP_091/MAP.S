@ Field_Map091: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map091
Field_Map091:
	map_header 0x0, 0x80, 0x80, 2, 1, 3, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map091, .LField_Map091_1
	map_block Field_Map091, .LField_Map091_2
	map_block Field_Map091, .LField_Map091_3
	map_block Field_Map091, .LField_Map091_4
	map_block Field_Map091, .LField_Map091_5
	map_block Field_Map091, .LField_Map091_6
	.incbin "MAP/MAP_091/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map091_1:
	.incbin "MAP/MAP_091/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map091_2:
	.incbin "MAP/MAP_091/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map091_3:
	.incbin "MAP/MAP_091/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map091_4:
	.incbin "MAP/MAP_091/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map091_5:
	.incbin "MAP/MAP_091/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map091_6:
	.incbin "MAP/MAP_091/SLOT_6.bin.lz"
