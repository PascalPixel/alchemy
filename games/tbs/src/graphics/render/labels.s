	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.balign 4
@ 64x16 text headings, each made of two 32x16 sprites
	.global gMenuTitlePictures
gMenuTitlePictures:
	.4byte gMenuTitle_Save, gMenuTitle_Continue, gMenuTitle_Copy, gMenuTitle_Erase
	.4byte gMenuTitle_Battle, gMenuTitle_Link, gMenuTitle_Pause, gMenuTitle_Settings
	.ifdef TBS_EDITION_JA
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_ja.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_ja.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_ja.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_ja.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_ja.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_ja.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_ja.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/render/labels/setup_ja.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_EN
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_en.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_en.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_en.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_en.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_en.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_en.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_en.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/render/labels/setup_en.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_DE
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_de.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_de.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_de.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_de.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_de.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_de.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_de.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/render/labels/setup_de.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_ES
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_es.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_es.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_es.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_es.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_es.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_es.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_es.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/render/labels/setup_es.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_FR
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_fr.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_fr.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_fr.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_fr.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_fr.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_fr.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_de.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/render/labels/setup_fr.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_IT
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_it.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_it.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_it.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_it.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_it.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_it.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_es.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/render/labels/setup_it.4bpp32x16.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
