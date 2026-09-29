@ Field_Map010: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map010
Field_Map010:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map010, .LField_Map010_1
	map_block Field_Map010, .LField_Map010_2
	map_block Field_Map010, .LField_Map010_3
	map_block Field_Map010, .LField_Map010_4
	map_block Field_Map010, .LField_Map010_5
	map_block Field_Map010, .LField_Map010_6
	.incbin "MAP/MAP_010/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map010_1:
	.incbin "MAP/MAP_010/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map010_2:
	.incbin "MAP/MAP_010/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map010_3:
	.incbin "MAP/MAP_010/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map010_4:
	.incbin "MAP/MAP_010/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map010_5:
	.incbin "MAP/MAP_010/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map010_6:
	.incbin "MAP/MAP_010/SLOT_6.bin.lz"
