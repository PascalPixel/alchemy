@ Field_Map287: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map287
Field_Map287:
	map_header 0x0, 0x80, 0x80, 3, 2, 0, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x0
	map_layer 100, 100, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map287, .LField_Map287_1
	map_block Field_Map287, .LField_Map287_2
	map_block Field_Map287, .LField_Map287_3
	map_block Field_Map287, .LField_Map287_4
	map_block Field_Map287, .LField_Map287_5
	map_block Field_Map287, .LField_Map287_6
	.incbin "MAP/MAP_287/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map287_1:
	.incbin "MAP/MAP_287/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map287_2:
	.incbin "MAP/MAP_287/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map287_3:
	.incbin "MAP/MAP_287/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map287_4:
	.incbin "MAP/MAP_287/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map287_5:
	.incbin "MAP/MAP_287/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map287_6:
	.incbin "MAP/MAP_287/SLOT_6.bin.lz"
