	.section .rodata
	.global BattleFx_SwordSlashSheet
BattleFx_SwordSlashSheet:
	.incbin "graphics/battle_fx/sword_slash_00.gbapal"
	.incbin "graphics/battle_fx/sword_slash.parts.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleFx_ParticleSpritesC+0x4c7
