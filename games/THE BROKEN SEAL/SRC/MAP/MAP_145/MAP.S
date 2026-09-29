@ Field_Map145: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map145
Field_Map145:
	map_header 0x0, 0x64, 0x64, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6400
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map145, .LField_Map145_1
	map_block Field_Map145, .LField_Map145_2
	map_block Field_Map145, .LField_Map145_3
	map_block Field_Map145, .LField_Map145_4
	map_block Field_Map145, .LField_Map145_5
	.incbin "MAP/MAP_145/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map145_1:
	.incbin "MAP/MAP_145/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map145_2:
	.incbin "MAP/MAP_145/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map145_3:
	.incbin "MAP/MAP_145/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map145_4:
	.incbin "MAP/MAP_145/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map145_5:
	.incbin "MAP/MAP_145/SLOT_5.bin"
