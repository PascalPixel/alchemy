@ Field_Map070: a field map of 3 layers.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map070
Field_Map070:
	map_header 0x0, 0x80, 0x80, 3, 2, 1, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 514, 3855, 0x40
	.2byte 0x0
	map_block Field_Map070, .LField_Map070_1
	map_block Field_Map070, .LField_Map070_2
	map_block Field_Map070, .LField_Map070_3
	map_block Field_Map070, .LField_Map070_4
	map_block Field_Map070, .LField_Map070_5
	map_block Field_Map070, .LField_Map070_6
	.incbin "MAP/MAP_070/CELLS.delta2.lz"
	.balign 4, 0
.LField_Map070_1:
	.incbin "MAP/MAP_070/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map070_2:
	.incbin "MAP/MAP_070/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map070_3:
	.incbin "MAP/MAP_070/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map070_4:
	.incbin "MAP/MAP_070/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map070_5:
	.incbin "MAP/MAP_070/SLOT_5.bin.lz"
	.balign 4, 0
.LField_Map070_6:
	.incbin "MAP/MAP_070/SLOT_6.bin.lz"
