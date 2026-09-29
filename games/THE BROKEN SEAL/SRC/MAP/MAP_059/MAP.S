@ Field_Map059: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map059
Field_Map059:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map059, .LField_Map059_1
	map_block Field_Map059, .LField_Map059_2
	map_block Field_Map059, .LField_Map059_3
	map_block Field_Map059
	map_block Field_Map059, .LField_Map059_5
	.incbin "MAP/MAP_059/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map059_1:
	.incbin "MAP/MAP_059/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map059_2:
	.incbin "MAP/MAP_059/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map059_3:
	.incbin "MAP/MAP_059/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map059_5:
	.incbin "MAP/MAP_059/SLOT_5.bin"
