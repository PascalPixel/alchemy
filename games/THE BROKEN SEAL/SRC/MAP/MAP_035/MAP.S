@ Field_Map035: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map035
Field_Map035:
	map_header 0x0, 0x40, 0x30, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x3000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map035, .LField_Map035_1
	map_block Field_Map035, .LField_Map035_2
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_5
	.incbin "MAP/MAP_035/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map035_1:
	.incbin "MAP/MAP_035/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map035_2:
	.incbin "MAP/MAP_035/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map035_5:
	.incbin "MAP/MAP_035/SLOT_5.bin"
