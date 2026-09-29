@ Korosseo_Lettering: five 16-colour palettes, then eight 4-bit pictures of Korosseo
@ lettering (the parts in LETTERING.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Korosseo_Lettering
Korosseo_Lettering:
	.incbin "GRAPHICS/KOROSSEO/LETTERING.parts.lz"
