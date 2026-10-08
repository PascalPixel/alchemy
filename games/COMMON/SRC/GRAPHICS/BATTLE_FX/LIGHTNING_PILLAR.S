	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_LightningPillarSheet
BattleFx_LightningPillarSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_PILLAR.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_PILLAR.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: the packer rounded the stream to 4 with 0xff, not with an earlier stream's bytes */
	.balign 4, 0xff
	.endif
