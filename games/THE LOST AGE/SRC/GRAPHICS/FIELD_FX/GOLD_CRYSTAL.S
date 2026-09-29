@ FieldFx_GoldCrystal: an upright golden crystal, 4 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_GoldCrystal
FieldFx_GoldCrystal:
	.incbin "GRAPHICS/FIELD_FX/GOLD_CRYSTAL.8bpp.lz"
