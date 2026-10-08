	.section .rodata
	.global BattleFx_ThornVineSheet
BattleFx_ThornVineSheet:
	.incbin "GRAPHICS/BATTLE_FX/THORN_VINE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/THORN_VINE.parts.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0x691
