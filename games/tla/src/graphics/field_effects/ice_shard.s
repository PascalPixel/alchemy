	.section .rodata
	.global FieldEffect_IceShard
FieldEffect_IceShard:
	.incbin "games/tla/src/graphics/field_effects/ice_shard.8bpp.lz"
	.byte 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
