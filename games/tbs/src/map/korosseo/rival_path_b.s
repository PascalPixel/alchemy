	.section .rodata
	.global Korosseo_RivalPathB
Korosseo_RivalPathB:
	.incbin "map/korosseo/rival_path_b.table.lz"
	.byte 0x33, 0x59	@ PACKER: left in the buffer by Korosseo_RivalPathA+0x15c6
