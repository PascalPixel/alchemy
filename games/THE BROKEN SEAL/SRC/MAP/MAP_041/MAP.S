@ Field_Map041: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map041
Field_Map041:
	map_header 0x0, 0x40, 0x54, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map041, .LField_Map041_1
	map_block Field_Map041, .LField_Map041_2
	map_block Field_Map041, .LField_Map041_3
	map_block Field_Map041
	map_block Field_Map041, .LField_Map041_5
	.incbin "MAP/MAP_041/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map041_1:
	.incbin "MAP/MAP_041/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map041_2:
	.incbin "MAP/MAP_041/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map041_3:
	.incbin "MAP/MAP_041/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map041_5:
	.incbin "MAP/MAP_041/SLOT_5.bin"
