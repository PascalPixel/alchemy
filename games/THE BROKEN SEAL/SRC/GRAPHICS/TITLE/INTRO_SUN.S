@ Title_IntroGraphicsA: the intro's sun glow and ring pieces, its 256-colour
@ palette, then 120 tiles of 8-bit colour in the byte LZ the resource loader
@ decodes.
	.section .rodata
	.global Title_IntroGraphicsA
Title_IntroGraphicsA:
	.incbin "GRAPHICS/TITLE/INTRO_SUN.gbapal"
	.incbin "GRAPHICS/TITLE/INTRO_SUN.8bpp.plz"
