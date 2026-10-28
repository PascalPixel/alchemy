	.section .rodata
	.global BattleEffect_DustCloudSheet
BattleEffect_DustCloudSheet:
	.incbin "games/tla/src/graphics/battle_effects/dust_cloud.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dust_cloud.bitmap.lz"
	.byte 0x9a, 0xcb	@ PACKER: left in the buffer by Resource_Data0F5+0x4ee
