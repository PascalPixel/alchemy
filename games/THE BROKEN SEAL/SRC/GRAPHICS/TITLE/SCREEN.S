@ Title_IntroGraphicsB: the title screen, its 256-colour palette, then 600
@ tiles of 8-bit colour in the byte LZ the resource loader decodes.
	.section .rodata
	.global Title_IntroGraphicsB
Title_IntroGraphicsB:
	.incbin "GRAPHICS/TITLE/SCREEN.gbapal"
	.incbin "GRAPHICS/TITLE/SCREEN.8bpp.plz"
