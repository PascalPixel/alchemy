@ Field_Map172: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map172
Field_Map172:
	map_header 0x0, 0x80, 0x60, 3, 1, 1, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 1021, 7967, 0x40
	.2byte 0x0
	map_block Field_Map172, .LField_Map172_1
	map_block Field_Map172, .LField_Map172_2
	map_block Field_Map172, .LField_Map172_3
	map_block Field_Map172, .LField_Map172_4
	map_block Field_Map172, .LField_Map172_5
	map_block Field_Map172, .LField_Map172_6
	.incbin "MAP/MAP_172/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map172_1:
	.incbin "MAP/MAP_172/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map172_2:
	.incbin "MAP/MAP_172/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map172_3:
	.incbin "MAP/MAP_172/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map172_4:
	.incbin "MAP/MAP_172/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map172_5:
	.incbin "MAP/MAP_172/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map172_6:
	.incbin "MAP/MAP_172/SLOT_6.bin.lz"
