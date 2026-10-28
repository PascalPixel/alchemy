	.include "games/tbs/include/ed_asm.h"
	.section .rodata
	.balign 4
@ 64x16 text headings, each made of two 32x16 sprites
	.global gMenuTitlePictures
gMenuTitlePictures:
	.4byte gMenuTitle_Save, gMenuTitle_Continue, gMenuTitle_Copy, gMenuTitle_Erase
	.4byte gMenuTitle_Battle, gMenuTitle_Link, gMenuTitle_Pause, gMenuTitle_Settings
	.ifdef TBS_EDITION_JA
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/ui/menu_titles/save_ja.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/ui/menu_titles/cont_ja.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/ui/menu_titles/copy_ja.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_ja.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/ui/menu_titles/battle_ja.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_ja.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/ui/menu_titles/pause_ja.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_ja.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_EN
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/ui/menu_titles/save_en.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/ui/menu_titles/cont_en.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/ui/menu_titles/copy_en.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_en.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/ui/menu_titles/battle_en.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_en.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/ui/menu_titles/pause_en.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_en.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_DE
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/ui/menu_titles/save_de.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/ui/menu_titles/cont_de.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/ui/menu_titles/copy_de.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_de.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/ui/menu_titles/battle_de.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_de.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/ui/menu_titles/pause_de.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_de.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_ES
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/ui/menu_titles/save_es.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/ui/menu_titles/cont_es.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/ui/menu_titles/copy_es.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_es.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/ui/menu_titles/battle_es.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_es.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_es.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_FR
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/ui/menu_titles/save_fr.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/ui/menu_titles/cont_fr.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/ui/menu_titles/copy_fr.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_fr.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/ui/menu_titles/battle_fr.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_fr.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/ui/menu_titles/pause_de.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_fr.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_IT
gMenuTitle_Save:
	.incbin "games/tbs/src/graphics/ui/menu_titles/save_it.4bpp32x16.plz"
gMenuTitle_Continue:
	.incbin "games/tbs/src/graphics/ui/menu_titles/cont_it.4bpp32x16.plz"
gMenuTitle_Copy:
	.incbin "games/tbs/src/graphics/ui/menu_titles/copy_it.4bpp32x16.plz"
gMenuTitle_Erase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_it.4bpp32x16.plz"
gMenuTitle_Battle:
	.incbin "games/tbs/src/graphics/ui/menu_titles/battle_it.4bpp32x16.plz"
gMenuTitle_Link:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_it.4bpp32x16.plz"
gMenuTitle_Pause:
	.incbin "games/tbs/src/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
gMenuTitle_Settings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_it.4bpp32x16.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
