	.section .rodata
	.global Korosseo_RivalPathB
Korosseo_RivalPathB:
	.incbin "MAP/KOROSSEO/RIVAL_PATH_B.table.lz"
	.byte 0x33, 0x59	@ PACKER: left in the buffer by Korosseo_RivalPathA+0x15c6
