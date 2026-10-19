	.section .rodata
	.global Battle_AltinPeakBackdrop
Battle_AltinPeakBackdrop:
	.incbin "games/tbs/src/graphics/backdrop/altin_peak.gbapal"
	.incbin "games/tbs/src/graphics/backdrop/altin_peak.bitmap.d7"
	.byte 0x54, 0x14	@ PACKER: left in the buffer by Battle_GomaCaveBackdrop+0x3b4a
