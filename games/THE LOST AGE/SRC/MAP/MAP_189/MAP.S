@ Field_Map189: a field map of 1 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map189
Field_Map189:
	map_header 0x0, 0x80, 0xa0, 1, 1, 3, 1, 2, 1, 0x0, 0x80
	map_layer 16, 16, 0, -1, 0xa000
	map_layer 16, 16, 771, 7967, 0x0
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map189, .LField_Map189_1
	map_block Field_Map189, .LField_Map189_2
	map_block Field_Map189, .LField_Map189_3
	map_block Field_Map189, .LField_Map189_4
	map_block Field_Map189, .LField_Map189_5
	map_block Field_Map189, .LField_Map189_6
	.incbin "MAP/MAP_189/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map189_1:
	.incbin "MAP/MAP_189/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map189_2:
	.incbin "MAP/MAP_189/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map189_3:
	.incbin "MAP/MAP_189/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map189_4:
	.incbin "MAP/MAP_189/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map189_5:
	.incbin "MAP/MAP_189/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map189_6:
	.incbin "MAP/MAP_189/SLOT_6.bin.lz"
