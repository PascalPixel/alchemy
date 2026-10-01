	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ LuckyDice_Sprites: its 256-colour palette, then 496 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global LuckyDice_Sprites
LuckyDice_Sprites:
	.ifeq EDITION_INTERNATIONAL
	.set LUCKY_DICE_JA, 1
	.endif
	.ifdef TBS_EDITION_DE
	.set LUCKY_DICE_DE, 1
	.endif
	.ifdef TLA_EDITION_DE
	.set LUCKY_DICE_DE, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set LUCKY_DICE_ES, 1
	.endif
	.ifdef TLA_EDITION_ES
	.set LUCKY_DICE_ES, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set LUCKY_DICE_FR, 1
	.endif
	.ifdef TLA_EDITION_FR
	.set LUCKY_DICE_FR, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set LUCKY_DICE_IT, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set LUCKY_DICE_IT, 1
	.endif
	.ifdef LUCKY_DICE_JA
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_JA.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_JA.parts.lz"
	.else
	.ifdef LUCKY_DICE_DE
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_DE.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_DE.parts.lz"
	.else
	.ifdef LUCKY_DICE_ES
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_ES.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_ES.parts.lz"
	.else
	.ifdef LUCKY_DICE_FR
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_FR.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_FR.parts.lz"
	.else
	.ifdef LUCKY_DICE_IT
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_IT.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_IT.parts.lz"
	.else
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
