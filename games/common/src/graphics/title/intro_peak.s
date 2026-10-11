	.section .rodata
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "graphics/title/intro_peak.gbapal"
	.incbin "graphics/title/intro_peak.parts.plz"
	.balign 4, 0
