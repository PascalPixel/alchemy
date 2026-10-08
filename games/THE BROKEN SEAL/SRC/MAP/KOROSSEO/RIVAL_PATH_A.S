	.section .rodata
	.global Korosseo_RivalPathA
Korosseo_RivalPathA:
	.incbin "MAP/KOROSSEO/RIVAL_PATH_A.table.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesE at +0x1899 */
	.byte 0xe0, 0xc8, 0xb4
