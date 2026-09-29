@ Field_Map249: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map249
Field_Map249:
	map_header 0x0, 0xcc, 0x40, 3, 3, 2, 0, 2, 2, 0x0, 0x0
	map_layer 12, 12, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map249, .LField_Map249_1
	map_block Field_Map249, .LField_Map249_2
	map_block Field_Map249, .LField_Map249_3
	map_block Field_Map249, .LField_Map249_4
	map_block Field_Map249
	map_block Field_Map249, .LField_Map249_6
	.incbin "MAP/MAP_249/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map249_1:
	.incbin "MAP/MAP_249/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map249_2:
	.incbin "MAP/MAP_249/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map249_3:
	.incbin "MAP/MAP_249/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map249_4:
	.incbin "MAP/MAP_249/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map249_6:
	.incbin "MAP/MAP_249/SLOT_6.bin.lz"
