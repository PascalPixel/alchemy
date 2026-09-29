@ Field_Map021: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map021
Field_Map021:
	map_header 0x0, 0x5a, 0x64, 3, 1, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6400
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map021, .LField_Map021_1
	map_block Field_Map021, .LField_Map021_2
	map_block Field_Map021, .LField_Map021_3
	map_block Field_Map021, .LField_Map021_4
	map_block Field_Map021, .LField_Map021_5
	map_block Field_Map021, .LField_Map021_6
	.incbin "MAP/MAP_021/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map021_1:
	.incbin "MAP/MAP_021/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map021_2:
	.incbin "MAP/MAP_021/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map021_3:
	.incbin "MAP/MAP_021/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map021_4:
	.incbin "MAP/MAP_021/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map021_5:
	.incbin "MAP/MAP_021/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map021_6:
	.incbin "MAP/MAP_021/SLOT_6.bin.lz"
