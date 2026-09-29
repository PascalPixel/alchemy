@ Field_Map011: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map011
Field_Map011:
	map_header 0x0, 0x40, 0x20, 3, 2, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x2000
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map011, .LField_Map011_1
	map_block Field_Map011, .LField_Map011_2
	map_block Field_Map011, .LField_Map011_3
	map_block Field_Map011, .LField_Map011_4
	map_block Field_Map011, .LField_Map011_5
	.incbin "MAP/MAP_011/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map011_1:
	.incbin "MAP/MAP_011/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map011_2:
	.incbin "MAP/MAP_011/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map011_3:
	.incbin "MAP/MAP_011/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map011_4:
	.incbin "MAP/MAP_011/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map011_5:
	.incbin "MAP/MAP_011/SLOT_5.bin"
