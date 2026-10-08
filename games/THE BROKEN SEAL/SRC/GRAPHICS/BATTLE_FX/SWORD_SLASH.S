	.section .rodata
	.global BattleFx_SwordSlashSheet
BattleFx_SwordSlashSheet:
	.incbin "GRAPHICS/BATTLE_FX/SWORD_SLASH_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SWORD_SLASH.parts.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleFx_ParticleSpritesC+0x4c7
