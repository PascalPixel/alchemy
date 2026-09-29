@ Field_Map112: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map112
Field_Map112:
	map_header 0x0, 0x64, 0x50, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map112, .LField_Map112_1
	map_block Field_Map112, .LField_Map112_2
	map_block Field_Map112, .LField_Map112_3
	map_block Field_Map112, .LField_Map112_4
	map_block Field_Map112, .LField_Map112_5
	map_block Field_Map112, .LField_Map112_6
	.incbin "MAP/MAP_112/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map112_1:
	.incbin "MAP/MAP_112/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map112_2:
	.incbin "MAP/MAP_112/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map112_3:
	.incbin "MAP/MAP_112/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map112_4:
	.incbin "MAP/MAP_112/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map112_5:
	.incbin "MAP/MAP_112/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map112_6:
	.incbin "MAP/MAP_112/SLOT_6.bin.lz"
