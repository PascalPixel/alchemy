@ Field_Map226: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map226
Field_Map226:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map226, .LField_Map226_1
	map_block Field_Map226, .LField_Map226_2
	map_block Field_Map226, .LField_Map226_3
	map_block Field_Map226, .LField_Map226_4
	map_block Field_Map226, .LField_Map226_5
	map_block Field_Map226, .LField_Map226_6
	.incbin "MAP/MAP_226/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map226_1:
	.incbin "MAP/MAP_226/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map226_2:
	.incbin "MAP/MAP_226/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map226_3:
	.incbin "MAP/MAP_226/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map226_4:
	.incbin "MAP/MAP_226/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map226_5:
	.incbin "MAP/MAP_226/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map226_6:
	.incbin "MAP/MAP_226/SLOT_6.bin.lz"
