	.section .rodata
	.global Battle_AltmillerCaveBackdrop
Battle_AltmillerCaveBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/altmiller_cave.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/altmiller_cave.bitmap.d7"
	.byte 0x41, 0x40	@ PACKER: left in the buffer by Battle_ColossoBackdrop+0x40d6
