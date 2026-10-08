	.section .rodata
	.global BattleFx_SwordSlashSheet
BattleFx_SwordSlashSheet:
	.incbin "GRAPHICS/BATTLE_FX/SWORD_SLASH_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SWORD_SLASH.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_ParticleSpritesC at +0x4c7 */
	.byte 0x02
