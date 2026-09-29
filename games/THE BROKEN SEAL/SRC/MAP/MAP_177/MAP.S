@ Field_Map177: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map177
Field_Map177:
	map_header 0x0, 0x40, 0x40, 3, 0, 0, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x0
	map_layer 16, 16, 0, -1, 0x0
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map177, .LField_Map177_1
	map_block Field_Map177, .LField_Map177_2
	map_block Field_Map177
	map_block Field_Map177
	map_block Field_Map177, .LField_Map177_5
	.incbin "MAP/MAP_177/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map177_1:
	.incbin "MAP/MAP_177/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map177_2:
	.incbin "MAP/MAP_177/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map177_5:
	.incbin "MAP/MAP_177/SLOT_5.bin"
