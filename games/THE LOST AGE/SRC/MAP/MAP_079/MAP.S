@ Field_Map079: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map079
Field_Map079:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map079, .LField_Map079_1
	map_block Field_Map079, .LField_Map079_2
	map_block Field_Map079, .LField_Map079_3
	map_block Field_Map079, .LField_Map079_4
	map_block Field_Map079, .LField_Map079_5
	map_block Field_Map079, .LField_Map079_6
	.incbin "MAP/MAP_079/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map079_1:
	.incbin "MAP/MAP_079/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map079_2:
	.incbin "MAP/MAP_079/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map079_3:
	.incbin "MAP/MAP_079/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map079_4:
	.incbin "MAP/MAP_079/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map079_5:
	.incbin "MAP/MAP_079/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map079_6:
	.incbin "MAP/MAP_079/SLOT_6.bin.lz"
