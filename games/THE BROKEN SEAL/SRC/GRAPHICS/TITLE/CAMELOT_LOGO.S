@ Title_CamelotLogo: a 224-colour palette, a 32x32 tilemap and 320 tiles of 8-bit
@ colour (the parts in CAMELOT_LOGO.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Title_CamelotLogo
Title_CamelotLogo:
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO.parts.lz"
