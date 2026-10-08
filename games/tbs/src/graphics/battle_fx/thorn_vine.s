	.section .rodata
	.global BattleFx_ThornVineSheet
BattleFx_ThornVineSheet:
	.incbin "graphics/battle_fx/thorn_vine_00.gbapal"
	.incbin "graphics/battle_fx/thorn_vine.parts.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0x691
