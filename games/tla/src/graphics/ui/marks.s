	.include "games/tla/include/edition.inc"
	.section .rodata
	.balign 4
@ The three icon bases
	.global Icon_BasePointerTable
Icon_BasePointerTable:
	.4byte Icon_CommandFrame, Icon_CommandFrameRed, Icon_ItemFrame
	.4byte -1
Icon_CommandFrame:
	.incbin "games/tla/src/graphics/ui/marks/command_frame.icon4"
Icon_CommandFrameRed:
	.incbin "games/tla/src/graphics/ui/marks/command_frame_red.icon4"
Icon_ItemFrame:
	.incbin "games/tla/src/graphics/ui/marks/item_frame.icon4"
	.balign 4, 0

@ The equipped, cross and phi marks
	.global Icon_MarkPointers
Icon_MarkPointers:
	.4byte Icon_MarkEquip, Icon_MarkCross, Icon_MarkCursed
Icon_MarkEquip:
	.ifdef TBS_EDITION_DE
	.incbin "games/tla/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tla/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/ui/marks/equip_de.icon4"
	.else
	.incbin "games/tla/src/graphics/ui/marks/equip.icon4"
	.endif
	.endif
	.endif
	.endif
Icon_MarkCross:
	.incbin "games/tla/src/graphics/ui/marks/cross.icon4"
Icon_MarkCursed:
	.incbin "games/tla/src/graphics/ui/marks/phi.icon4"
	.balign 4, 0

@ The digits 0 to 9, then the tens 1 to 3
	.global Icon_DigitPointers
Icon_DigitPointers:
	.4byte Icon_Digit0, Icon_Digit1, Icon_Digit2, Icon_Digit3, Icon_Digit4
	.4byte Icon_Digit5, Icon_Digit6, Icon_Digit7, Icon_Digit8, Icon_Digit9
	.4byte Icon_Tens1, Icon_Tens2, Icon_Tens3
	.4byte -1
Icon_Digit0:
	.incbin "games/tla/src/graphics/ui/marks/digit_0.icon4"
Icon_Digit1:
	.incbin "games/tla/src/graphics/ui/marks/digit_1.icon4"
Icon_Digit2:
	.incbin "games/tla/src/graphics/ui/marks/digit_2.icon4"
Icon_Digit3:
	.incbin "games/tla/src/graphics/ui/marks/digit_3.icon4"
Icon_Digit4:
	.incbin "games/tla/src/graphics/ui/marks/digit_4.icon4"
Icon_Digit5:
	.incbin "games/tla/src/graphics/ui/marks/digit_5.icon4"
Icon_Digit6:
	.incbin "games/tla/src/graphics/ui/marks/digit_6.icon4"
Icon_Digit7:
	.incbin "games/tla/src/graphics/ui/marks/digit_7.icon4"
Icon_Digit8:
	.incbin "games/tla/src/graphics/ui/marks/digit_8.icon4"
Icon_Digit9:
	.incbin "games/tla/src/graphics/ui/marks/digit_9.icon4"
Icon_Tens1:
	.incbin "games/tla/src/graphics/ui/marks/tens_1.icon4"
Icon_Tens2:
	.incbin "games/tla/src/graphics/ui/marks/tens_2.icon4"
Icon_Tens3:
	.incbin "games/tla/src/graphics/ui/marks/tens_3.icon4"
	.balign 4, 0

	.global Icon_OverlayPointerTable
Icon_OverlayPointerTable:
	.rept 43
	.4byte Icon_Overlay
	.endr
Icon_Overlay:
	.incbin "games/tla/src/graphics/ui/marks/overlay.icon4"
	.balign 4, 0
