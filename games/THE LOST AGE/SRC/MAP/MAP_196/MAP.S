@ Field_Map196: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map196
Field_Map196:
	map_header 0x0, 0x80, 0x80, 3, 2, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map196, .LField_Map196_1
	map_block Field_Map196, .LField_Map196_2
	map_block Field_Map196, .LField_Map196_3
	map_block Field_Map196, .LField_Map196_4
	map_block Field_Map196, .LField_Map196_5
	map_block Field_Map196, .LField_Map196_6
	.incbin "MAP/MAP_196/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map196_1:
	.incbin "MAP/MAP_196/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map196_2:
	.incbin "MAP/MAP_196/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map196_3:
	.incbin "MAP/MAP_196/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map196_4:
	.incbin "MAP/MAP_196/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map196_5:
	.incbin "MAP/MAP_196/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map196_6:
	.incbin "MAP/MAP_196/SLOT_6.bin.lz"
