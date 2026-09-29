@ Field_Map165: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map165
Field_Map165:
	map_header 0x0, 0x80, 0x80, 2, 3, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0xa400
	map_layer 4, 4, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map165, .LField_Map165_1
	map_block Field_Map165, .LField_Map165_2
	map_block Field_Map165, .LField_Map165_3
	map_block Field_Map165, .LField_Map165_4
	map_block Field_Map165, .LField_Map165_5
	map_block Field_Map165, .LField_Map165_6
	.incbin "MAP/MAP_165/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map165_1:
	.incbin "MAP/MAP_165/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map165_2:
	.incbin "MAP/MAP_165/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map165_3:
	.incbin "MAP/MAP_165/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map165_4:
	.incbin "MAP/MAP_165/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map165_5:
	.incbin "MAP/MAP_165/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map165_6:
	.incbin "MAP/MAP_165/SLOT_6.bin.lz"
