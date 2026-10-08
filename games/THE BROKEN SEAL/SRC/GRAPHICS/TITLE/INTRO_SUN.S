	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Title_IntroGraphicsA
Title_IntroGraphicsA:
	.incbin "GRAPHICS/TITLE/INTRO_SUN_1.gbapal"
	.incbin "GRAPHICS/TITLE/INTRO_SUN.parts.plz"
	/* PACKER: left in the packer's buffer by Ui_WindowTiles at +0x1177 */
	.ifeq EDITION_INTERNATIONAL
	.byte 0x43
	.else
	.byte 0x44
	.endif
