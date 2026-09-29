@ Field_Map013: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map013
Field_Map013:
	map_header 0x0, 0x6e, 0x6e, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6e00
	map_layer 16, 16, 0, -1, 0x6e
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map013, .LField_Map013_1
	map_block Field_Map013, .LField_Map013_2
	map_block Field_Map013, .LField_Map013_3
	map_block Field_Map013, .LField_Map013_4
	map_block Field_Map013, .LField_Map013_5
	map_block Field_Map013, .LField_Map013_6
	.incbin "MAP/MAP_013/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map013_1:
	.incbin "MAP/MAP_013/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map013_2:
	.incbin "MAP/MAP_013/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map013_3:
	.incbin "MAP/MAP_013/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map013_4:
	.incbin "MAP/MAP_013/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map013_5:
	.incbin "MAP/MAP_013/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map013_6:
	.incbin "MAP/MAP_013/SLOT_6.bin.lz"
