@ Field_Map076: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map076
Field_Map076:
	map_header 0x0, 0x78, 0x78, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map076, .LField_Map076_1
	map_block Field_Map076, .LField_Map076_2
	map_block Field_Map076, .LField_Map076_3
	map_block Field_Map076, .LField_Map076_4
	map_block Field_Map076, .LField_Map076_5
	.incbin "MAP/MAP_076/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map076_1:
	.incbin "MAP/MAP_076/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map076_2:
	.incbin "MAP/MAP_076/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map076_3:
	.incbin "MAP/MAP_076/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map076_4:
	.incbin "MAP/MAP_076/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map076_5:
	.incbin "MAP/MAP_076/SLOT_5.bin"
