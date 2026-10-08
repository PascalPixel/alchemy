	.section .rodata
	.balign 4
@ 枠
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.4byte UiIcon_Frame0, UiIcon_Frame1, UiIcon_Frame2
	.4byte -1
UiIcon_Frame0:
	.incbin "GRAPHICS/UI/MARKS/FRAME_0.icon4"
UiIcon_Frame1:
	.incbin "GRAPHICS/UI/MARKS/FRAME_1.icon4"
UiIcon_Frame2:
	.incbin "GRAPHICS/UI/MARKS/FRAME_2.icon4"
	.balign 4, 0

@ 装備, ×, Φ の印
	.global UiIcon_MarkPointers
UiIcon_MarkPointers:
	.4byte UiIcon_MarkEquip, UiIcon_MarkCross, UiIcon_MarkPhi
UiIcon_MarkEquip:
	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/UI/MARKS/EQUIP_DE.icon4"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/UI/MARKS/EQUIP_DE.icon4"
	.else
	.incbin "GRAPHICS/UI/MARKS/EQUIP.icon4"
	.endif
	.endif
UiIcon_MarkCross:
	.incbin "GRAPHICS/UI/MARKS/CROSS.icon4"
UiIcon_MarkPhi:
	.incbin "GRAPHICS/UI/MARKS/PHI.icon4"
	.balign 4, 0

@ 数字 0-9, 十の位 1-3
	.global UiIcon_DigitPointers
UiIcon_DigitPointers:
	.4byte UiIcon_Digit0, UiIcon_Digit1, UiIcon_Digit2, UiIcon_Digit3, UiIcon_Digit4
	.4byte UiIcon_Digit5, UiIcon_Digit6, UiIcon_Digit7, UiIcon_Digit8, UiIcon_Digit9
	.4byte UiIcon_Tens1, UiIcon_Tens2, UiIcon_Tens3
	.4byte -1
UiIcon_Digit0:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_0.icon4"
UiIcon_Digit1:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_1.icon4"
UiIcon_Digit2:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_2.icon4"
UiIcon_Digit3:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_3.icon4"
UiIcon_Digit4:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_4.icon4"
UiIcon_Digit5:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_5.icon4"
UiIcon_Digit6:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_6.icon4"
UiIcon_Digit7:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_7.icon4"
UiIcon_Digit8:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_8.icon4"
UiIcon_Digit9:
	.incbin "GRAPHICS/UI/MARKS/DIGIT_9.icon4"
UiIcon_Tens1:
	.incbin "GRAPHICS/UI/MARKS/TENS_1.icon4"
UiIcon_Tens2:
	.incbin "GRAPHICS/UI/MARKS/TENS_2.icon4"
UiIcon_Tens3:
	.incbin "GRAPHICS/UI/MARKS/TENS_3.icon4"
	.balign 4, 0

	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.rept 43
	.4byte UiIcon_Overlay
	.endr
UiIcon_Overlay:
	.incbin "GRAPHICS/UI/MARKS/OVERLAY.icon4"
	.balign 4, 0
