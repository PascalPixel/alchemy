@ Field_Map293: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map293
Field_Map293:
	map_header 0x0, 0x40, 0x40, 3, 0, 0, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x0
	map_layer 16, 16, 0, -1, 0x0
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map293, .LField_Map293_1
	map_block Field_Map293, .LField_Map293_2
	map_block Field_Map293, .LField_Map293_3
	map_block Field_Map293
	map_block Field_Map293
	map_block Field_Map293, .LField_Map293_6
	.incbin "MAP/MAP_293/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map293_1:
	.incbin "MAP/MAP_293/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map293_2:
	.incbin "MAP/MAP_293/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map293_3:
	.incbin "MAP/MAP_293/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map293_6:
	.incbin "MAP/MAP_293/SLOT_6.bin.lz"
