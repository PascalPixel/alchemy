@ Field_Map149: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map149
Field_Map149:
	map_header 0x0, 0x80, 0x80, 3, 1, 2, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map149, .LField_Map149_1
	map_block Field_Map149, .LField_Map149_2
	map_block Field_Map149, .LField_Map149_3
	map_block Field_Map149, .LField_Map149_4
	map_block Field_Map149, .LField_Map149_5
	.incbin "MAP/MAP_149/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map149_1:
	.incbin "MAP/MAP_149/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map149_2:
	.incbin "MAP/MAP_149/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map149_3:
	.incbin "MAP/MAP_149/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map149_4:
	.incbin "MAP/MAP_149/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map149_5:
	.incbin "MAP/MAP_149/SLOT_5.bin"
