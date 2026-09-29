@ Field_Map137: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map137
Field_Map137:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map137, .LField_Map137_1
	map_block Field_Map137, .LField_Map137_2
	map_block Field_Map137, .LField_Map137_3
	map_block Field_Map137, .LField_Map137_4
	map_block Field_Map137, .LField_Map137_5
	map_block Field_Map137, .LField_Map137_6
	.incbin "MAP/MAP_137/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map137_1:
	.incbin "MAP/MAP_137/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map137_2:
	.incbin "MAP/MAP_137/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map137_3:
	.incbin "MAP/MAP_137/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map137_4:
	.incbin "MAP/MAP_137/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map137_5:
	.incbin "MAP/MAP_137/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map137_6:
	.incbin "MAP/MAP_137/SLOT_6.bin.lz"
