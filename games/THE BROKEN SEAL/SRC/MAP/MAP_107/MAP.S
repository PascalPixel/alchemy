@ Field_Map107: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map107
Field_Map107:
	map_header 0x0, 0x40, 0x4e, 3, 2, 1, 1, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x4c
	map_layer 16, 16, 0, -1, 0x98
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map107, .LField_Map107_1
	map_block Field_Map107, .LField_Map107_2
	map_block Field_Map107, .LField_Map107_3
	map_block Field_Map107
	map_block Field_Map107, .LField_Map107_5
	.incbin "MAP/MAP_107/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map107_1:
	.incbin "MAP/MAP_107/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map107_2:
	.incbin "MAP/MAP_107/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map107_3:
	.incbin "MAP/MAP_107/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map107_5:
	.incbin "MAP/MAP_107/SLOT_5.bin"
