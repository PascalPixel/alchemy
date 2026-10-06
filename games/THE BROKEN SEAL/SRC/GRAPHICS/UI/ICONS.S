	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Ui_Icons
Ui_Icons:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/UI/ICONS/ICONS_JA.icons4"
	.else
	.incbin "GRAPHICS/UI/ICONS/ICONS.icons4"
	.endif
