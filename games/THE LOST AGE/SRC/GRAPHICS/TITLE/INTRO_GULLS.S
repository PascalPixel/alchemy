@ Title_IntroGullSprites: the intro's seagull flying away, its 16-colour
@ palette, then 872 tiles of 4-bit colour, frames 64 pixels wide, in the byte
@ LZ the resource loader decodes.
	.section .rodata
	.global Title_IntroGullSprites
Title_IntroGullSprites:
	.incbin "GRAPHICS/TITLE/INTRO_GULLS.gbapal"
	.incbin "GRAPHICS/TITLE/INTRO_GULLS.4bpp.plz"
