@ Field_Map148: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map148
Field_Map148:
	map_header 0x0, 0x80, 0x80, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map148, .LField_Map148_1
	map_block Field_Map148, .LField_Map148_2
	map_block Field_Map148, .LField_Map148_3
	map_block Field_Map148, .LField_Map148_4
	map_block Field_Map148, .LField_Map148_5
	map_block Field_Map148, .LField_Map148_6
	.incbin "MAP/MAP_148/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map148_1:
	.incbin "MAP/MAP_148/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map148_2:
	.incbin "MAP/MAP_148/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map148_3:
	.incbin "MAP/MAP_148/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map148_4:
	.incbin "MAP/MAP_148/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map148_5:
	.incbin "MAP/MAP_148/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map148_6:
	.incbin "MAP/MAP_148/SLOT_6.bin.lz"
