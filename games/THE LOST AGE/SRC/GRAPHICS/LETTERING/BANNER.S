@ Lettering_Banner: 16-colour palettes, then 4-bit tiles for the large orange
@ banner words and kana a contest shows, the first palette drawn (the parts
@ in BANNER.TSV), joined and packed by the resource packer. The Japanese and
@ English banks carry five palettes and 256 tiles; the European ones six
@ palettes and their own words, the French in 240 tiles.
	.section .rodata
	.global Lettering_Banner
Lettering_Banner:
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/LETTERING/BANNER_DE.parts.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/LETTERING/BANNER_ES.parts.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/LETTERING/BANNER_FR.parts.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/LETTERING/BANNER_IT.parts.lz"
	.else
	.incbin "GRAPHICS/LETTERING/BANNER.parts.lz"
	.endif
	.endif
	.endif
	.endif
