	.include "games/tbs/include/edition.inc"
	.section .rodata
	.balign 4
@ This string is not filled to four bytes: the tables follow it directly.
	.if EDITION_INTERNATIONAL
	.global BattleStatusScreen_PpString
BattleStatusScreen_PpString:
	.asciz "PP"
	.else
	.global BattleStatusScreen_PpString
BattleStatusScreen_PpString:
	.asciz "EP"
	.endif
	.global DjinnMenu_ElementXOffsets
DjinnMenu_ElementXOffsets:
	.byte 0, 2, 0, 2
	.global DjinnMenu_ElementYOffsets
DjinnMenu_ElementYOffsets:
	.byte 0, 0, 1, 1
	.global Menu_SelectionStepDelays
Menu_SelectionStepDelays:
	.byte 46, 20, 28, 24, 30, 44, 29, 32
	.global FieldMenu_CommandByPosition
FieldMenu_CommandByPosition:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 3, 4, 5, 0
	.global FieldMenu_PositionByCommand
FieldMenu_PositionByCommand:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 0, 2, 3, 0
	.global FileList_ActionByPosition
FileList_ActionByPosition:
	.byte 0, 1, 2, 3, 4
	.byte 5, 1, 3, 4, 5
	.byte 1, 1, 3, 4, 5
	.byte 1, 1, 1, 0, 2
	.byte 3, 4, 5, 1, 0
