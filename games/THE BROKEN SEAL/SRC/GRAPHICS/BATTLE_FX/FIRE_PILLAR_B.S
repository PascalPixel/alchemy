	.section .rodata
	.global BattleFx_FirePillarSheetB
BattleFx_FirePillarSheetB:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_B.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleFx_BlastSheet+0x11fe
