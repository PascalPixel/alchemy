	.section .rodata
	.global BattleFx_BeamSequenceImage
BattleFx_BeamSequenceImage:
	.incbin "GRAPHICS/BATTLE_FX/BEAM_SEQUENCE.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleFx_LightningBoltSheet+0x37b
