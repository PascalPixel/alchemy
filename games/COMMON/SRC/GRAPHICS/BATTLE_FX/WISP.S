	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_WispSheet
BattleFx_WispSheet:
	.incbin "GRAPHICS/BATTLE_FX/WISP.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WISP.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleFx_SmokeSheet+0x7ae
	.endif
