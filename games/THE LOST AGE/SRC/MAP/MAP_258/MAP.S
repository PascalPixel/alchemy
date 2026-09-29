@ Field_Map258: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map258
Field_Map258:
	map_header 0x0, 0x58, 0x68, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map258, .LField_Map258_1
	map_block Field_Map258, .LField_Map258_2
	map_block Field_Map258, .LField_Map258_3
	map_block Field_Map258, .LField_Map258_4
	map_block Field_Map258
	map_block Field_Map258, .LField_Map258_6
	.incbin "MAP/MAP_258/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map258_1:
	.incbin "MAP/MAP_258/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map258_2:
	.incbin "MAP/MAP_258/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map258_3:
	.incbin "MAP/MAP_258/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map258_4:
	.incbin "MAP/MAP_258/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map258_6:
	.incbin "MAP/MAP_258/SLOT_6.bin.lz"
