@ Field_Map136: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map136
Field_Map136:
	map_header 0x0, 0x5a, 0x64, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6400
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map136, .LField_Map136_1
	map_block Field_Map136, .LField_Map136_2
	map_block Field_Map136, .LField_Map136_3
	map_block Field_Map136, .LField_Map136_4
	map_block Field_Map136, .LField_Map136_5
	map_block Field_Map136, .LField_Map136_6
	.incbin "MAP/MAP_136/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map136_1:
	.incbin "MAP/MAP_136/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map136_2:
	.incbin "MAP/MAP_136/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map136_3:
	.incbin "MAP/MAP_136/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map136_4:
	.incbin "MAP/MAP_136/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map136_5:
	.incbin "MAP/MAP_136/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map136_6:
	.incbin "MAP/MAP_136/SLOT_6.bin.lz"
