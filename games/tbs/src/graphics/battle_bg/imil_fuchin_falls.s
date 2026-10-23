	.section .rodata
	.global Battle_ImilFuchinFallsBackdrop
Battle_ImilFuchinFallsBackdrop:
	.incbin "games/tbs/src/graphics/battle_bg/imil_fuchin_falls.gbapal"
	.incbin "games/tbs/src/graphics/battle_bg/imil_fuchin_falls.bitmap.d7"
	.byte 0x45, 0x15	@ PACKER: left in the buffer by Battle_TolbiShipInteriorsBackdrop+0x383e
