@ Field_Map220: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map220
Field_Map220:
	map_header 0x0, 0x80, 0x80, 3, 1, 1, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 257, 7967, 0x40
	.2byte 0x0
	map_block Field_Map220, .LField_Map220_1
	map_block Field_Map220, .LField_Map220_2
	map_block Field_Map220, .LField_Map220_3
	map_block Field_Map220, .LField_Map220_4
	map_block Field_Map220, .LField_Map220_5
	map_block Field_Map220, .LField_Map220_6
	.incbin "MAP/MAP_220/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map220_1:
	.incbin "MAP/MAP_220/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map220_2:
	.incbin "MAP/MAP_220/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map220_3:
	.incbin "MAP/MAP_220/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map220_4:
	.incbin "MAP/MAP_220/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map220_5:
	.incbin "MAP/MAP_220/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map220_6:
	.incbin "MAP/MAP_220/SLOT_6.bin.lz"
