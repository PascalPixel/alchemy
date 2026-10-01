	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Ui_Icons: 80 icon slots of 32x32, 57 of them drawn, each with its own
@ 16-colour palette: a halfword offset per slot (0 when empty), then each
@ icon's palette and its pixels in the 4-bit icon coder. The Japanese bank
@ pads each icon to 32 bytes, the European ones to a word.
	.section .rodata
	.global Ui_Icons
Ui_Icons:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/UI/ICONS/ICONS_JA.icons4"
	.else
	.incbin "GRAPHICS/UI/ICONS/ICONS.icons4"
	.endif
