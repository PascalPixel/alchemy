@ Field_Map022: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map022
Field_Map022:
	map_header 0x0, 0x5a, 0x64, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6400
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map022, .LField_Map022_1
	map_block Field_Map022, .LField_Map022_2
	map_block Field_Map022, .LField_Map022_3
	map_block Field_Map022, .LField_Map022_4
	map_block Field_Map022, .LField_Map022_5
	map_block Field_Map022, .LField_Map022_6
	.incbin "MAP/MAP_022/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map022_1:
	.incbin "MAP/MAP_022/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map022_2:
	.incbin "MAP/MAP_022/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map022_3:
	.incbin "MAP/MAP_022/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map022_4:
	.incbin "MAP/MAP_022/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map022_5:
	.incbin "MAP/MAP_022/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map022_6:
	.incbin "MAP/MAP_022/SLOT_6.bin.lz"
