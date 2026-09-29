@ Field_Map274: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map274
Field_Map274:
	map_header 0x0, 0xcc, 0x50, 3, 3, 2, 0, 2, 2, 0x0, 0x0
	map_layer 12, 12, -258, 3855, 0x5000
	map_layer 16, 16, 0, -1, 0xa000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map274, .LField_Map274_1
	map_block Field_Map274, .LField_Map274_2
	map_block Field_Map274, .LField_Map274_3
	map_block Field_Map274, .LField_Map274_4
	map_block Field_Map274
	map_block Field_Map274, .LField_Map274_6
	.incbin "MAP/MAP_274/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map274_1:
	.incbin "MAP/MAP_274/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map274_2:
	.incbin "MAP/MAP_274/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map274_3:
	.incbin "MAP/MAP_274/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map274_4:
	.incbin "MAP/MAP_274/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map274_6:
	.incbin "MAP/MAP_274/SLOT_6.bin.lz"
