@ Field_Map057: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map057
Field_Map057:
	map_header 0x0, 0x80, 0x80, 3, 1, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map057, .LField_Map057_1
	map_block Field_Map057, .LField_Map057_2
	map_block Field_Map057, .LField_Map057_3
	map_block Field_Map057, .LField_Map057_4
	map_block Field_Map057, .LField_Map057_5
	map_block Field_Map057, .LField_Map057_6
	.incbin "MAP/MAP_057/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map057_1:
	.incbin "MAP/MAP_057/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map057_2:
	.incbin "MAP/MAP_057/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map057_3:
	.incbin "MAP/MAP_057/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map057_4:
	.incbin "MAP/MAP_057/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map057_5:
	.incbin "MAP/MAP_057/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map057_6:
	.incbin "MAP/MAP_057/SLOT_6.bin.lz"
