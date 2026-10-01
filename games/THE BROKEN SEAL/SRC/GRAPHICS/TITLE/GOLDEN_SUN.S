	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Title_GoldenSunLogo: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
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
	.else
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN.gbapal"
	.incbin "GRAPHICS/TITLE/GOLDEN_SUN.8bpp.lz"
	.endif
	.endif
