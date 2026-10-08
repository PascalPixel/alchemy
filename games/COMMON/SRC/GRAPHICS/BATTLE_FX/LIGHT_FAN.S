	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_LightFanSheet
BattleFx_LightFanSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIGHT_FAN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIGHT_FAN.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_IceBlockSheet at +0x59d */
	.byte 0x09, 0x82, 0x76
	.endif
