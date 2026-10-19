	.include "games/common/include/game/ed_asm.h"
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
	.incbin "games/tla/src/graphics/render/labels/save_ja.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/render/labels/cont_ja.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/render/labels/copy_ja.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/render/labels/erase_ja.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/render/labels/battle_ja.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/render/labels/transfer_ja.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/render/labels/update_ja.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/render/labels/pause_ja.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/render/labels/setup_ja.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_EN
UiLabel_Save:
	.incbin "games/tla/src/graphics/render/labels/save_en.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/render/labels/cont_en.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/render/labels/copy_en.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/render/labels/erase_en.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/render/labels/battle_en.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/render/labels/transfer_en.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/render/labels/update_en.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/render/labels/pause_en.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/render/labels/setup_en.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_DE
UiLabel_Save:
	.incbin "games/tla/src/graphics/render/labels/save_de.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/render/labels/cont_de.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/render/labels/copy_de.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/render/labels/erase_de.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/render/labels/battle_de.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/render/labels/transfer_en.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/render/labels/update_en.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/render/labels/pause_de.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/render/labels/setup_de.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_ES
UiLabel_Save:
	.incbin "games/tla/src/graphics/render/labels/save_es.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/render/labels/cont_es.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/render/labels/copy_es.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/render/labels/erase_es.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/render/labels/battle_es.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/render/labels/transfer_es.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/render/labels/update_es.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/render/labels/pause_es.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/render/labels/setup_es.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_FR
UiLabel_Save:
	.incbin "games/tla/src/graphics/render/labels/save_fr.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/render/labels/cont_fr.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/render/labels/copy_fr.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/render/labels/erase_fr.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/render/labels/battle_fr.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/render/labels/transfer_fr.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/render/labels/update_fr.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/render/labels/pause_fr.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/render/labels/setup_fr.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_IT
UiLabel_Save:
	.incbin "games/tla/src/graphics/render/labels/save_it.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "games/tla/src/graphics/render/labels/cont_it.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "games/tla/src/graphics/render/labels/copy_it.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "games/tla/src/graphics/render/labels/erase_it.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "games/tla/src/graphics/render/labels/battle_it.4bpp32x16.plz"
UiLabel_Transfer:
	.incbin "games/tla/src/graphics/render/labels/transfer_it.4bpp32x16.plz"
UiLabel_Update:
	.incbin "games/tla/src/graphics/render/labels/update_it.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "games/tla/src/graphics/render/labels/pause_es.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "games/tla/src/graphics/render/labels/setup_it.4bpp32x16.plz"
	.endif
	.balign 4, 0
