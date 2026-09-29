@ Field_Map089: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map089
Field_Map089:
	map_header 0x0, 0x37, 0x44, 3, 1, 2, 0, 2, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x7800
	map_layer 16, 16, 0, -1, 0x78
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map089, .LField_Map089_1
	map_block Field_Map089, .LField_Map089_2
	map_block Field_Map089, .LField_Map089_3
	map_block Field_Map089, .LField_Map089_4
	map_block Field_Map089, .LField_Map089_5
	.incbin "MAP/MAP_089/CELLS.delta0.lz"
	.balign 4, 0
.LField_Map089_1:
	.incbin "MAP/MAP_089/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map089_2:
	.incbin "MAP/MAP_089/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map089_3:
	.incbin "MAP/MAP_089/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map089_4:
	.incbin "MAP/MAP_089/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map089_5:
	.incbin "MAP/MAP_089/SLOT_5.bin"
