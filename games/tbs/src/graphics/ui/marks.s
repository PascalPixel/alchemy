	.include "games/tbs/include/edition.inc"
	.section .rodata
	.balign 4
@ The three icon bases
	.global IconBasePointerTable
IconBasePointerTable:
	.4byte IconCommandFrame, IconCommandFrameRed, IconItemFrame
	.4byte -1
IconCommandFrame:
	.incbin "games/tbs/src/graphics/ui/marks/command_frame.icon4"
IconCommandFrameRed:
	.incbin "games/tbs/src/graphics/ui/marks/command_frame_red.icon4"
IconItemFrame:
	.incbin "games/tbs/src/graphics/ui/marks/item_frame.icon4"
	.balign 4, 0

@ The equipped, cross and phi marks
	.global IconMarkPointers
IconMarkPointers:
	.4byte IconMarkEquip, IconMarkCross, IconMarkCursed
IconMarkEquip:
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.incbin "games/tbs/src/graphics/ui/marks/equip.icon4"
	.endif
	.endif
IconMarkCross:
	.incbin "games/tbs/src/graphics/ui/marks/cross.icon4"
IconMarkCursed:
	.incbin "games/tbs/src/graphics/ui/marks/cursed.icon4"
	.balign 4, 0

@ The digits 0 to 9, then the tens 1 to 3
	.global IconDigitPointers
IconDigitPointers:
	.4byte IconDigit0, IconDigit1, IconDigit2, IconDigit3, IconDigit4
	.4byte IconDigit5, IconDigit6, IconDigit7, IconDigit8, IconDigit9
	.4byte IconTens1, IconTens2, IconTens3
	.4byte -1
IconDigit0:
	.incbin "games/tbs/src/graphics/ui/marks/digit_0.icon4"
IconDigit1:
	.incbin "games/tbs/src/graphics/ui/marks/digit_1.icon4"
IconDigit2:
	.incbin "games/tbs/src/graphics/ui/marks/digit_2.icon4"
IconDigit3:
	.incbin "games/tbs/src/graphics/ui/marks/digit_3.icon4"
IconDigit4:
	.incbin "games/tbs/src/graphics/ui/marks/digit_4.icon4"
IconDigit5:
	.incbin "games/tbs/src/graphics/ui/marks/digit_5.icon4"
IconDigit6:
	.incbin "games/tbs/src/graphics/ui/marks/digit_6.icon4"
IconDigit7:
	.incbin "games/tbs/src/graphics/ui/marks/digit_7.icon4"
IconDigit8:
	.incbin "games/tbs/src/graphics/ui/marks/digit_8.icon4"
IconDigit9:
	.incbin "games/tbs/src/graphics/ui/marks/digit_9.icon4"
IconTens1:
	.incbin "games/tbs/src/graphics/ui/marks/tens_1.icon4"
IconTens2:
	.incbin "games/tbs/src/graphics/ui/marks/tens_2.icon4"
IconTens3:
	.incbin "games/tbs/src/graphics/ui/marks/tens_3.icon4"
	.balign 4, 0

	.global IconOverlayPointerTable
IconOverlayPointerTable:
	.rept 43
	.4byte IconOverlay
	.endr
IconOverlay:
	.incbin "games/tbs/src/graphics/ui/marks/overlay.icon4"
	.balign 4, 0
