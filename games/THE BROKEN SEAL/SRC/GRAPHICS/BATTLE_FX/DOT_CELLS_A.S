@ BattleFx_ParticleSpritesA: 10 pictures (listed in DOT_CELLS_A.TSV) with no palette of their
@ own, drawn with BattleFx_OrangePaletteA's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_ParticleSpritesA
BattleFx_ParticleSpritesA:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_A.parts.lz"
