	.section .rodata
	.global BattleEffect_FlareImage
BattleEffect_FlareImage:
	.incbin "games/tla/src/graphics/battle_effects/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x277
