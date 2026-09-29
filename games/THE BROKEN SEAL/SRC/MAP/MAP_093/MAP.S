@ Field_Map093: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map093
Field_Map093:
	map_header 0x0, 0x40, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map093, .LField_Map093_1
	map_block Field_Map093, .LField_Map093_2
	map_block Field_Map093, .LField_Map093_3
	map_block Field_Map093, .LField_Map093_4
	map_block Field_Map093, .LField_Map093_5
	.incbin "MAP/MAP_093/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map093_1:
	.incbin "MAP/MAP_093/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map093_2:
	.incbin "MAP/MAP_093/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map093_3:
	.incbin "MAP/MAP_093/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map093_4:
	.incbin "MAP/MAP_093/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map093_5:
	.incbin "MAP/MAP_093/SLOT_5.bin"
