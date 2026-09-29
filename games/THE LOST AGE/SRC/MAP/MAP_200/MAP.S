@ Field_Map200: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map200
Field_Map200:
	map_header 0x0, 0x80, 0x80, 2, 2, 3, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, 7967, 0x8000
	map_layer 12, 12, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map200, .LField_Map200_1
	map_block Field_Map200, .LField_Map200_2
	map_block Field_Map200, .LField_Map200_3
	map_block Field_Map200, .LField_Map200_4
	map_block Field_Map200
	map_block Field_Map200, .LField_Map200_6
	.incbin "MAP/MAP_200/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map200_1:
	.incbin "MAP/MAP_200/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map200_2:
	.incbin "MAP/MAP_200/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map200_3:
	.incbin "MAP/MAP_200/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map200_4:
	.incbin "MAP/MAP_200/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map200_6:
	.incbin "MAP/MAP_200/SLOT_6.bin.lz"
