@ Field_Map111: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map111
Field_Map111:
	map_header 0x0, 0x80, 0x80, 3, 1, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map111, .LField_Map111_1
	map_block Field_Map111, .LField_Map111_2
	map_block Field_Map111, .LField_Map111_3
	map_block Field_Map111, .LField_Map111_4
	map_block Field_Map111, .LField_Map111_5
	.incbin "MAP/MAP_111/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map111_1:
	.incbin "MAP/MAP_111/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map111_2:
	.incbin "MAP/MAP_111/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map111_3:
	.incbin "MAP/MAP_111/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map111_4:
	.incbin "MAP/MAP_111/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map111_5:
	.incbin "MAP/MAP_111/SLOT_5.bin"
