@ Field_Map000: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map000
Field_Map000:
	map_header 0x0, 0x3c, 0xbc, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x3c
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map000, .LField_Map000_1
	map_block Field_Map000, .LField_Map000_2
	map_block Field_Map000, .LField_Map000_3
	map_block Field_Map000
	map_block Field_Map000, .LField_Map000_5
	.incbin "MAP/MAP_000/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map000_1:
	.incbin "MAP/MAP_000/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map000_2:
	.incbin "MAP/MAP_000/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map000_3:
	.incbin "MAP/MAP_000/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map000_5:
	.incbin "MAP/MAP_000/SLOT_5.bin"
