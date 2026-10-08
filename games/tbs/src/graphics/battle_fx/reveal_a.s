	.section .rodata
	.global BattleFx_RevealMaskA
BattleFx_RevealMaskA:
	.incbin "graphics/battle_fx/reveal_a.gbapal"
	.incbin "graphics/battle_fx/reveal_a.bitmap.lz"
	.byte 0x0a, 0x21	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x6c2
