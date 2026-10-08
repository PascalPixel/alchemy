	.section .rodata
	.global BattleFx_FlameSheetA
BattleFx_FlameSheetA:
	.incbin "graphics/battle_fx/flame_a.gbapal"
	.incbin "graphics/battle_fx/flame_a.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleFx_RainSheet+0x4a2
