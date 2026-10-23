	.section .rodata
	.global Battle_SuhallaGateBackdrop
Battle_SuhallaGateBackdrop:
	.incbin "games/tbs/src/graphics/battle_bg/suhalla_gate.gbapal"
	.incbin "games/tbs/src/graphics/battle_bg/suhalla_gate.bitmap.d7"
	.byte 0x41, 0x83	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x4442
