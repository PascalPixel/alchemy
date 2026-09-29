@ Field_Map185: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map185
Field_Map185:
	map_header 0x0, 0x80, 0x80, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map185, .LField_Map185_1
	map_block Field_Map185, .LField_Map185_2
	map_block Field_Map185, .LField_Map185_3
	map_block Field_Map185, .LField_Map185_4
	map_block Field_Map185, .LField_Map185_5
	map_block Field_Map185, .LField_Map185_6
	.incbin "MAP/MAP_185/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map185_1:
	.incbin "MAP/MAP_185/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map185_2:
	.incbin "MAP/MAP_185/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map185_3:
	.incbin "MAP/MAP_185/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map185_4:
	.incbin "MAP/MAP_185/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map185_5:
	.incbin "MAP/MAP_185/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map185_6:
	.incbin "MAP/MAP_185/SLOT_6.bin.lz"
