@ Field_Map100: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map100
Field_Map100:
	map_header 0x0, 0x5e, 0x5e, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6000
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map100, .LField_Map100_1
	map_block Field_Map100, .LField_Map100_2
	map_block Field_Map100, .LField_Map100_3
	map_block Field_Map100, .LField_Map100_4
	map_block Field_Map100, .LField_Map100_5
	.incbin "MAP/MAP_100/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map100_1:
	.incbin "MAP/MAP_100/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map100_2:
	.incbin "MAP/MAP_100/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map100_3:
	.incbin "MAP/MAP_100/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map100_4:
	.incbin "MAP/MAP_100/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map100_5:
	.incbin "MAP/MAP_100/SLOT_5.bin"
