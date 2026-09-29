@ Field_Map099: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map099
Field_Map099:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map099, .LField_Map099_1
	map_block Field_Map099, .LField_Map099_2
	map_block Field_Map099, .LField_Map099_3
	map_block Field_Map099, .LField_Map099_4
	map_block Field_Map099, .LField_Map099_5
	map_block Field_Map099, .LField_Map099_6
	.incbin "MAP/MAP_099/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map099_1:
	.incbin "MAP/MAP_099/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map099_2:
	.incbin "MAP/MAP_099/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map099_3:
	.incbin "MAP/MAP_099/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map099_4:
	.incbin "MAP/MAP_099/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map099_5:
	.incbin "MAP/MAP_099/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map099_6:
	.incbin "MAP/MAP_099/SLOT_6.bin.lz"
