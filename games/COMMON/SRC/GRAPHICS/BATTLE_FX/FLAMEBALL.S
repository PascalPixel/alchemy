	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlameballSheet
BattleFx_FlameballSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAMEBALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAMEBALL.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x83	@ PACKER: left in the buffer by BattleFx_FirePillarSheetA+0x557
	.endif
