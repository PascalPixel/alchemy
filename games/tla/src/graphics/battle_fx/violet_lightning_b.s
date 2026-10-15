	.section .rodata
	.global BattleFx_VioletLightningSheetB
BattleFx_VioletLightningSheetB:
	.incbin "games/tla/src/graphics/battle_fx/violet_lightning_b.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/violet_lightning_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x1bd3
