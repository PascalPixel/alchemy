@ Field_Map006: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map006
Field_Map006:
	map_header 0x0, 0x5e, 0x5e, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x6000
	map_layer 16, 16, 0, -1, 0x64
	map_layer 16, 16, 0, -1, 0x40
	.2byte 0x0
	map_block Field_Map006, .LField_Map006_1
	map_block Field_Map006, .LField_Map006_2
	map_block Field_Map006, .LField_Map006_3
	map_block Field_Map006, .LField_Map006_4
	map_block Field_Map006, .LField_Map006_5
	map_block Field_Map006, .LField_Map006_6
	.incbin "MAP/MAP_006/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map006_1:
	.incbin "MAP/MAP_006/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map006_2:
	.incbin "MAP/MAP_006/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map006_3:
	.incbin "MAP/MAP_006/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map006_4:
	.incbin "MAP/MAP_006/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map006_5:
	.incbin "MAP/MAP_006/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map006_6:
	.incbin "MAP/MAP_006/SLOT_6.bin.lz"
