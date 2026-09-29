@ Field_Map208: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map208
Field_Map208:
	map_header 0x0, 0x80, 0x80, 2, 2, 3, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map208, .LField_Map208_1
	map_block Field_Map208, .LField_Map208_2
	map_block Field_Map208, .LField_Map208_3
	map_block Field_Map208, .LField_Map208_4
	map_block Field_Map208, .LField_Map208_5
	map_block Field_Map208, .LField_Map208_6
	.incbin "MAP/MAP_208/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map208_1:
	.incbin "MAP/MAP_208/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map208_2:
	.incbin "MAP/MAP_208/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map208_3:
	.incbin "MAP/MAP_208/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map208_4:
	.incbin "MAP/MAP_208/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map208_5:
	.incbin "MAP/MAP_208/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map208_6:
	.incbin "MAP/MAP_208/SLOT_6.bin.lz"
