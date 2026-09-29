@ Field_Map127: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map127
Field_Map127:
	map_header 0x1c, 0xb4, 0x38, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x3800
	map_layer 16, 16, 0, -1, 0x7000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map127, .LField_Map127_1
	map_block Field_Map127, .LField_Map127_2
	map_block Field_Map127, .LField_Map127_3
	map_block Field_Map127, .LField_Map127_4
	map_block Field_Map127, .LField_Map127_5
	.incbin "MAP/MAP_127/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map127_1:
	.incbin "MAP/MAP_127/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map127_2:
	.incbin "MAP/MAP_127/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map127_3:
	.incbin "MAP/MAP_127/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map127_4:
	.incbin "MAP/MAP_127/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map127_5:
	.incbin "MAP/MAP_127/SLOT_5.bin"
