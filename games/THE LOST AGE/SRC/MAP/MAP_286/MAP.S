@ Field_Map286: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map286
Field_Map286:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map286, .LField_Map286_1
	map_block Field_Map286, .LField_Map286_2
	map_block Field_Map286, .LField_Map286_3
	map_block Field_Map286, .LField_Map286_4
	map_block Field_Map286, .LField_Map286_5
	map_block Field_Map286, .LField_Map286_6
	.incbin "MAP/MAP_286/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map286_1:
	.incbin "MAP/MAP_286/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map286_2:
	.incbin "MAP/MAP_286/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map286_3:
	.incbin "MAP/MAP_286/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map286_4:
	.incbin "MAP/MAP_286/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map286_5:
	.incbin "MAP/MAP_286/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map286_6:
	.incbin "MAP/MAP_286/SLOT_6.bin.lz"
