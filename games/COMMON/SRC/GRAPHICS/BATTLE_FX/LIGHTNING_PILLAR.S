	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_LightningPillarSheet
BattleFx_LightningPillarSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_PILLAR.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_PILLAR.bitmap.lz"
	.ifeq GAME_TLA
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.endif
