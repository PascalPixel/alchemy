	.include "games/tbs/include/edition.inc"
	.section .rodata
	.balign 4
@ This string is not filled to four bytes: the tables follow it directly.
	.if EDITION_INTERNATIONAL
	.global gBattleStatusScreen_PpString
gBattleStatusScreen_PpString:
	.asciz "PP"
	.else
	.global gBattleStatusScreen_PpString
gBattleStatusScreen_PpString:
	.asciz "EP"
	.endif
	.global gDjinnMenu_ElementXOffsets
gDjinnMenu_ElementXOffsets:
	.byte 0, 2, 0, 2
	.global gDjinnMenu_ElementYOffsets
gDjinnMenu_ElementYOffsets:
	.byte 0, 0, 1, 1
	.global gMenu_SelectionStepDelays
gMenu_SelectionStepDelays:
	.byte 46, 20, 28, 24, 30, 44, 29, 32
	.global gFieldMenu_CommandByPosition
gFieldMenu_CommandByPosition:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 3, 4, 5, 0
	.global gFieldMenu_PositionByCommand
gFieldMenu_PositionByCommand:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 0, 2, 3, 0
	.global gFileList_ActionByPosition
gFileList_ActionByPosition:
	.byte 0, 1, 2, 3, 4
	.byte 5, 1, 3, 4, 5
	.byte 1, 1, 3, 4, 5
	.byte 1, 1, 1, 0, 2
	.byte 3, 4, 5, 1, 0
