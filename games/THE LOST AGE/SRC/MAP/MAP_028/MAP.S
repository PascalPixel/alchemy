@ Field_Map028: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map028
Field_Map028:
	map_header 0x0, 0x5a, 0x64, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6400
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map028, .LField_Map028_1
	map_block Field_Map028, .LField_Map028_2
	map_block Field_Map028, .LField_Map028_3
	map_block Field_Map028, .LField_Map028_4
	map_block Field_Map028, .LField_Map028_5
	map_block Field_Map028, .LField_Map028_6
	.incbin "MAP/MAP_028/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map028_1:
	.incbin "MAP/MAP_028/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map028_2:
	.incbin "MAP/MAP_028/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map028_3:
	.incbin "MAP/MAP_028/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map028_4:
	.incbin "MAP/MAP_028/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map028_5:
	.incbin "MAP/MAP_028/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map028_6:
	.incbin "MAP/MAP_028/SLOT_6.bin.lz"
