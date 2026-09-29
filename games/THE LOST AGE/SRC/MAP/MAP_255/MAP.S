@ Field_Map255: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map255
Field_Map255:
	map_header 0x0, 0x5a, 0x48, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x5a
	map_layer 16, 16, 0, -1, 0x4800
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map255, .LField_Map255_1
	map_block Field_Map255, .LField_Map255_2
	map_block Field_Map255, .LField_Map255_3
	map_block Field_Map255, .LField_Map255_4
	map_block Field_Map255, .LField_Map255_5
	map_block Field_Map255, .LField_Map255_6
	.incbin "MAP/MAP_255/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map255_1:
	.incbin "MAP/MAP_255/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map255_2:
	.incbin "MAP/MAP_255/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map255_3:
	.incbin "MAP/MAP_255/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map255_4:
	.incbin "MAP/MAP_255/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map255_5:
	.incbin "MAP/MAP_255/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map255_6:
	.incbin "MAP/MAP_255/SLOT_6.bin.lz"
