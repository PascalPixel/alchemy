@ Field_Map071: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map071
Field_Map071:
	map_header 0x0, 0x6c, 0x5a, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6c
	map_layer 16, 16, 0, -1, 0x5a00
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map071, .LField_Map071_1
	map_block Field_Map071, .LField_Map071_2
	map_block Field_Map071
	map_block Field_Map071
	map_block Field_Map071, .LField_Map071_5
	.incbin "MAP/MAP_071/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map071_1:
	.incbin "MAP/MAP_071/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map071_2:
	.incbin "MAP/MAP_071/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map071_5:
	.incbin "MAP/MAP_071/SLOT_5.bin"
