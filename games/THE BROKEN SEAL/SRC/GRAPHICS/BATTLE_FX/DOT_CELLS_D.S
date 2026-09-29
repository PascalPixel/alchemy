@ BattleFx_ParticleSpritesD: 10 pictures (listed in DOT_CELLS_D.TSV) with no palette of their
@ own, drawn with BattleFx_BlueBeamSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_ParticleSpritesD
BattleFx_ParticleSpritesD:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_D.parts.lz"
