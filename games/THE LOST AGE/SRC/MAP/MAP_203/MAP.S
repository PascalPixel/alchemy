@ Field_Map203: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map203
Field_Map203:
	map_header 0x0, 0x3c, 0x3c, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x3c00
	map_layer 16, 16, 0, -1, 0x3c
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map203, .LField_Map203_1
	map_block Field_Map203, .LField_Map203_2
	map_block Field_Map203, .LField_Map203_3
	map_block Field_Map203, .LField_Map203_4
	map_block Field_Map203, .LField_Map203_5
	map_block Field_Map203, .LField_Map203_6
	.incbin "MAP/MAP_203/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map203_1:
	.incbin "MAP/MAP_203/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map203_2:
	.incbin "MAP/MAP_203/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map203_3:
	.incbin "MAP/MAP_203/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map203_4:
	.incbin "MAP/MAP_203/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map203_5:
	.incbin "MAP/MAP_203/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map203_6:
	.incbin "MAP/MAP_203/SLOT_6.bin.lz"
