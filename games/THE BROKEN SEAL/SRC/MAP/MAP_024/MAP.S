@ Field_Map024: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map024
Field_Map024:
	map_header 0x0, 0x60, 0x3c, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x3c00
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map024, .LField_Map024_1
	map_block Field_Map024, .LField_Map024_2
	map_block Field_Map024, .LField_Map024_3
	map_block Field_Map024, .LField_Map024_4
	map_block Field_Map024, .LField_Map024_5
	.incbin "MAP/MAP_024/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map024_1:
	.incbin "MAP/MAP_024/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map024_2:
	.incbin "MAP/MAP_024/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map024_3:
	.incbin "MAP/MAP_024/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map024_4:
	.incbin "MAP/MAP_024/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map024_5:
	.incbin "MAP/MAP_024/SLOT_5.bin"
