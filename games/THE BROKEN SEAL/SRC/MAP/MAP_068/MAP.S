@ Field_Map068: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map068
Field_Map068:
	map_header 0x0, 0x6a, 0x50, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6c
	map_layer 16, 16, 0, -1, 0x5a00
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map068, .LField_Map068_1
	map_block Field_Map068, .LField_Map068_2
	map_block Field_Map068, .LField_Map068_3
	map_block Field_Map068
	map_block Field_Map068, .LField_Map068_5
	.incbin "MAP/MAP_068/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map068_1:
	.incbin "MAP/MAP_068/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map068_2:
	.incbin "MAP/MAP_068/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map068_3:
	.incbin "MAP/MAP_068/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map068_5:
	.incbin "MAP/MAP_068/SLOT_5.bin"
