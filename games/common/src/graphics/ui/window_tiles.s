	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Ui_WindowTiles
Ui_WindowTiles:
	.ifeq EDITION_INTERNATIONAL
	.ifdef TLA_EDITION_JA
	.incbin "graphics/ui/window_tiles_tla_ja.4bpp"
	.else
	.incbin "graphics/ui/window_tiles_ja.4bpp"
	.endif
	.else
	.ifdef TBS_EDITION_DE
	.incbin "graphics/ui/window_tiles_de.4bpp"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/ui/window_tiles_es.4bpp"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/ui/window_tiles_fr.4bpp"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/ui/window_tiles_it.4bpp"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "graphics/ui/window_tiles_de.4bpp"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/ui/window_tiles_es.4bpp"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/ui/window_tiles_fr.4bpp"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/ui/window_tiles_it.4bpp"
	.else
	.incbin "graphics/ui/window_tiles.4bpp"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
