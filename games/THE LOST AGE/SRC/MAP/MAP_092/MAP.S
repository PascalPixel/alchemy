@ Field_Map092: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map092
Field_Map092:
	map_header 0x0, 0x5a, 0x5a, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x5a
	map_layer 16, 16, 0, -1, 0x5a00
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map092, .LField_Map092_1
	map_block Field_Map092, .LField_Map092_2
	map_block Field_Map092, .LField_Map092_3
	map_block Field_Map092, .LField_Map092_4
	map_block Field_Map092, .LField_Map092_5
	map_block Field_Map092, .LField_Map092_6
	.incbin "MAP/MAP_092/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map092_1:
	.incbin "MAP/MAP_092/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map092_2:
	.incbin "MAP/MAP_092/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map092_3:
	.incbin "MAP/MAP_092/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map092_4:
	.incbin "MAP/MAP_092/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map092_5:
	.incbin "MAP/MAP_092/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map092_6:
	.incbin "MAP/MAP_092/SLOT_6.bin.lz"
