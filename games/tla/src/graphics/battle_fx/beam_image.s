	.section .rodata
	.global BattleEffect_BeamImage
BattleEffect_BeamImage:
	.incbin "games/tla/src/graphics/battle_fx/beam_image.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x37b
