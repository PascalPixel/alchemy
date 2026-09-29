@ Field_Map042: a field map of 2 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map042
Field_Map042:
	map_header 0x0, 0x40, 0x40, 2, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map042, .LField_Map042_1
	map_block Field_Map042, .LField_Map042_2
	map_block Field_Map042, .LField_Map042_3
	map_block Field_Map042, .LField_Map042_4
	map_block Field_Map042, .LField_Map042_5
	.incbin "MAP/MAP_042/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map042_1:
	.incbin "MAP/MAP_042/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map042_2:
	.incbin "MAP/MAP_042/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map042_3:
	.incbin "MAP/MAP_042/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map042_4:
	.incbin "MAP/MAP_042/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map042_5:
	.incbin "MAP/MAP_042/SLOT_5.bin"
