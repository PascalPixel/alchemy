@ Field_Map239: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map239
Field_Map239:
	map_header 0x0, 0x40, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x40
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map239, .LField_Map239_1
	map_block Field_Map239, .LField_Map239_2
	map_block Field_Map239, .LField_Map239_3
	map_block Field_Map239, .LField_Map239_4
	map_block Field_Map239, .LField_Map239_5
	map_block Field_Map239, .LField_Map239_6
	.incbin "MAP/MAP_239/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map239_1:
	.incbin "MAP/MAP_239/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map239_2:
	.incbin "MAP/MAP_239/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map239_3:
	.incbin "MAP/MAP_239/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map239_4:
	.incbin "MAP/MAP_239/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map239_5:
	.incbin "MAP/MAP_239/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map239_6:
	.incbin "MAP/MAP_239/SLOT_6.bin.lz"
