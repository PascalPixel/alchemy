@ Field_Map007: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map007
Field_Map007:
	map_header 0x0, 0x30, 0x21, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map007, .LField_Map007_1
	map_block Field_Map007, .LField_Map007_2
	map_block Field_Map007, .LField_Map007_3
	map_block Field_Map007, .LField_Map007_4
	map_block Field_Map007, .LField_Map007_5
	.incbin "MAP/MAP_007/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map007_1:
	.incbin "MAP/MAP_007/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map007_2:
	.incbin "MAP/MAP_007/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map007_3:
	.incbin "MAP/MAP_007/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map007_4:
	.incbin "MAP/MAP_007/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map007_5:
	.incbin "MAP/MAP_007/SLOT_5.bin"
