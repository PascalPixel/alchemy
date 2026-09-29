@ Battle_TakaraShimaCaveBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_TakaraShimaCaveBackdrop
Battle_TakaraShimaCaveBackdrop:
	.incbin "GRAPHICS/BACKDROP/TAKARA_SHIMA_CAVE.gbapal"
	.incbin "GRAPHICS/BACKDROP/TAKARA_SHIMA_CAVE.bitmap.d7"
