	.include "games/tbs/include/edition.inc"
	.section .rodata
	.balign 4
@ This string is not filled to four bytes: the tables follow it directly.
	.if EDITION_INTERNATIONAL
	.global BattleStatusScreenPpString
BattleStatusScreenPpString:
	.asciz "PP"
	.else
	.global BattleStatusScreenPpString
BattleStatusScreenPpString:
	.asciz "EP"
	.endif
	.global DjinnMenuElementXOffsets
DjinnMenuElementXOffsets:
	.byte 0, 2, 0, 2
	.global DjinnMenuElementYOffsets
DjinnMenuElementYOffsets:
	.byte 0, 0, 1, 1
	.global MenuSelectionStepDelays
MenuSelectionStepDelays:
	.byte 46, 20, 28, 24, 30, 44, 29, 32
	.global FieldMenuCommandByPosition
FieldMenuCommandByPosition:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 3, 4, 5, 0
	.global FieldMenuPositionByCommand
FieldMenuPositionByCommand:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 0, 2, 3, 0
	.global FileListActionByPosition
FileListActionByPosition:
	.byte 0, 1, 2, 3, 4
	.byte 5, 1, 3, 4, 5
	.byte 1, 1, 3, 4, 5
	.byte 1, 1, 1, 0, 2
	.byte 3, 4, 5, 1, 0
