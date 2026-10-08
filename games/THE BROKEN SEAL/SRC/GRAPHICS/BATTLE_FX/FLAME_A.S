	.section .rodata
	.global BattleFx_FlameSheetA
BattleFx_FlameSheetA:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_A.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_RainSheet at +0x4a2 */
	.byte 0x2f, 0x18
