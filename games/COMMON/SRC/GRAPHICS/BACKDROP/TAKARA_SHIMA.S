@ Battle_TakaraShimaBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_TakaraShimaBackdrop
Battle_TakaraShimaBackdrop:
	.incbin "GRAPHICS/BACKDROP/TAKARA_SHIMA.gbapal"
	.incbin "GRAPHICS/BACKDROP/TAKARA_SHIMA.bitmap.d7"
