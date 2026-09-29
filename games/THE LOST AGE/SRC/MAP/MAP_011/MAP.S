@ Field_Map011: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map011
Field_Map011:
	map_header 0x0, 0x5a, 0x64, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6400
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map011, .LField_Map011_1
	map_block Field_Map011, .LField_Map011_2
	map_block Field_Map011, .LField_Map011_3
	map_block Field_Map011, .LField_Map011_4
	map_block Field_Map011, .LField_Map011_5
	map_block Field_Map011, .LField_Map011_6
	.incbin "MAP/MAP_011/CELLS.delta2.lz"
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
	.incbin "MAP/MAP_011/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map011_6:
	.incbin "MAP/MAP_011/SLOT_6.bin.lz"
