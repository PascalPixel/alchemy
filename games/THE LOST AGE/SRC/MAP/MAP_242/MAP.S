@ Field_Map242: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map242
Field_Map242:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map242, .LField_Map242_1
	map_block Field_Map242, .LField_Map242_2
	map_block Field_Map242, .LField_Map242_3
	map_block Field_Map242, .LField_Map242_4
	map_block Field_Map242
	map_block Field_Map242, .LField_Map242_6
	.incbin "MAP/MAP_242/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map242_1:
	.incbin "MAP/MAP_242/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map242_2:
	.incbin "MAP/MAP_242/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map242_3:
	.incbin "MAP/MAP_242/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map242_4:
	.incbin "MAP/MAP_242/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map242_6:
	.incbin "MAP/MAP_242/SLOT_6.bin.lz"
