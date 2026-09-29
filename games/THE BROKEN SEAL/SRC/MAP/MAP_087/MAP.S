@ Field_Map087: a field map of 3 layers.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map087
Field_Map087:
	map_header 0x0, 0x80, 0x80, 3, 1, 2, 1, 1, 2, 0x0, 0x0
	map_layer 16, 16, 0, -1, 0x80
	map_layer 16, 16, 0, -1, 0x8000
	map_layer 16, 16, 0, -1, 0x3c
	.2byte 0x0
	map_block Field_Map087, .LField_Map087_1
	map_block Field_Map087, .LField_Map087_2
	map_block Field_Map087, .LField_Map087_3
	map_block Field_Map087, .LField_Map087_4
	map_block Field_Map087, .LField_Map087_5
	.incbin "MAP/MAP_087/CELLS.delta1.lz"
	.balign 4, 0
.LField_Map087_1:
	.incbin "MAP/MAP_087/SLOT_1.bin.lz"
	.balign 4, 0
.LField_Map087_2:
	.incbin "MAP/MAP_087/SLOT_2.bin.lz"
	.balign 4, 0
.LField_Map087_3:
	.incbin "MAP/MAP_087/SLOT_3.bin.lz"
	.balign 4, 0
.LField_Map087_4:
	.incbin "MAP/MAP_087/SLOT_4.bin.lz"
	.balign 4, 0
.LField_Map087_5:
	.incbin "MAP/MAP_087/SLOT_5.bin"
