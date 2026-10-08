	.section .rodata
	.global Korosseo_RivalPathC
Korosseo_RivalPathC:
	.incbin "map/korosseo/rival_path_c.table.lz"
	.byte 0x8d, 0x8f	@ PACKER: left in the buffer by WorldMap_TilesE+0x1a8e
