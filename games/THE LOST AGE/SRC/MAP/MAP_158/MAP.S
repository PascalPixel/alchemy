@ Field_Map158: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map158
Field_Map158:
	map_header 0x0, 0x82, 0x70, 3, 2, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map158, .LField_Map158_1
	map_block Field_Map158, .LField_Map158_2
	map_block Field_Map158, .LField_Map158_3
	map_block Field_Map158, .LField_Map158_4
	map_block Field_Map158, .LField_Map158_5
	map_block Field_Map158, .LField_Map158_6
	.incbin "MAP/MAP_158/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map158_1:
	.incbin "MAP/MAP_158/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map158_2:
	.incbin "MAP/MAP_158/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map158_3:
	.incbin "MAP/MAP_158/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map158_4:
	.incbin "MAP/MAP_158/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map158_5:
	.incbin "MAP/MAP_158/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map158_6:
	.incbin "MAP/MAP_158/SLOT_6.bin.lz"
