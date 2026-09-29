@ Field_Map102: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map102
Field_Map102:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map102, .LField_Map102_1
	map_block Field_Map102, .LField_Map102_2
	map_block Field_Map102, .LField_Map102_3
	map_block Field_Map102, .LField_Map102_4
	map_block Field_Map102, .LField_Map102_5
	.incbin "MAP/MAP_102/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map102_1:
	.incbin "MAP/MAP_102/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map102_2:
	.incbin "MAP/MAP_102/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map102_3:
	.incbin "MAP/MAP_102/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map102_4:
	.incbin "MAP/MAP_102/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map102_5:
	.incbin "MAP/MAP_102/SLOT_5.bin"
