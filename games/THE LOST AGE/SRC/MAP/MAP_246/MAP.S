@ Field_Map246: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map246
Field_Map246:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map246, .LField_Map246_1
	map_block Field_Map246, .LField_Map246_2
	map_block Field_Map246, .LField_Map246_3
	map_block Field_Map246, .LField_Map246_4
	map_block Field_Map246
	map_block Field_Map246, .LField_Map246_6
	.incbin "MAP/MAP_246/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map246_1:
	.incbin "MAP/MAP_246/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map246_2:
	.incbin "MAP/MAP_246/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map246_3:
	.incbin "MAP/MAP_246/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map246_4:
	.incbin "MAP/MAP_246/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map246_6:
	.incbin "MAP/MAP_246/SLOT_6.bin.lz"
