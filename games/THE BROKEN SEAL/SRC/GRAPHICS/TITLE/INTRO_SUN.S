@ Title_IntroGraphicsA: the intro's sun glow and ring pieces, its 256-colour
@ palette, then 120 tiles of 8-bit colour (the pictures INTRO_SUN.TSV lists) in the
@ byte LZ the resource loader decodes.
	.section .rodata
	.global Title_IntroGraphicsA
Title_IntroGraphicsA:
	.incbin "GRAPHICS/TITLE/INTRO_SUN_1.gbapal"
	.incbin "GRAPHICS/TITLE/INTRO_SUN.parts.plz"
