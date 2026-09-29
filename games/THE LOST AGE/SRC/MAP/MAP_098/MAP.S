@ Field_Map098: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map098
Field_Map098:
	map_header 0x0, 0x40, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map098, .LField_Map098_1
	map_block Field_Map098, .LField_Map098_2
	map_block Field_Map098, .LField_Map098_3
	map_block Field_Map098, .LField_Map098_4
	map_block Field_Map098, .LField_Map098_5
	map_block Field_Map098, .LField_Map098_6
	.incbin "MAP/MAP_098/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map098_1:
	.incbin "MAP/MAP_098/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map098_2:
	.incbin "MAP/MAP_098/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map098_3:
	.incbin "MAP/MAP_098/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map098_4:
	.incbin "MAP/MAP_098/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map098_5:
	.incbin "MAP/MAP_098/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map098_6:
	.incbin "MAP/MAP_098/SLOT_6.bin.lz"
