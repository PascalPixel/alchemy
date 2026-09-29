@ Field_Map031: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map031
Field_Map031:
	map_header 0x0, 0x80, 0x80, 2, 2, 3, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 12, 12, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map031, .LField_Map031_1
	map_block Field_Map031, .LField_Map031_2
	map_block Field_Map031, .LField_Map031_3
	map_block Field_Map031, .LField_Map031_4
	map_block Field_Map031
	map_block Field_Map031, .LField_Map031_6
	.incbin "MAP/MAP_031/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map031_1:
	.incbin "MAP/MAP_031/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map031_2:
	.incbin "MAP/MAP_031/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map031_3:
	.incbin "MAP/MAP_031/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map031_4:
	.incbin "MAP/MAP_031/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map031_6:
	.incbin "MAP/MAP_031/SLOT_6.bin.lz"
