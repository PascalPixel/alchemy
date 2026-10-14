	.section .rodata
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "games/common/src/graphics/title/intro_peak.gbapal"
	.incbin "games/common/src/graphics/title/intro_peak.parts.plz"
	.balign 4, 0
