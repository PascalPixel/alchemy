	.section .rodata
	.global BattleFx_FirePillarSheetB
BattleFx_FirePillarSheetB:
	.incbin "graphics/battle_fx/fire_pillar_b.gbapal"
	.incbin "graphics/battle_fx/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleFx_BlastSheet+0x11fe
