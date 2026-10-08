	.section .rodata
	.global BattleFx_VioletLightningSheetB
BattleFx_VioletLightningSheetB:
	.incbin "GRAPHICS/BATTLE_FX/BOLT_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOLT_B.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_FlareSheet at +0x1bd3 */
	.byte 0xe5
