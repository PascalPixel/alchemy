	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.balign 4
@ The file menu's 64x16 headings, each drawn as two 32x16 sprites. The
@ Broken Seal's Link heading gives way to Transfer and Update.
	.global gRenderResource_PairSourceTable
gRenderResource_PairSourceTable:
	.4byte UiLabel_Save, UiLabel_Continue, UiLabel_Copy, UiLabel_Erase, UiLabel_Battle
	.4byte UiLabel_Transfer, UiLabel_Update, UiLabel_Pause, UiLabel_Setup
	.ifdef TLA_EDITION_JA
UiLabel_Save:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_ja.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_ja.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_ja.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_ja.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_ja.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_ja.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_ja.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_ja.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/ui/menu_titles/setup_ja.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_EN
UiLabel_Save:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_en.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_en.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_en.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_en.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_en.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_en.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_en.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_en.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/ui/menu_titles/setup_en.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_DE
UiLabel_Save:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_de.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_de.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_de.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_de.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_de.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_en.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_en.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_de.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/ui/menu_titles/setup_de.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_ES
UiLabel_Save:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_es.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_es.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_es.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_es.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_es.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_es.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_es.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/ui/menu_titles/setup_es.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_FR
UiLabel_Save:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_fr.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_fr.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_fr.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_fr.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_fr.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_fr.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_fr.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_fr.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/ui/menu_titles/setup_fr.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_IT
UiLabel_Save:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_it.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_it.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_it.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_it.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_it.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_it.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_it.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/ui/menu_titles/setup_it.4bpp32x16.plz"
	.endif
	.balign 4, 0
