@ Korosseo_Lettering: five 16-colour palettes, then eight 4-bit pictures of Korosseo
@ lettering (the parts in LETTERING.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Korosseo_Lettering
Korosseo_Lettering:
	.ifdef TBS_EDITION_JA
	.incbin "GRAPHICS/KOROSSEO/LETTERING_JA.parts.lz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/KOROSSEO/LETTERING_DE.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/KOROSSEO/LETTERING_ES.parts.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/KOROSSEO/LETTERING_FR.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/KOROSSEO/LETTERING_IT.parts.lz"
	.else
	.incbin "GRAPHICS/KOROSSEO/LETTERING.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
