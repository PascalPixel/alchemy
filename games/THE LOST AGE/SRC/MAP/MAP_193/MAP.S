@ Field_Map193: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map193
Field_Map193:
	map_header 0x0, 0x1e, 0x16, 3, 1, 2, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8400
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map193, .LField_Map193_1
	map_block Field_Map193, .LField_Map193_2
	map_block Field_Map193, .LField_Map193_3
	map_block Field_Map193, .LField_Map193_4
	map_block Field_Map193, .LField_Map193_5
	map_block Field_Map193, .LField_Map193_6
	.incbin "MAP/MAP_193/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map193_1:
	.incbin "MAP/MAP_193/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map193_2:
	.incbin "MAP/MAP_193/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map193_3:
	.incbin "MAP/MAP_193/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map193_4:
	.incbin "MAP/MAP_193/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map193_5:
	.incbin "MAP/MAP_193/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map193_6:
	.incbin "MAP/MAP_193/SLOT_6.bin.lz"
