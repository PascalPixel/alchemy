@ Field_Map133: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map133
Field_Map133:
	map_header 0x0, 0x40, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map133, .LField_Map133_1
	map_block Field_Map133, .LField_Map133_2
	map_block Field_Map133, .LField_Map133_3
	map_block Field_Map133, .LField_Map133_4
	map_block Field_Map133, .LField_Map133_5
	map_block Field_Map133, .LField_Map133_6
	.incbin "MAP/MAP_133/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map133_1:
	.incbin "MAP/MAP_133/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map133_2:
	.incbin "MAP/MAP_133/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map133_3:
	.incbin "MAP/MAP_133/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map133_4:
	.incbin "MAP/MAP_133/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map133_5:
	.incbin "MAP/MAP_133/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map133_6:
	.incbin "MAP/MAP_133/SLOT_6.bin.lz"
