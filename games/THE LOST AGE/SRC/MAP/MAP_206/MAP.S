@ Field_Map206: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map206
Field_Map206:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map206, .LField_Map206_1
	map_block Field_Map206, .LField_Map206_2
	map_block Field_Map206, .LField_Map206_3
	map_block Field_Map206, .LField_Map206_4
	map_block Field_Map206, .LField_Map206_5
	map_block Field_Map206, .LField_Map206_6
	.incbin "MAP/MAP_206/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map206_1:
	.incbin "MAP/MAP_206/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map206_2:
	.incbin "MAP/MAP_206/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map206_3:
	.incbin "MAP/MAP_206/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map206_4:
	.incbin "MAP/MAP_206/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map206_5:
	.incbin "MAP/MAP_206/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map206_6:
	.incbin "MAP/MAP_206/SLOT_6.bin.lz"
