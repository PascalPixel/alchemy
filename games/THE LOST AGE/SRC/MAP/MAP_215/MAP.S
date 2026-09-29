@ Field_Map215: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map215
Field_Map215:
	map_header 0x0, 0x82, 0x70, 3, 3, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map215, .LField_Map215_1
	map_block Field_Map215, .LField_Map215_2
	map_block Field_Map215, .LField_Map215_3
	map_block Field_Map215, .LField_Map215_4
	map_block Field_Map215, .LField_Map215_5
	map_block Field_Map215, .LField_Map215_6
	.incbin "MAP/MAP_215/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map215_1:
	.incbin "MAP/MAP_215/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map215_2:
	.incbin "MAP/MAP_215/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map215_3:
	.incbin "MAP/MAP_215/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map215_4:
	.incbin "MAP/MAP_215/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map215_5:
	.incbin "MAP/MAP_215/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map215_6:
	.incbin "MAP/MAP_215/SLOT_6.bin.lz"
