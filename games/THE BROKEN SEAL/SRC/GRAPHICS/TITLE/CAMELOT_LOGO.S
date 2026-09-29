@ Title_CamelotLogo: a 224-colour palette, a 32x32 tilemap and 320 tiles of 8-bit
@ colour (the parts in CAMELOT_LOGO.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Title_CamelotLogo
Title_CamelotLogo:
	.ifdef TBS_EDITION_JA
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO_JA.parts.lz"
	.else
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO.parts.lz"
	.endif
