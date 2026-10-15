	.include "games/common/include/game/ed_asm.h"
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
	.incbin "games/common/src/graphics/lucky_dice/sprites_1_ja.gbapal"
	.incbin "games/common/src/graphics/lucky_dice/sprites_ja.parts.lz"
	.else
	.ifdef LUCKY_DICE_DE
	.incbin "games/common/src/graphics/lucky_dice/sprites_1_de.gbapal"
	.incbin "games/common/src/graphics/lucky_dice/sprites_de.parts.lz"
	.else
	.ifdef LUCKY_DICE_ES
	.incbin "games/common/src/graphics/lucky_dice/sprites_1_es.gbapal"
	.incbin "games/common/src/graphics/lucky_dice/sprites_es.parts.lz"
	.else
	.ifdef LUCKY_DICE_FR
	.incbin "games/common/src/graphics/lucky_dice/sprites_1_fr.gbapal"
	.incbin "games/common/src/graphics/lucky_dice/sprites_fr.parts.lz"
	.else
	.ifdef LUCKY_DICE_IT
	.incbin "games/common/src/graphics/lucky_dice/sprites_1_it.gbapal"
	.incbin "games/common/src/graphics/lucky_dice/sprites_it.parts.lz"
	.else
	.incbin "games/common/src/graphics/lucky_dice/sprites_1.gbapal"
	.incbin "games/common/src/graphics/lucky_dice/sprites.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifeq GAME_TLA
	.ifdef TBS_EDITION_EN
	.byte 0x0f, 0x72, 0xb1	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x3021
	.else
	.ifdef TBS_EDITION_ES
	.byte 0x5a, 0x9b	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x2f7a
	.else
	.ifdef TBS_EDITION_FR
	.byte 0xac	@ PACKER: left in the buffer by Battle_AerialViewBackdrop+0x31a7
	.endif
	.endif
	.endif
	.else
	.ifdef TLA_EDITION_EN
	.byte 0xd1, 0x40, 0x09	@ PACKER: left in the buffer by Slots_DomePicture+0x3021
	.else
	.ifdef TLA_EDITION_ES
	.byte 0xef, 0x4f	@ PACKER: left in the buffer by Slots_DomePicture+0x2f7a
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x54	@ PACKER: left in the buffer by Slots_DomePicture+0x31a7
	.endif
	.endif
	.endif
	.endif
