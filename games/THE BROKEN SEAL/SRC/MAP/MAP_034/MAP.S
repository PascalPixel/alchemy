@ Field_Map034: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map034
Field_Map034:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x3c
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map034, .LField_Map034_1
	map_block Field_Map034, .LField_Map034_2
	map_block Field_Map034, .LField_Map034_3
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_5
	.incbin "MAP/MAP_034/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map034_1:
	.incbin "MAP/MAP_034/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map034_2:
	.incbin "MAP/MAP_034/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map034_3:
	.incbin "MAP/MAP_034/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map034_5:
	.incbin "MAP/MAP_034/SLOT_5.bin"
