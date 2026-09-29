@ Field_Map096: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map096
Field_Map096:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map096, .LField_Map096_1
	map_block Field_Map096, .LField_Map096_2
	map_block Field_Map096, .LField_Map096_3
	map_block Field_Map096, .LField_Map096_4
	map_block Field_Map096, .LField_Map096_5
	map_block Field_Map096, .LField_Map096_6
	.incbin "MAP/MAP_096/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map096_1:
	.incbin "MAP/MAP_096/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map096_2:
	.incbin "MAP/MAP_096/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map096_3:
	.incbin "MAP/MAP_096/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map096_4:
	.incbin "MAP/MAP_096/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map096_5:
	.incbin "MAP/MAP_096/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map096_6:
	.incbin "MAP/MAP_096/SLOT_6.bin.lz"
