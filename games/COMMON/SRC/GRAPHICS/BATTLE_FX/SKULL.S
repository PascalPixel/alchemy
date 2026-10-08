	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_SkullSheet
BattleFx_SkullSheet:
	.incbin "GRAPHICS/BATTLE_FX/SKULL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SKULL.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x98, 0x43, 0x4a	@ PACKER: left in the buffer by BattleFx_SheepSheet+0x421
	.endif
