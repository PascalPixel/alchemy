	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Ending_CliffDayPicture
Ending_CliffDayPicture:
	.incbin "games/tla/src/graphics/ending/cliff_day.gbapal"
	.incbin "games/tla/src/graphics/ending/cliff_day.bitmap.d7"
	.global Ending_CliffSunsetPicture
Ending_CliffSunsetPicture:
	.incbin "games/tla/src/graphics/ending/cliff_sunset.gbapal"
	.incbin "games/tla/src/graphics/ending/cliff_sunset.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Ending_VillagersPictureA
Ending_VillagersPictureA:
	.incbin "games/tla/src/graphics/ending/villagers_a.gbapal"
	.incbin "games/tla/src/graphics/ending/villagers_a.bitmap.d7"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Ending_VillagersPictureB
Ending_VillagersPictureB:
	.incbin "games/tla/src/graphics/ending/villagers_b.gbapal"
	.incbin "games/tla/src/graphics/ending/villagers_b.bitmap.d7"
	.byte 0x01, 0x05	@ PACKER: left in the buffer by Ending_VillagersPictureA+0x921a
	.global Ending_EmbracePicture
Ending_EmbracePicture:
	.incbin "games/tla/src/graphics/ending/embrace.gbapal"
	.incbin "games/tla/src/graphics/ending/embrace.bitmap.d7"
	.global Ending_ReunionPicture
Ending_ReunionPicture:
	.incbin "games/tla/src/graphics/ending/reunion.gbapal"
	.incbin "games/tla/src/graphics/ending/reunion.bitmap.d7"
	.byte 0x37, 0x16	@ PACKER: left in the buffer by Ending_VillagersPictureB+0x7842
	.global Ending_SunsetPlainPicture
Ending_SunsetPlainPicture:
	.incbin "games/tla/src/graphics/ending/sunset_plain.gbapal"
	.incbin "games/tla/src/graphics/ending/sunset_plain.bitmap.d7"
	.byte 0x5e, 0x21	@ PACKER: left in the buffer by Ending_ReunionPicture+0x5e52
	.global Ending_TheEndPicture
Ending_TheEndPicture:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/ending/the_end_ja.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_ja.bitmap.d7"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/ending/the_end_de.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_de.bitmap.d7"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/ending/the_end_es.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_es.bitmap.d7"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/ending/the_end_es.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_es.bitmap.d7"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/ending/the_end_it.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end_it.bitmap.d7"
	.else
	.incbin "games/tla/src/graphics/ending/the_end.gbapal"
	.incbin "games/tla/src/graphics/ending/the_end.bitmap.d7"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x88, 0x0e	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x5cd2
	.else
	.ifdef TLA_EDITION_EN
	.byte 0xc5, 0x19	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x5d1e
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x00, 0x00	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x59b6
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x50, 0x45	@ PACKER: left in the buffer by Ending_SunsetPlainPicture+0x5d42
	.endif
	.endif
	.endif
	.endif
