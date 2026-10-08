	.section .rodata
	.global BattleFx_VioletLightningSheetB
BattleFx_VioletLightningSheetB:
	.incbin "graphics/battle_fx/bolt_b.gbapal"
	.incbin "graphics/battle_fx/bolt_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x1bd3
