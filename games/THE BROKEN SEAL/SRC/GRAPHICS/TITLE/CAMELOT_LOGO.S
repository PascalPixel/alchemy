@ Title_CamelotLogo: a 224-colour palette, a 32x32 tilemap and 320 tiles of 8-bit
@ colour (the parts in CAMELOT_LOGO.TSV), joined and packed by the resource packer.
@ Title_ShowAnimatedSplash copies the first 256 tiles (16KB) to the BG, the
@ picture the tilemap shows, then every eight frames one of four 1KB (16-tile)
@ frames of the castle flags over tiles 4 on, the frames drawn as that map shows
@ them.
	.section .rodata
	.global Title_CamelotLogo
Title_CamelotLogo:
	.ifdef TBS_EDITION_JA
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO_JA.parts.lz"
	.else
	.incbin "GRAPHICS/TITLE/CAMELOT_LOGO.parts.lz"
	.endif
