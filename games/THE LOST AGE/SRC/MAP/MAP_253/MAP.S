@ Field_Map253: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map253
Field_Map253:
	map_header 0x0, 0x5c, 0x80, 3, 1, 2, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map253, .LField_Map253_1
	map_block Field_Map253, .LField_Map253_2
	map_block Field_Map253, .LField_Map253_3
	map_block Field_Map253, .LField_Map253_4
	map_block Field_Map253, .LField_Map253_5
	map_block Field_Map253, .LField_Map253_6
	.incbin "MAP/MAP_253/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map253_1:
	.incbin "MAP/MAP_253/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map253_2:
	.incbin "MAP/MAP_253/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map253_3:
	.incbin "MAP/MAP_253/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map253_4:
	.incbin "MAP/MAP_253/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map253_5:
	.incbin "MAP/MAP_253/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map253_6:
	.incbin "MAP/MAP_253/SLOT_6.bin.lz"
