@ Field_Map159: a field map of 2 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map159
Field_Map159:
	map_header 0x0, 0x40, 0x38, 2, 1, 3, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map159, .LField_Map159_1
	map_block Field_Map159, .LField_Map159_2
	map_block Field_Map159, .LField_Map159_3
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_5
	.incbin "MAP/MAP_159/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map159_1:
	.incbin "MAP/MAP_159/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map159_2:
	.incbin "MAP/MAP_159/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map159_3:
	.incbin "MAP/MAP_159/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map159_5:
	.incbin "MAP/MAP_159/SLOT_5.bin"
