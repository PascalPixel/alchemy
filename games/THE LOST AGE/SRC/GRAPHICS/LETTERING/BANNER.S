@ Lettering_Banner: five 16-colour palettes, then 256 tiles of 4-bit colour
@ for the large orange banner words and kana a contest shows, the first
@ palette drawn (the parts in BANNER.TSV), joined and packed by the resource
@ packer.
	.section .rodata
	.global Lettering_Banner
Lettering_Banner:
	.incbin "GRAPHICS/LETTERING/BANNER.parts.lz"
