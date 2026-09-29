@ Field_Map053: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map053
Field_Map053:
	map_header 0x0, 0x3c, 0x3c, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x3c00
	map_layer 16, 16, 0, -1, 0x46
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map053, .LField_Map053_1
	map_block Field_Map053, .LField_Map053_2
	map_block Field_Map053, .LField_Map053_3
	map_block Field_Map053, .LField_Map053_4
	map_block Field_Map053, .LField_Map053_5
	.incbin "MAP/MAP_053/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map053_1:
	.incbin "MAP/MAP_053/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map053_2:
	.incbin "MAP/MAP_053/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map053_3:
	.incbin "MAP/MAP_053/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map053_4:
	.incbin "MAP/MAP_053/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map053_5:
	.incbin "MAP/MAP_053/SLOT_5.bin"
