	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_YellowSparkSheet
BattleFx_YellowSparkSheet:
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_SPARK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_SPARK.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_VioletLightningSheetA at +0x2fd */
	.byte 0x17, 0x6e, 0xa0
	.endif
