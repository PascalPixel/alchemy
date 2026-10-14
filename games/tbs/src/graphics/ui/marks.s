	.section .rodata
	.balign 4
@ 枠
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.4byte UiIcon_Frame0, UiIcon_Frame1, UiIcon_Frame2
	.4byte -1
UiIcon_Frame0:
	.incbin "games/tbs/src/graphics/ui/marks/frame_0.icon4"
UiIcon_Frame1:
	.incbin "games/tbs/src/graphics/ui/marks/frame_1.icon4"
UiIcon_Frame2:
	.incbin "games/tbs/src/graphics/ui/marks/frame_2.icon4"
	.balign 4, 0

@ 装備, ×, Φ の印
	.global UiIcon_MarkPointers
UiIcon_MarkPointers:
	.4byte UiIcon_MarkEquip, UiIcon_MarkCross, UiIcon_MarkPhi
UiIcon_MarkEquip:
	.include "games/common/include/game/ed_asm.h"
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.incbin "games/tbs/src/graphics/ui/marks/equip.icon4"
	.endif
	.endif
UiIcon_MarkCross:
	.incbin "games/tbs/src/graphics/ui/marks/cross.icon4"
UiIcon_MarkPhi:
	.incbin "games/tbs/src/graphics/ui/marks/phi.icon4"
	.balign 4, 0

@ 数字 0-9, 十の位 1-3
	.global UiIcon_DigitPointers
UiIcon_DigitPointers:
	.4byte UiIcon_Digit0, UiIcon_Digit1, UiIcon_Digit2, UiIcon_Digit3, UiIcon_Digit4
	.4byte UiIcon_Digit5, UiIcon_Digit6, UiIcon_Digit7, UiIcon_Digit8, UiIcon_Digit9
	.4byte UiIcon_Tens1, UiIcon_Tens2, UiIcon_Tens3
	.4byte -1
UiIcon_Digit0:
	.incbin "games/tbs/src/graphics/ui/marks/digit_0.icon4"
UiIcon_Digit1:
	.incbin "games/tbs/src/graphics/ui/marks/digit_1.icon4"
UiIcon_Digit2:
	.incbin "games/tbs/src/graphics/ui/marks/digit_2.icon4"
UiIcon_Digit3:
	.incbin "games/tbs/src/graphics/ui/marks/digit_3.icon4"
UiIcon_Digit4:
	.incbin "games/tbs/src/graphics/ui/marks/digit_4.icon4"
UiIcon_Digit5:
	.incbin "games/tbs/src/graphics/ui/marks/digit_5.icon4"
UiIcon_Digit6:
	.incbin "games/tbs/src/graphics/ui/marks/digit_6.icon4"
UiIcon_Digit7:
	.incbin "games/tbs/src/graphics/ui/marks/digit_7.icon4"
UiIcon_Digit8:
	.incbin "games/tbs/src/graphics/ui/marks/digit_8.icon4"
UiIcon_Digit9:
	.incbin "games/tbs/src/graphics/ui/marks/digit_9.icon4"
UiIcon_Tens1:
	.incbin "games/tbs/src/graphics/ui/marks/tens_1.icon4"
UiIcon_Tens2:
	.incbin "games/tbs/src/graphics/ui/marks/tens_2.icon4"
UiIcon_Tens3:
	.incbin "games/tbs/src/graphics/ui/marks/tens_3.icon4"
	.balign 4, 0

	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.rept 43
	.4byte UiIcon_Overlay
	.endr
UiIcon_Overlay:
	.incbin "games/tbs/src/graphics/ui/marks/overlay.icon4"
	.balign 4, 0
