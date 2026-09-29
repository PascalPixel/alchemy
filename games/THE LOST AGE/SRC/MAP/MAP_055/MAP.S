@ Field_Map055: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map055
Field_Map055:
	map_header 0x0, 0x80, 0x80, 2, 3, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 11, 14, 0, 7967, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map055, .LField_Map055_1
	map_block Field_Map055, .LField_Map055_2
	map_block Field_Map055, .LField_Map055_3
	map_block Field_Map055, .LField_Map055_4
	map_block Field_Map055, .LField_Map055_5
	map_block Field_Map055, .LField_Map055_6
	.incbin "MAP/MAP_055/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map055_1:
	.incbin "MAP/MAP_055/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map055_2:
	.incbin "MAP/MAP_055/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map055_3:
	.incbin "MAP/MAP_055/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map055_4:
	.incbin "MAP/MAP_055/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map055_5:
	.incbin "MAP/MAP_055/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map055_6:
	.incbin "MAP/MAP_055/SLOT_6.bin.lz"
