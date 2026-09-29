@ Field_Map092: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map092
Field_Map092:
	map_header 0x0, 0xc0, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -129, 0x4000
	map_layer 16, 16, 0, -129, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map092, .LField_Map092_1
	map_block Field_Map092, .LField_Map092_2
	map_block Field_Map092, .LField_Map092_3
	map_block Field_Map092
	map_block Field_Map092, .LField_Map092_5
	.incbin "MAP/MAP_092/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map092_1:
	.incbin "MAP/MAP_092/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map092_2:
	.incbin "MAP/MAP_092/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map092_3:
	.incbin "MAP/MAP_092/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map092_5:
	.incbin "MAP/MAP_092/SLOT_5.bin"
