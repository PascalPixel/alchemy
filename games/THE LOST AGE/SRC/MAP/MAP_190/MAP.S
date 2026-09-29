@ Field_Map190: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map190
Field_Map190:
	map_header 0x0, 0x4c, 0x80, 3, 1, 3, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map190, .LField_Map190_1
	map_block Field_Map190, .LField_Map190_2
	map_block Field_Map190, .LField_Map190_3
	map_block Field_Map190, .LField_Map190_4
	map_block Field_Map190, .LField_Map190_5
	map_block Field_Map190, .LField_Map190_6
	.incbin "MAP/MAP_190/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map190_1:
	.incbin "MAP/MAP_190/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map190_2:
	.incbin "MAP/MAP_190/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map190_3:
	.incbin "MAP/MAP_190/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map190_4:
	.incbin "MAP/MAP_190/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map190_5:
	.incbin "MAP/MAP_190/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map190_6:
	.incbin "MAP/MAP_190/SLOT_6.bin.lz"
