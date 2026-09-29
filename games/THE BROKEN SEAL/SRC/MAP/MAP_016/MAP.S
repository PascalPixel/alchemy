@ Field_Map016: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map016
Field_Map016:
	map_header 0x0, 0xff, 0x51, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x5100
	map_layer 16, 16, 0, -1, 0xa100
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map016, .LField_Map016_1
	map_block Field_Map016, .LField_Map016_2
	map_block Field_Map016, .LField_Map016_3
	map_block Field_Map016, .LField_Map016_4
	map_block Field_Map016, .LField_Map016_5
	.incbin "MAP/MAP_016/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map016_1:
	.incbin "MAP/MAP_016/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map016_2:
	.incbin "MAP/MAP_016/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map016_3:
	.incbin "MAP/MAP_016/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map016_4:
	.incbin "MAP/MAP_016/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map016_5:
	.incbin "MAP/MAP_016/SLOT_5.bin"
