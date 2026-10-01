	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Slots_FinalGameBanner: its 16-colour palette, then 32 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_FinalGameBanner
Slots_FinalGameBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_JA.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_DE.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_ES.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_FR.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/FINAL_GAME_IT.4bpp16x16.lz"
	.else
	.incbin "GRAPHICS/SLOTS/FINAL_GAME.gbapal"
	.incbin "GRAPHICS/SLOTS/FINAL_GAME.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
