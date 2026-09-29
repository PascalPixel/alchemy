@ Field_Map256: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map256
Field_Map256:
	map_header 0x0, 0x5a, 0x42, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x5a
	map_layer 16, 16, 0, -1, 0x4200
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map256, .LField_Map256_1
	map_block Field_Map256, .LField_Map256_2
	map_block Field_Map256, .LField_Map256_3
	map_block Field_Map256, .LField_Map256_4
	map_block Field_Map256, .LField_Map256_5
	map_block Field_Map256, .LField_Map256_6
	.incbin "MAP/MAP_256/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map256_1:
	.incbin "MAP/MAP_256/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map256_2:
	.incbin "MAP/MAP_256/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map256_3:
	.incbin "MAP/MAP_256/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map256_4:
	.incbin "MAP/MAP_256/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map256_5:
	.incbin "MAP/MAP_256/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map256_6:
	.incbin "MAP/MAP_256/SLOT_6.bin.lz"
