@ Field_Map205: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map205
Field_Map205:
	map_header 0x0, 0x34, 0x36, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x34
	map_layer 16, 16, 0, -1, 0x3600
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map205, .LField_Map205_1
	map_block Field_Map205, .LField_Map205_2
	map_block Field_Map205, .LField_Map205_3
	map_block Field_Map205, .LField_Map205_4
	map_block Field_Map205, .LField_Map205_5
	map_block Field_Map205, .LField_Map205_6
	.incbin "MAP/MAP_205/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map205_1:
	.incbin "MAP/MAP_205/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map205_2:
	.incbin "MAP/MAP_205/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map205_3:
	.incbin "MAP/MAP_205/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map205_4:
	.incbin "MAP/MAP_205/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map205_5:
	.incbin "MAP/MAP_205/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map205_6:
	.incbin "MAP/MAP_205/SLOT_6.bin.lz"
