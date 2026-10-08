	.section .rodata
	.global BattleFx_FlameBladeSheet
BattleFx_FlameBladeSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_BLD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_BLD.bitmap.lz"
	.byte 0xb7	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0xc83
