@ Field_Map276: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map276
Field_Map276:
	map_header 0x0, 0xcc, 0x50, 3, 3, 2, 0, 2, 2, 0x0, 0x0
	map_layer 12, 12, -258, 3855, 0x5000
	map_layer 16, 16, 0, -1, 0xa000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map276, .LField_Map276_1
	map_block Field_Map276, .LField_Map276_2
	map_block Field_Map276, .LField_Map276_3
	map_block Field_Map276, .LField_Map276_4
	map_block Field_Map276
	map_block Field_Map276, .LField_Map276_6
	.incbin "MAP/MAP_276/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map276_1:
	.incbin "MAP/MAP_276/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map276_2:
	.incbin "MAP/MAP_276/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map276_3:
	.incbin "MAP/MAP_276/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map276_4:
	.incbin "MAP/MAP_276/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map276_6:
	.incbin "MAP/MAP_276/SLOT_6.bin.lz"
