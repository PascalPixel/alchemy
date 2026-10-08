	.section .rodata
	.global BattleFx_FlameSheetA
BattleFx_FlameSheetA:
	.incbin "GRAPHICS/BATTLE_FX/FLAME_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLAME_A.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleFx_RainSheet+0x4a2
