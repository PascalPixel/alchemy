	.section .rodata
	.global BattleFx_FlameBladeSheet
BattleFx_FlameBladeSheet:
	.incbin "graphics/battle_fx/flame_bld.gbapal"
	.incbin "graphics/battle_fx/flame_bld.bitmap.lz"
	.byte 0xb7	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0xc83
