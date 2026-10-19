	.section .rodata
	.global BattleFx_BeamSequenceImage
BattleFx_BeamSequenceImage:
	.incbin "games/tla/src/graphics/battle_fx/beam_sequence.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleFx_LightningBoltSheet+0x37b
