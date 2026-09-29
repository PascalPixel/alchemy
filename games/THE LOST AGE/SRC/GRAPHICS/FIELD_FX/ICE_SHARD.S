@ FieldFx_IceShard: a turning shard of ice, 16 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_IceShard
FieldFx_IceShard:
	.incbin "GRAPHICS/FIELD_FX/ICE_SHARD.8bpp.lz"
