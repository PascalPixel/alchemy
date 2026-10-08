	.section .rodata
	.global BattleFx_FlameBladeSheet
BattleFx_FlameBladeSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_BLD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_BLD.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CyanSparkSheet at +0xc83 */
	.byte 0xb7
