	.section .rodata
	.global BattleFx_FlameBladeSheet
BattleFx_FlameBladeSheet:
	.incbin "games/tla/src/graphics/battle_fx/flame_blade.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/flame_blade.bitmap.lz"
	.byte 0xff	@ PACKER: left in the buffer by BattleFx_GreyCrescentSheet+0xc83
