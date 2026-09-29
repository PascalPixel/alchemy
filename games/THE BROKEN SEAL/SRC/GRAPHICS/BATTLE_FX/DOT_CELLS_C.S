@ BattleFx_ParticleSpritesC: 10 pictures (listed in DOT_CELLS_C.TSV) with no palette of their
@ own, drawn with BattleFx_LightFanSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_ParticleSpritesC
BattleFx_ParticleSpritesC:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_C.parts.lz"
