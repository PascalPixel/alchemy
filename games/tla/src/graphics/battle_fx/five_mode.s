	.section .rodata
	.global BattleEffect_FiveModeImage
BattleEffect_FiveModeImage:
	.incbin "games/tla/src/graphics/battle_fx/five_mode.bitmap.lz"
	.byte 0x96	@ PACKER: left in the buffer by BattleEffect_IceTileSheet+0xfc7
