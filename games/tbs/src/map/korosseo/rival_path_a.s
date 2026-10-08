	.section .rodata
	.global Korosseo_RivalPathA
Korosseo_RivalPathA:
	.incbin "map/korosseo/rival_path_a.table.lz"
	.byte 0xe0, 0xc8, 0xb4	@ PACKER: left in the buffer by WorldMap_TilesE+0x1899
