@ Field_Map095: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map095
Field_Map095:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map095, .LField_Map095_1
	map_block Field_Map095, .LField_Map095_2
	map_block Field_Map095, .LField_Map095_3
	map_block Field_Map095, .LField_Map095_4
	map_block Field_Map095, .LField_Map095_5
	.incbin "MAP/MAP_095/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map095_1:
	.incbin "MAP/MAP_095/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map095_2:
	.incbin "MAP/MAP_095/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map095_3:
	.incbin "MAP/MAP_095/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map095_4:
	.incbin "MAP/MAP_095/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map095_5:
	.incbin "MAP/MAP_095/SLOT_5.bin"
