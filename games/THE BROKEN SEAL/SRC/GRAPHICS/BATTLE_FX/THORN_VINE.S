	.section .rodata
	.global BattleFx_ThornVineSheet
BattleFx_ThornVineSheet:
	.incbin "GRAPHICS/BATTLE_FX/THORN_VINE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/THORN_VINE.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_VioletStreakSheet at +0x691 */
	.byte 0x1e, 0x99, 0x03
