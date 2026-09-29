@ Field_Map030: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map030
Field_Map030:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map030, .LField_Map030_1
	map_block Field_Map030, .LField_Map030_2
	map_block Field_Map030, .LField_Map030_3
	map_block Field_Map030
	map_block Field_Map030, .LField_Map030_5
	.incbin "MAP/MAP_030/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map030_1:
	.incbin "MAP/MAP_030/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map030_2:
	.incbin "MAP/MAP_030/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map030_3:
	.incbin "MAP/MAP_030/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map030_5:
	.incbin "MAP/MAP_030/SLOT_5.bin"
