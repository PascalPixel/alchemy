	.section .rodata
	.global BattleFx_RevealMaskA
BattleFx_RevealMaskA:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_A.bitmap.lz"
	.byte 0x0a, 0x21	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x6c2
