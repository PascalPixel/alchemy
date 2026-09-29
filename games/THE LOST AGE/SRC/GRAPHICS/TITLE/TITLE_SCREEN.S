@ Title_ScreenPicture: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Title_ScreenPicture
Title_ScreenPicture:
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN.gbapal"
	.incbin "GRAPHICS/TITLE/TITLE_SCREEN.8bpp.lz"
