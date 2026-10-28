	.section .rodata
	.global BattleEffect_TwelveModeImage
BattleEffect_TwelveModeImage:
	.incbin "games/tla/src/graphics/battle_effects/twelve_mode.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x42f
