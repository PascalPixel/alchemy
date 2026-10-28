	.section .rodata
	.global Battle_AltinPeakBackdrop
Battle_AltinPeakBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/altin_peak.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/altin_peak.bitmap.d7"
	.byte 0x13, 0x24	@ PACKER: left in the buffer by Battle_TretTreeInteriorsBackdrop+0x3b4a
