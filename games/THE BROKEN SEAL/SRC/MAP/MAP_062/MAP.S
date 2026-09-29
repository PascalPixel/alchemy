@ Field_Map062: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map062
Field_Map062:
	map_header 0x0, 0x48, 0x5e, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6000
	map_layer 16, 16, 0, -1, 0x50
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map062, .LField_Map062_1
	map_block Field_Map062, .LField_Map062_2
	map_block Field_Map062, .LField_Map062_3
	map_block Field_Map062, .LField_Map062_4
	map_block Field_Map062, .LField_Map062_5
	.incbin "MAP/MAP_062/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map062_1:
	.incbin "MAP/MAP_062/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map062_2:
	.incbin "MAP/MAP_062/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map062_3:
	.incbin "MAP/MAP_062/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map062_4:
	.incbin "MAP/MAP_062/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map062_5:
	.incbin "MAP/MAP_062/SLOT_5.bin"
