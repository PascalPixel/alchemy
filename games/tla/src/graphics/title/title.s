	.section .rodata
	.global Title_IntroLighthousePicture
Title_IntroLighthousePicture:
	.incbin "games/tla/src/graphics/title/intro_lighthouse.gbapal"
	.incbin "games/tla/src/graphics/title/intro_lighthouse.8bpp.plz"
	.balign 4, 0
	.global Title_IntroGullSprites
Title_IntroGullSprites:
	.incbin "games/tla/src/graphics/title/intro_gulls.gbapal"
	.incbin "games/tla/src/graphics/title/intro_gulls.4bpp.plz"
	.byte 0x19, 0x2f, 0x63	@ PACKER: left in the buffer by Title_IntroLighthousePicture+0x11ed
	.section .rodata.a
	.global Title_NintendoLogo
Title_NintendoLogo:
	.incbin "games/tla/src/graphics/title/nintendo_splash.gbapal"
	.incbin "games/tla/src/graphics/title/nintendo_splash.8bpp.lz"
@ The "CAMELOT SOFTWARE PLANNING" logo and four flag frames.
	.global Title_CamelotLogo
Title_CamelotLogo:
	.incbin "games/tla/src/graphics/title/camelot_logo.parts.lz"
	.section .rodata.b
	.global Resource_Data023
Resource_Data023:
	.incbin "games/tla/src/graphics/title/picture_023.strips"
	.byte 0x8f	@ PACKER: left in the buffer by Title_IntroLighthousePicture+0x761b
	.global Resource_Data024
Resource_Data024:
	.incbin "games/tla/src/graphics/title/picture_024.strips"
@ A copy of The Broken Seal's Title_IntroTilesB, the "To be continued" lettering.
	.global Resource_Data025
Resource_Data025:
	.incbin "games/tla/src/graphics/title/to_be_continued.parts.lz"
