@ Field_Map126: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map126
Field_Map126:
	map_header 0x0, 0x32, 0x80, 3, 1, 1, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, -513, 16191, 0x40
	.2byte 0x0
	map_block Field_Map126, .LField_Map126_1
	map_block Field_Map126, .LField_Map126_2
	map_block Field_Map126, .LField_Map126_3
	map_block Field_Map126, .LField_Map126_4
	map_block Field_Map126, .LField_Map126_5
	map_block Field_Map126, .LField_Map126_6
	.incbin "MAP/MAP_126/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map126_1:
	.incbin "MAP/MAP_126/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map126_2:
	.incbin "MAP/MAP_126/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map126_3:
	.incbin "MAP/MAP_126/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map126_4:
	.incbin "MAP/MAP_126/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map126_5:
	.incbin "MAP/MAP_126/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map126_6:
	.incbin "MAP/MAP_126/SLOT_6.bin.lz"
