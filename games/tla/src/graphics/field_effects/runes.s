	.section .rodata
	.global FieldEffect_Runes
FieldEffect_Runes:
	.incbin "games/tla/src/graphics/field_effects/runes.8bpp.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
