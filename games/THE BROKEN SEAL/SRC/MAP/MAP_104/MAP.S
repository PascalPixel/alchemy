@ Field_Map104: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map104
Field_Map104:
	map_header 0x0, 0x2e, 0x2e, 3, 1, 2, 0, 2, 2, 0x0, 0x5c00
	map_layer 6, 6, 0, -1, 0x2e00
	map_layer 16, 16, 0, -1, 0x0
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map104, .LField_Map104_1
	map_block Field_Map104, .LField_Map104_2
	map_block Field_Map104, .LField_Map104_3
	map_block Field_Map104
	map_block Field_Map104, .LField_Map104_5
	.incbin "MAP/MAP_104/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map104_1:
	.incbin "MAP/MAP_104/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map104_2:
	.incbin "MAP/MAP_104/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map104_3:
	.incbin "MAP/MAP_104/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map104_5:
	.incbin "MAP/MAP_104/SLOT_5.bin"
