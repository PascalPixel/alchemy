@ Field_Map217: a field map of 2 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map217
Field_Map217:
	map_header 0x0, 0x80, 0x80, 2, 1, 3, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map217, .LField_Map217_1
	map_block Field_Map217, .LField_Map217_2
	map_block Field_Map217, .LField_Map217_3
	map_block Field_Map217, .LField_Map217_4
	map_block Field_Map217
	map_block Field_Map217, .LField_Map217_6
	.incbin "MAP/MAP_217/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map217_1:
	.incbin "MAP/MAP_217/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map217_2:
	.incbin "MAP/MAP_217/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map217_3:
	.incbin "MAP/MAP_217/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map217_4:
	.incbin "MAP/MAP_217/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map217_6:
	.incbin "MAP/MAP_217/SLOT_6.bin.lz"
