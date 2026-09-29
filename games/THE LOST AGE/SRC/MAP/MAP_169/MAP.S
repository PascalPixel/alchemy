@ Field_Map169: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map169
Field_Map169:
	map_header 0x0, 0x82, 0x70, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map169, .LField_Map169_1
	map_block Field_Map169, .LField_Map169_2
	map_block Field_Map169, .LField_Map169_3
	map_block Field_Map169, .LField_Map169_4
	map_block Field_Map169, .LField_Map169_5
	map_block Field_Map169, .LField_Map169_6
	.incbin "MAP/MAP_169/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map169_1:
	.incbin "MAP/MAP_169/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map169_2:
	.incbin "MAP/MAP_169/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map169_3:
	.incbin "MAP/MAP_169/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map169_4:
	.incbin "MAP/MAP_169/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map169_5:
	.incbin "MAP/MAP_169/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map169_6:
	.incbin "MAP/MAP_169/SLOT_6.bin.lz"
