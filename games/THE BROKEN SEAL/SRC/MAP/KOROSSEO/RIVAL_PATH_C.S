	.section .rodata
	.global Korosseo_RivalPathC
Korosseo_RivalPathC:
	.incbin "MAP/KOROSSEO/RIVAL_PATH_C.table.lz"
	.byte 0x8d, 0x8f	@ PACKER: left in the buffer by WorldMap_TilesE+0x1a8e
