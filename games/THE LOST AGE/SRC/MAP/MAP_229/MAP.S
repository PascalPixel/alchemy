@ Field_Map229: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map229
Field_Map229:
	map_header 0x0, 0x80, 0x80, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map229, .LField_Map229_1
	map_block Field_Map229, .LField_Map229_2
	map_block Field_Map229, .LField_Map229_3
	map_block Field_Map229, .LField_Map229_4
	map_block Field_Map229
	map_block Field_Map229, .LField_Map229_6
	.incbin "MAP/MAP_229/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map229_1:
	.incbin "MAP/MAP_229/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map229_2:
	.incbin "MAP/MAP_229/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map229_3:
	.incbin "MAP/MAP_229/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map229_4:
	.incbin "MAP/MAP_229/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map229_6:
	.incbin "MAP/MAP_229/SLOT_6.bin.lz"
