@ Field_Map182: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map182
Field_Map182:
	map_header 0x0, 0x80, 0x80, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map182, .LField_Map182_1
	map_block Field_Map182, .LField_Map182_2
	map_block Field_Map182, .LField_Map182_3
	map_block Field_Map182, .LField_Map182_4
	map_block Field_Map182, .LField_Map182_5
	map_block Field_Map182, .LField_Map182_6
	.incbin "MAP/MAP_182/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map182_1:
	.incbin "MAP/MAP_182/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map182_2:
	.incbin "MAP/MAP_182/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map182_3:
	.incbin "MAP/MAP_182/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map182_4:
	.incbin "MAP/MAP_182/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map182_5:
	.incbin "MAP/MAP_182/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map182_6:
	.incbin "MAP/MAP_182/SLOT_6.bin.lz"
