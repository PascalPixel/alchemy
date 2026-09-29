@ BattleFx_ParticleSpritesB: 10 pictures (listed in DOT_CELLS_B.TSV) with no palette of their
@ own, drawn with BattleFx_CrystalSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_ParticleSpritesB
BattleFx_ParticleSpritesB:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_B.parts.lz"
