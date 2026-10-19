	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global Colosso_RivalPathA
Colosso_RivalPathA:
	.incbin "games/tbs/src/map/colosso/rival_path_a.table.lz"
	.byte 0xe0, 0xc8, 0xb4	@ PACKER: left in the buffer by WorldMap_TilesE+0x1899
	.global Colosso_RivalPathB
Colosso_RivalPathB:
	.incbin "games/tbs/src/map/colosso/rival_path_b.table.lz"
	.byte 0x33, 0x59	@ PACKER: left in the buffer by Colosso_RivalPathA+0x15c6
	.global Colosso_RivalPathC
Colosso_RivalPathC:
	.incbin "games/tbs/src/map/colosso/rival_path_c.table.lz"
	.byte 0x8d, 0x8f	@ PACKER: left in the buffer by WorldMap_TilesE+0x1a8e
	.global Colosso_Lettering
Colosso_Lettering:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tbs/src/graphics/colosso/lettering_ja.parts.lz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/colosso/lettering_de.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/colosso/lettering_es.parts.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/colosso/lettering_fr.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/colosso/lettering_it.parts.lz"
	.else
	.incbin "games/tbs/src/graphics/colosso/lettering.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x30, 0x58	@ PACKER: left in the buffer by Colosso_RivalPathC+0xdfa
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x73	@ PACKER: left in the buffer by Colosso_RivalPathC+0xdb3
	.else
	.ifdef TBS_EDITION_JA
	.byte 0xbd, 0x8e	@ PACKER: left in the buffer by Colosso_RivalPathC+0xf02
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xc5	@ PACKER: left in the buffer by Colosso_RivalPathC+0x124f
	.endif
	.endif
	.endif
	.endif
