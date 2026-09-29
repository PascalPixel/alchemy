@ Field_Map019: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map019
Field_Map019:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map019, .LField_Map019_1
	map_block Field_Map019, .LField_Map019_2
	map_block Field_Map019, .LField_Map019_3
	map_block Field_Map019, .LField_Map019_4
	map_block Field_Map019, .LField_Map019_5
	.incbin "MAP/MAP_019/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map019_1:
	.incbin "MAP/MAP_019/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map019_2:
	.incbin "MAP/MAP_019/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map019_3:
	.incbin "MAP/MAP_019/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map019_4:
	.incbin "MAP/MAP_019/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map019_5:
	.incbin "MAP/MAP_019/SLOT_5.bin"
