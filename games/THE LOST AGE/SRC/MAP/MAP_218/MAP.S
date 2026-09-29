@ Field_Map218: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map218
Field_Map218:
	map_header 0x0, 0x68, 0x48, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x68
	map_layer 16, 16, 0, -1, 0x4800
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map218, .LField_Map218_1
	map_block Field_Map218, .LField_Map218_2
	map_block Field_Map218, .LField_Map218_3
	map_block Field_Map218, .LField_Map218_4
	map_block Field_Map218, .LField_Map218_5
	map_block Field_Map218, .LField_Map218_6
	.incbin "MAP/MAP_218/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map218_1:
	.incbin "MAP/MAP_218/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map218_2:
	.incbin "MAP/MAP_218/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map218_3:
	.incbin "MAP/MAP_218/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map218_4:
	.incbin "MAP/MAP_218/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map218_5:
	.incbin "MAP/MAP_218/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map218_6:
	.incbin "MAP/MAP_218/SLOT_6.bin.lz"
