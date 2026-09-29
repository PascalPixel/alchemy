@ Field_Map296: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map296
Field_Map296:
	map_header 0x0, 0x40, 0x40, 3, 0, 0, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x0
	map_layer 16, 16, 0, -1, 0x0
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map296, .LField_Map296_1
	map_block Field_Map296, .LField_Map296_2
	map_block Field_Map296, .LField_Map296_3
	map_block Field_Map296
	map_block Field_Map296
	map_block Field_Map296, .LField_Map296_6
	.incbin "MAP/MAP_296/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map296_1:
	.incbin "MAP/MAP_296/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map296_2:
	.incbin "MAP/MAP_296/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map296_3:
	.incbin "MAP/MAP_296/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map296_6:
	.incbin "MAP/MAP_296/SLOT_6.bin.lz"
