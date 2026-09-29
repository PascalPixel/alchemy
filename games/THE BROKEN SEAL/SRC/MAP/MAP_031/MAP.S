@ Field_Map031: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map031
Field_Map031:
	map_header 0x0, 0x38, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map031, .LField_Map031_1
	map_block Field_Map031, .LField_Map031_2
	map_block Field_Map031, .LField_Map031_3
	map_block Field_Map031, .LField_Map031_4
	map_block Field_Map031, .LField_Map031_5
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
.LField_Map031_5:
	.incbin "MAP/MAP_031/SLOT_5.bin"
