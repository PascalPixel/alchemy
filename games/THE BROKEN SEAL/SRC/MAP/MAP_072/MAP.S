@ Field_Map072: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map072
Field_Map072:
	map_header 0x40a, 0x43, 0x43, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map072, .LField_Map072_1
	map_block Field_Map072, .LField_Map072_2
	map_block Field_Map072, .LField_Map072_3
	map_block Field_Map072, .LField_Map072_4
	map_block Field_Map072, .LField_Map072_5
	.incbin "MAP/MAP_072/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map072_1:
	.incbin "MAP/MAP_072/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map072_2:
	.incbin "MAP/MAP_072/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map072_3:
	.incbin "MAP/MAP_072/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map072_4:
	.incbin "MAP/MAP_072/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map072_5:
	.incbin "MAP/MAP_072/SLOT_5.bin"
