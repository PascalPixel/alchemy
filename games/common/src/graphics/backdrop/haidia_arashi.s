	.section .rodata
	.global Battle_HaidiaArashiBackdrop
Battle_HaidiaArashiBackdrop:
	.incbin "games/common/src/graphics/backdrop/haidia_arashi.gbapal"
	.incbin "games/common/src/graphics/backdrop/haidia_arashi.bitmap.d7"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
