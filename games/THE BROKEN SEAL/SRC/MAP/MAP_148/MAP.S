@ Field_Map148: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map148
Field_Map148:
	map_header 0x0, 0x40, 0xc0, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -129, 0x40
	map_layer 16, 16, 0, -129, 0xc0
	map_layer 20, 20, 9095, 7967, 0x3c
	.2byte 0x0
	map_block Field_Map148, .LField_Map148_1
	map_block Field_Map148, .LField_Map148_2
	map_block Field_Map148
	map_block Field_Map148
	map_block Field_Map148, .LField_Map148_5
	.incbin "MAP/MAP_148/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map148_1:
	.incbin "MAP/MAP_148/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map148_2:
	.incbin "MAP/MAP_148/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map148_5:
	.incbin "MAP/MAP_148/SLOT_5.bin"
