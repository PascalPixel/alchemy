	.section .rodata
	.global Korosseo_RivalPathC
Korosseo_RivalPathC:
	.incbin "MAP/KOROSSEO/RIVAL_PATH_C.table.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesE at +0x1a8e */
	.byte 0x8d, 0x8f
