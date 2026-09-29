@ Field_Map018: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map018
Field_Map018:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map018, .LField_Map018_1
	map_block Field_Map018, .LField_Map018_2
	map_block Field_Map018, .LField_Map018_3
	map_block Field_Map018, .LField_Map018_4
	map_block Field_Map018, .LField_Map018_5
	.incbin "MAP/MAP_018/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map018_1:
	.incbin "MAP/MAP_018/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map018_2:
	.incbin "MAP/MAP_018/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map018_3:
	.incbin "MAP/MAP_018/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map018_4:
	.incbin "MAP/MAP_018/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map018_5:
	.incbin "MAP/MAP_018/SLOT_5.bin"
