@ Field_Map008: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map008
Field_Map008:
	map_header 0x0, 0x1e, 0x2d, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x1e
	map_layer 16, 16, 0, -1, 0x3200
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map008, .LField_Map008_1
	map_block Field_Map008, .LField_Map008_2
	map_block Field_Map008
	map_block Field_Map008
	map_block Field_Map008, .LField_Map008_5
	.incbin "MAP/MAP_008/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map008_1:
	.incbin "MAP/MAP_008/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map008_2:
	.incbin "MAP/MAP_008/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map008_5:
	.incbin "MAP/MAP_008/SLOT_5.bin"
