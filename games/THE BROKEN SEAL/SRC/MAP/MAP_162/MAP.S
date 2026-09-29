@ Field_Map162: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map162
Field_Map162:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map162, .LField_Map162_1
	map_block Field_Map162, .LField_Map162_2
	map_block Field_Map162, .LField_Map162_3
	map_block Field_Map162, .LField_Map162_4
	map_block Field_Map162, .LField_Map162_5
	.incbin "MAP/MAP_162/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map162_1:
	.incbin "MAP/MAP_162/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map162_2:
	.incbin "MAP/MAP_162/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map162_3:
	.incbin "MAP/MAP_162/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map162_4:
	.incbin "MAP/MAP_162/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map162_5:
	.incbin "MAP/MAP_162/SLOT_5.bin"
