	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Korosseo_RivalPathA
Korosseo_RivalPathA:
	.incbin "map/korosseo/rival_path_a.table.lz"
	.byte 0xe0, 0xc8, 0xb4	@ PACKER: left in the buffer by WorldMap_TilesE+0x1899
	.global Korosseo_RivalPathB
Korosseo_RivalPathB:
	.incbin "map/korosseo/rival_path_b.table.lz"
	.byte 0x33, 0x59	@ PACKER: left in the buffer by Korosseo_RivalPathA+0x15c6
	.global Korosseo_RivalPathC
Korosseo_RivalPathC:
	.incbin "map/korosseo/rival_path_c.table.lz"
	.byte 0x8d, 0x8f	@ PACKER: left in the buffer by WorldMap_TilesE+0x1a8e
	.global Korosseo_Lettering
Korosseo_Lettering:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/korosseo/lettering_ja.parts.lz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "graphics/korosseo/lettering_de.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/korosseo/lettering_es.parts.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/korosseo/lettering_fr.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/korosseo/lettering_it.parts.lz"
	.else
	.incbin "graphics/korosseo/lettering.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x30, 0x58	@ PACKER: left in the buffer by Korosseo_RivalPathC+0xdfa
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x73	@ PACKER: left in the buffer by Korosseo_RivalPathC+0xdb3
	.else
	.ifdef TBS_EDITION_JA
	.byte 0xbd, 0x8e	@ PACKER: left in the buffer by Korosseo_RivalPathC+0xf02
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xc5	@ PACKER: left in the buffer by Korosseo_RivalPathC+0x124f
	.endif
	.endif
	.endif
	.endif
