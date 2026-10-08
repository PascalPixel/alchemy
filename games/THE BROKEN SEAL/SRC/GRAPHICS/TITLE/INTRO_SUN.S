	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Title_IntroGraphicsA
Title_IntroGraphicsA:
	.incbin "GRAPHICS/TITLE/INTRO_SUN_1.gbapal"
	.incbin "GRAPHICS/TITLE/INTRO_SUN.parts.plz"
	.ifeq EDITION_INTERNATIONAL
	.byte 0x43	@ PACKER: left in the buffer by Ui_WindowTiles+0x1177
	.else
	.byte 0x44	@ PACKER: left in the buffer by Ui_WindowTiles+0x1177
	.endif
