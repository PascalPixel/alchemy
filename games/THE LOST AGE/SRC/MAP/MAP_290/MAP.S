@ Field_Map290: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map290
Field_Map290:
	map_header 0x0, 0x64, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map290, .LField_Map290_1
	map_block Field_Map290, .LField_Map290_2
	map_block Field_Map290, .LField_Map290_3
	map_block Field_Map290, .LField_Map290_4
	map_block Field_Map290, .LField_Map290_5
	map_block Field_Map290, .LField_Map290_6
	.incbin "MAP/MAP_290/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map290_1:
	.incbin "MAP/MAP_290/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map290_2:
	.incbin "MAP/MAP_290/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map290_3:
	.incbin "MAP/MAP_290/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map290_4:
	.incbin "MAP/MAP_290/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map290_5:
	.incbin "MAP/MAP_290/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map290_6:
	.incbin "MAP/MAP_290/SLOT_6.bin.lz"
