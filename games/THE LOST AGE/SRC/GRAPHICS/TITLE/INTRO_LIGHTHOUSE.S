@ Title_IntroLighthousePicture: the intro's view of the lighthouse from the
@ mountains, its 256-colour palette, then 1,440 tiles of 8-bit colour in the
@ byte LZ the resource loader decodes.
	.section .rodata
	.global Title_IntroLighthousePicture
Title_IntroLighthousePicture:
	.incbin "GRAPHICS/TITLE/INTRO_LIGHTHOUSE.gbapal"
	.incbin "GRAPHICS/TITLE/INTRO_LIGHTHOUSE.8bpp.plz"
	.balign 4, 0
