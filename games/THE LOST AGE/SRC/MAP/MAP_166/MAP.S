@ Field_Map166: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map166
Field_Map166:
	map_header 0x0, 0x80, 0x80, 2, 3, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0xa400
	map_layer 4, 4, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map166, .LField_Map166_1
	map_block Field_Map166, .LField_Map166_2
	map_block Field_Map166, .LField_Map166_3
	map_block Field_Map166, .LField_Map166_4
	map_block Field_Map166, .LField_Map166_5
	map_block Field_Map166, .LField_Map166_6
	.incbin "MAP/MAP_166/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map166_1:
	.incbin "MAP/MAP_166/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map166_2:
	.incbin "MAP/MAP_166/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map166_3:
	.incbin "MAP/MAP_166/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map166_4:
	.incbin "MAP/MAP_166/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map166_5:
	.incbin "MAP/MAP_166/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map166_6:
	.incbin "MAP/MAP_166/SLOT_6.bin.lz"
