@ BattleFx_BeamSequenceImage: 6 pictures (listed in BEAM_SEQUENCE.TSV) with no palette of their
@ own, drawn with BattleFx_LightningBoltSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_BeamSequenceImage
BattleFx_BeamSequenceImage:
	.incbin "GRAPHICS/BATTLE_FX/BEAM_SEQUENCE.parts.lz"
