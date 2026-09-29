@ Title_LogoOnBlackPicture: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Title_LogoOnBlackPicture
Title_LogoOnBlackPicture:
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK.gbapal"
	.incbin "GRAPHICS/TITLE/LOGO_ON_BLACK.8bpp.lz"
