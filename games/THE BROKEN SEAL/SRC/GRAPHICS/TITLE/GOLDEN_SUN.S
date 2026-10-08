	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Title_GoldenSunLogo
Title_GoldenSunLogo:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN_JA.gbapal"
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN_JA.8bpp.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN_FR.gbapal"
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN_FR.8bpp.lz"
	/* PACKER: left in the packer's buffer by Title_IntroGraphicsC at +0x1921 */
	.byte 0x08, 0x01, 0x29
	.else
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN.gbapal"
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN.8bpp.lz"
	/* PACKER: left in the packer's buffer by Title_IntroGraphicsC at +0x190d */
	.byte 0xd1, 0x27, 0x47
	.endif
	.endif
