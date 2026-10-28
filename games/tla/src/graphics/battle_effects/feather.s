	.section .rodata
	.global BattleEffect_FeatherSheet
BattleEffect_FeatherSheet:
	.incbin "games/tla/src/graphics/battle_effects/feather.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/feather.bitmap.lz"
	.byte 0xda, 0x25, 0x56	@ PACKER: left in the buffer by BattleEffect_CloudSwirlSheet+0x789
