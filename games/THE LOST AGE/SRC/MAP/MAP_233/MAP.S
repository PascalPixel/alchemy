@ Field_Map233: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map233
Field_Map233:
	map_header 0x0, 0x54, 0xf0, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x54
	map_layer 16, 16, 0, -1, 0xa8
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map233, .LField_Map233_1
	map_block Field_Map233, .LField_Map233_2
	map_block Field_Map233, .LField_Map233_3
	map_block Field_Map233, .LField_Map233_4
	map_block Field_Map233, .LField_Map233_5
	map_block Field_Map233, .LField_Map233_6
	.incbin "MAP/MAP_233/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map233_1:
	.incbin "MAP/MAP_233/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map233_2:
	.incbin "MAP/MAP_233/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map233_3:
	.incbin "MAP/MAP_233/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map233_4:
	.incbin "MAP/MAP_233/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map233_5:
	.incbin "MAP/MAP_233/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map233_6:
	.incbin "MAP/MAP_233/SLOT_6.bin.lz"
