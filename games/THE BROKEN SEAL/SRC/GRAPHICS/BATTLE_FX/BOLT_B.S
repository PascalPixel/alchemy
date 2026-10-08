	.section .rodata
	.global BattleFx_VioletLightningSheetB
BattleFx_VioletLightningSheetB:
	.incbin "GRAPHICS/BATTLE_FX/BOLT_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOLT_B.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x1bd3
