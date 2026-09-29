@ Field_Map147: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map147
Field_Map147:
	map_header 0x0, 0x40, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -129, 0x40
	map_layer 16, 16, 0, -129, 0x80
	map_layer 20, 20, 9095, 7967, 0x3c
	.2byte 0x0
	map_block Field_Map147, .LField_Map147_1
	map_block Field_Map147, .LField_Map147_2
	map_block Field_Map147, .LField_Map147_3
	map_block Field_Map147
	map_block Field_Map147, .LField_Map147_5
	.incbin "MAP/MAP_147/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map147_1:
	.incbin "MAP/MAP_147/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map147_2:
	.incbin "MAP/MAP_147/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map147_3:
	.incbin "MAP/MAP_147/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map147_5:
	.incbin "MAP/MAP_147/SLOT_5.bin"
