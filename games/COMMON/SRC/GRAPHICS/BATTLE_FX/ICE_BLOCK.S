	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_BLOCK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_BLOCK.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_BlueBeastSheet at +0x1586 */
	.byte 0x30, 0xae
	.endif
