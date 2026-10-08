	.section .rodata
	.global BattleFx_BeamSequenceImage
BattleFx_BeamSequenceImage:
	.incbin "GRAPHICS/BATTLE_FX/BEAM_SEQUENCE.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_LightningBoltSheet at +0x37b */
	.byte 0x5f
