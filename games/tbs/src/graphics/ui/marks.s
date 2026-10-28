	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.balign 4
@ The three icon bases
	.global gIcon_BasePointerTable
gIcon_BasePointerTable:
	.4byte Icon_Base0, Icon_Base1, Icon_Base2
	.4byte -1
Icon_Base0:
	.incbin "games/tbs/src/graphics/ui/marks/base_0.icon4"
Icon_Base1:
	.incbin "games/tbs/src/graphics/ui/marks/base_1.icon4"
Icon_Base2:
	.incbin "games/tbs/src/graphics/ui/marks/base_2.icon4"
	.balign 4, 0

@ The equipped, cross and phi marks
	.global gIcon_MarkPointers
gIcon_MarkPointers:
	.4byte Icon_MarkEquip, Icon_MarkCross, Icon_MarkPhi
Icon_MarkEquip:
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tbs/src/graphics/ui/marks/equip_de.icon4"
	.else
	.incbin "games/tbs/src/graphics/ui/marks/equip.icon4"
	.endif
	.endif
	.endif
	.endif
Icon_MarkCross:
	.incbin "games/tbs/src/graphics/ui/marks/cross.icon4"
Icon_MarkPhi:
	.incbin "games/tbs/src/graphics/ui/marks/phi.icon4"
	.balign 4, 0

@ The digits 0 to 9, then the tens 1 to 3
	.global gIcon_DigitPointers
gIcon_DigitPointers:
	.4byte Icon_Digit0, Icon_Digit1, Icon_Digit2, Icon_Digit3, Icon_Digit4
	.4byte Icon_Digit5, Icon_Digit6, Icon_Digit7, Icon_Digit8, Icon_Digit9
	.4byte Icon_Tens1, Icon_Tens2, Icon_Tens3
	.4byte -1
Icon_Digit0:
	.incbin "games/tbs/src/graphics/ui/marks/digit_0.icon4"
Icon_Digit1:
	.incbin "games/tbs/src/graphics/ui/marks/digit_1.icon4"
Icon_Digit2:
	.incbin "games/tbs/src/graphics/ui/marks/digit_2.icon4"
Icon_Digit3:
	.incbin "games/tbs/src/graphics/ui/marks/digit_3.icon4"
Icon_Digit4:
	.incbin "games/tbs/src/graphics/ui/marks/digit_4.icon4"
Icon_Digit5:
	.incbin "games/tbs/src/graphics/ui/marks/digit_5.icon4"
Icon_Digit6:
	.incbin "games/tbs/src/graphics/ui/marks/digit_6.icon4"
Icon_Digit7:
	.incbin "games/tbs/src/graphics/ui/marks/digit_7.icon4"
Icon_Digit8:
	.incbin "games/tbs/src/graphics/ui/marks/digit_8.icon4"
Icon_Digit9:
	.incbin "games/tbs/src/graphics/ui/marks/digit_9.icon4"
Icon_Tens1:
	.incbin "games/tbs/src/graphics/ui/marks/tens_1.icon4"
Icon_Tens2:
	.incbin "games/tbs/src/graphics/ui/marks/tens_2.icon4"
Icon_Tens3:
	.incbin "games/tbs/src/graphics/ui/marks/tens_3.icon4"
	.balign 4, 0

	.global gIcon_OverlayPointerTable
gIcon_OverlayPointerTable:
	.rept 43
	.4byte Icon_Overlay
	.endr
Icon_Overlay:
	.incbin "games/tbs/src/graphics/ui/marks/overlay.icon4"
	.balign 4, 0
