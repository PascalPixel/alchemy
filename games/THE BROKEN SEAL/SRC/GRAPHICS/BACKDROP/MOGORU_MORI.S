@ Battle_MogoruMoriBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_MogoruMoriBackdrop
Battle_MogoruMoriBackdrop:
	.incbin "GRAPHICS/BACKDROP/MOGORU_MORI.gbapal"
	.incbin "GRAPHICS/BACKDROP/MOGORU_MORI.bitmap.d7"
