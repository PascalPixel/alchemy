@ Field_Map254: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map254
Field_Map254:
	map_header 0x0, 0x26, 0x32, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x26
	map_layer 16, 16, 0, -1, 0x3200
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map254, .LField_Map254_1
	map_block Field_Map254, .LField_Map254_2
	map_block Field_Map254, .LField_Map254_3
	map_block Field_Map254, .LField_Map254_4
	map_block Field_Map254, .LField_Map254_5
	map_block Field_Map254, .LField_Map254_6
	.incbin "MAP/MAP_254/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map254_1:
	.incbin "MAP/MAP_254/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map254_2:
	.incbin "MAP/MAP_254/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map254_3:
	.incbin "MAP/MAP_254/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map254_4:
	.incbin "MAP/MAP_254/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map254_5:
	.incbin "MAP/MAP_254/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map254_6:
	.incbin "MAP/MAP_254/SLOT_6.bin.lz"
