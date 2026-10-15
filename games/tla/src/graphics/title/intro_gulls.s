	.section .rodata
	.global Title_IntroGullSprites
Title_IntroGullSprites:
	.incbin "games/tla/src/graphics/title/intro_gulls.gbapal"
	.incbin "games/tla/src/graphics/title/intro_gulls.4bpp.plz"
	.byte 0x19, 0x2f, 0x63	@ PACKER: left in the buffer by Title_IntroLighthousePicture+0x11ed
