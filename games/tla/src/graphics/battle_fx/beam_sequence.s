	.section .rodata
	.global BattleEffect_BeamSequenceImage
BattleEffect_BeamSequenceImage:
	.incbin "games/tla/src/graphics/battle_fx/beam_sequence.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x37b
