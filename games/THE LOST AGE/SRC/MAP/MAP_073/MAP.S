@ Field_Map073: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map073
Field_Map073:
	map_header 0x0, 0x80, 0x80, 3, 2, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map073, .LField_Map073_1
	map_block Field_Map073, .LField_Map073_2
	map_block Field_Map073, .LField_Map073_3
	map_block Field_Map073, .LField_Map073_4
	map_block Field_Map073
	map_block Field_Map073, .LField_Map073_6
	.incbin "MAP/MAP_073/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map073_1:
	.incbin "MAP/MAP_073/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map073_2:
	.incbin "MAP/MAP_073/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map073_3:
	.incbin "MAP/MAP_073/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map073_4:
	.incbin "MAP/MAP_073/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map073_6:
	.incbin "MAP/MAP_073/SLOT_6.bin.lz"
