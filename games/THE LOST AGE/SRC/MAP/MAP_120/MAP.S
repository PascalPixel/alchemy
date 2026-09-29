@ Field_Map120: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map120
Field_Map120:
	map_header 0x0, 0x6e, 0x64, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6e00
	map_layer 16, 16, 0, -1, 0x6e
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map120, .LField_Map120_1
	map_block Field_Map120, .LField_Map120_2
	map_block Field_Map120, .LField_Map120_3
	map_block Field_Map120, .LField_Map120_4
	map_block Field_Map120, .LField_Map120_5
	map_block Field_Map120, .LField_Map120_6
	.incbin "MAP/MAP_120/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map120_1:
	.incbin "MAP/MAP_120/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map120_2:
	.incbin "MAP/MAP_120/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map120_3:
	.incbin "MAP/MAP_120/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map120_4:
	.incbin "MAP/MAP_120/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map120_5:
	.incbin "MAP/MAP_120/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map120_6:
	.incbin "MAP/MAP_120/SLOT_6.bin.lz"
