	.section .rodata
	.global BattleEffect_VioletLightningSheetB
BattleEffect_VioletLightningSheetB:
	.incbin "games/tla/src/graphics/battle_effects/violet_lightning_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/violet_lightning_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x1bd3
