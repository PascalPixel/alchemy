@ Field_Map106: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map106
Field_Map106:
	map_header 0x0, 0x3c, 0xbc, 3, 2, 1, 1, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x3c
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map106, .LField_Map106_1
	map_block Field_Map106, .LField_Map106_2
	map_block Field_Map106, .LField_Map106_3
	map_block Field_Map106
	map_block Field_Map106, .LField_Map106_5
	.incbin "MAP/MAP_106/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map106_1:
	.incbin "MAP/MAP_106/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map106_2:
	.incbin "MAP/MAP_106/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map106_3:
	.incbin "MAP/MAP_106/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map106_5:
	.incbin "MAP/MAP_106/SLOT_5.bin"
