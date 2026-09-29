@ Field_Map039: a field map of 2 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map039
Field_Map039:
	map_header 0x0, 0x40, 0x40, 2, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map039, .LField_Map039_1
	map_block Field_Map039, .LField_Map039_2
	map_block Field_Map039, .LField_Map039_3
	map_block Field_Map039, .LField_Map039_4
	map_block Field_Map039, .LField_Map039_5
	.incbin "MAP/MAP_039/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map039_1:
	.incbin "MAP/MAP_039/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map039_2:
	.incbin "MAP/MAP_039/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map039_3:
	.incbin "MAP/MAP_039/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map039_4:
	.incbin "MAP/MAP_039/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map039_5:
	.incbin "MAP/MAP_039/SLOT_5.bin"
