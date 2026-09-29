@ Field_Map017: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map017
Field_Map017:
	map_header 0x0, 0x40, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map017, .LField_Map017_1
	map_block Field_Map017, .LField_Map017_2
	map_block Field_Map017
	map_block Field_Map017
	map_block Field_Map017, .LField_Map017_5
	.incbin "MAP/MAP_017/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map017_1:
	.incbin "MAP/MAP_017/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map017_2:
	.incbin "MAP/MAP_017/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map017_5:
	.incbin "MAP/MAP_017/SLOT_5.bin"
