@ Field_Map083: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map083
Field_Map083:
	map_header 0x0, 0x80, 0x80, 3, 1, 2, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map083, .LField_Map083_1
	map_block Field_Map083, .LField_Map083_2
	map_block Field_Map083, .LField_Map083_3
	map_block Field_Map083, .LField_Map083_4
	map_block Field_Map083, .LField_Map083_5
	.incbin "MAP/MAP_083/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map083_1:
	.incbin "MAP/MAP_083/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map083_2:
	.incbin "MAP/MAP_083/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map083_3:
	.incbin "MAP/MAP_083/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map083_4:
	.incbin "MAP/MAP_083/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map083_5:
	.incbin "MAP/MAP_083/SLOT_5.bin"
