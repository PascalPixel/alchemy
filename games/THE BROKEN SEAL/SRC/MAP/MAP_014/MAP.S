@ Field_Map014: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map014
Field_Map014:
	map_header 0x0, 0x70, 0x50, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x5000
	map_layer 16, 16, 0, -1, 0x70
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map014, .LField_Map014_1
	map_block Field_Map014, .LField_Map014_2
	map_block Field_Map014, .LField_Map014_3
	map_block Field_Map014, .LField_Map014_4
	map_block Field_Map014, .LField_Map014_5
	.incbin "MAP/MAP_014/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map014_1:
	.incbin "MAP/MAP_014/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map014_2:
	.incbin "MAP/MAP_014/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map014_3:
	.incbin "MAP/MAP_014/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map014_4:
	.incbin "MAP/MAP_014/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map014_5:
	.incbin "MAP/MAP_014/SLOT_5.bin"
