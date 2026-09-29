@ Field_Map150: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map150
Field_Map150:
	map_header 0x0, 0x82, 0x70, 3, 2, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map150, .LField_Map150_1
	map_block Field_Map150, .LField_Map150_2
	map_block Field_Map150, .LField_Map150_3
	map_block Field_Map150, .LField_Map150_4
	map_block Field_Map150, .LField_Map150_5
	map_block Field_Map150, .LField_Map150_6
	.incbin "MAP/MAP_150/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map150_1:
	.incbin "MAP/MAP_150/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map150_2:
	.incbin "MAP/MAP_150/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map150_3:
	.incbin "MAP/MAP_150/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map150_4:
	.incbin "MAP/MAP_150/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map150_5:
	.incbin "MAP/MAP_150/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map150_6:
	.incbin "MAP/MAP_150/SLOT_6.bin.lz"
