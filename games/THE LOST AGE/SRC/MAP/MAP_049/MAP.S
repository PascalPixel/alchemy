@ Field_Map049: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map049
Field_Map049:
	map_header 0x0, 0x4a, 0x40, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x4a
	map_layer 16, 16, 0, -1, 0x4000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map049, .LField_Map049_1
	map_block Field_Map049, .LField_Map049_2
	map_block Field_Map049, .LField_Map049_3
	map_block Field_Map049, .LField_Map049_4
	map_block Field_Map049, .LField_Map049_5
	map_block Field_Map049, .LField_Map049_6
	.incbin "MAP/MAP_049/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map049_1:
	.incbin "MAP/MAP_049/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map049_2:
	.incbin "MAP/MAP_049/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map049_3:
	.incbin "MAP/MAP_049/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map049_4:
	.incbin "MAP/MAP_049/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map049_5:
	.incbin "MAP/MAP_049/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map049_6:
	.incbin "MAP/MAP_049/SLOT_6.bin.lz"
