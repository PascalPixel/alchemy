	.section .rodata
	.global Battle_FuneHeyaBackdrop
Battle_FuneHeyaBackdrop:
	.incbin "games/common/src/graphics/backdrop/tolbi_ship_interiors.gbapal"
	.incbin "games/common/src/graphics/backdrop/tolbi_ship_interiors.bitmap.d7"
	.byte 0x39, 0x66	@ PACKER: left in the buffer by Battle_MogoruMoriBackdrop+0x4076
