@ Field_Map061: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map061
Field_Map061:
	map_header 0x0, 0x78, 0x78, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map061, .LField_Map061_1
	map_block Field_Map061, .LField_Map061_2
	map_block Field_Map061, .LField_Map061_3
	map_block Field_Map061, .LField_Map061_4
	map_block Field_Map061, .LField_Map061_5
	.incbin "MAP/MAP_061/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map061_1:
	.incbin "MAP/MAP_061/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map061_2:
	.incbin "MAP/MAP_061/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map061_3:
	.incbin "MAP/MAP_061/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map061_4:
	.incbin "MAP/MAP_061/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map061_5:
	.incbin "MAP/MAP_061/SLOT_5.bin"
