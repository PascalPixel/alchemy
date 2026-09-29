@ Title_NintendoLogo: the Nintendo splash, its 224-colour palette then its 600
@ tiles of 8-bit colour, 30 tiles wide, in palette LZ.
	.section .rodata
	.global Title_NintendoLogo
Title_NintendoLogo:
	.incbin "GRAPHICS/TITLE/NINTENDO_SPLASH.gbapal"
	.incbin "GRAPHICS/TITLE/NINTENDO_SPLASH.8bpp.lz"
