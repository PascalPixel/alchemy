	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.balign 4
@ 64x16 text headings, each made of two 32x16 sprites
	.global gRenderResource_PairSourceTable
gRenderResource_PairSourceTable:
	.4byte UiLabel_Save, UiLabel_Continue, UiLabel_Copy, UiLabel_Erase
	.4byte UiLabel_Battle, UiLabel_Link, UiLabel_Pause, UiLabel_Setup
	.ifdef TBS_EDITION_JA
UiLabel_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_ja.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_ja.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_ja.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_ja.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_ja.4bpp32x16.plz"
UiLabel_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_ja.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_ja.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tbs/src/graphics/render/labels/setup_ja.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_EN
UiLabel_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_en.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_en.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_en.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_en.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_en.4bpp32x16.plz"
UiLabel_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_en.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_en.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tbs/src/graphics/render/labels/setup_en.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_DE
UiLabel_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_de.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_de.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_de.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_de.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_de.4bpp32x16.plz"
UiLabel_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_de.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_de.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tbs/src/graphics/render/labels/setup_de.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_ES
UiLabel_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_es.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_es.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_es.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_es.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_es.4bpp32x16.plz"
UiLabel_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_es.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_es.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tbs/src/graphics/render/labels/setup_es.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_FR
UiLabel_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_fr.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_fr.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_fr.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_fr.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_fr.4bpp32x16.plz"
UiLabel_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_fr.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_de.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tbs/src/graphics/render/labels/setup_fr.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_IT
UiLabel_Save:
	.incbin "games/tbs/src/graphics/render/labels/save_it.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tbs/src/graphics/render/labels/cont_it.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tbs/src/graphics/render/labels/copy_it.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tbs/src/graphics/render/labels/erase_it.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tbs/src/graphics/render/labels/battle_it.4bpp32x16.plz"
UiLabel_Link:
	.incbin "games/tbs/src/graphics/render/labels/link_it.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tbs/src/graphics/render/labels/pause_es.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tbs/src/graphics/render/labels/setup_it.4bpp32x16.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
