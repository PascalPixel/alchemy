@ Field_Map278: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map278
Field_Map278:
	map_header 0x0, 0x60, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6000
	map_layer 16, 16, 0, -1, 0x60
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map278, .LField_Map278_1
	map_block Field_Map278, .LField_Map278_2
	map_block Field_Map278, .LField_Map278_3
	map_block Field_Map278, .LField_Map278_4
	map_block Field_Map278, .LField_Map278_5
	map_block Field_Map278, .LField_Map278_6
	.incbin "MAP/MAP_278/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map278_1:
	.incbin "MAP/MAP_278/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map278_2:
	.incbin "MAP/MAP_278/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map278_3:
	.incbin "MAP/MAP_278/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map278_4:
	.incbin "MAP/MAP_278/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map278_5:
	.incbin "MAP/MAP_278/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map278_6:
	.incbin "MAP/MAP_278/SLOT_6.bin.lz"
