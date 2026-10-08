	.section .rodata
	.global BattleFx_FirePillarSheetB
BattleFx_FirePillarSheetB:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_B.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_BlastSheet at +0x11fe */
	.byte 0x0d, 0x03
