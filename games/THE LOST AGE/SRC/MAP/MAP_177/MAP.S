@ Field_Map177: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map177
Field_Map177:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map177, .LField_Map177_1
	map_block Field_Map177, .LField_Map177_2
	map_block Field_Map177, .LField_Map177_3
	map_block Field_Map177, .LField_Map177_4
	map_block Field_Map177, .LField_Map177_5
	map_block Field_Map177, .LField_Map177_6
	.incbin "MAP/MAP_177/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map177_1:
	.incbin "MAP/MAP_177/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map177_2:
	.incbin "MAP/MAP_177/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map177_3:
	.incbin "MAP/MAP_177/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map177_4:
	.incbin "MAP/MAP_177/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map177_5:
	.incbin "MAP/MAP_177/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map177_6:
	.incbin "MAP/MAP_177/SLOT_6.bin.lz"
