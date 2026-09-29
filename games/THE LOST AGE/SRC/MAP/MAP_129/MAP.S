@ Field_Map129: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map129
Field_Map129:
	map_header 0x0, 0x3a, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map129, .LField_Map129_1
	map_block Field_Map129, .LField_Map129_2
	map_block Field_Map129, .LField_Map129_3
	map_block Field_Map129, .LField_Map129_4
	map_block Field_Map129, .LField_Map129_5
	map_block Field_Map129, .LField_Map129_6
	.incbin "MAP/MAP_129/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map129_1:
	.incbin "MAP/MAP_129/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map129_2:
	.incbin "MAP/MAP_129/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map129_3:
	.incbin "MAP/MAP_129/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map129_4:
	.incbin "MAP/MAP_129/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map129_5:
	.incbin "MAP/MAP_129/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map129_6:
	.incbin "MAP/MAP_129/SLOT_6.bin.lz"
