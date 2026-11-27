	.include "games/tla/include/edition.inc"
	.section .rodata
	.balign 4
@ The file menu's 64x16 headings, each drawn as two 32x16 sprites. The
@ Broken Seal's Link heading gives way to Transfer and Update.
	.global MenuTitlePictures
MenuTitlePictures:
	.4byte UiLabelSave, UiLabelContinue, UiLabelCopy, UiLabelErase, UiLabelBattle
	.4byte UiLabelTransfer, UiLabelUpdate, UiLabelPause, UiLabelSettings
	.ifdef TLA_EDITION_JA
UiLabelSave:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_ja.4bpp32x16.plz"
UiLabelContinue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_ja.4bpp32x16.plz"
UiLabelCopy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_ja.4bpp32x16.plz"
UiLabelErase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_ja.4bpp32x16.plz"
UiLabelBattle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_ja.4bpp32x16.plz"
UiLabelTransfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_ja.4bpp32x16.plz"
UiLabelUpdate:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_ja.4bpp32x16.plz"
UiLabelPause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_ja.4bpp32x16.plz"
UiLabelSettings:
	.incbin "games/tla/src/graphics/ui/menu_titles/settings_ja.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_EN
UiLabelSave:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_en.4bpp32x16.plz"
UiLabelContinue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_en.4bpp32x16.plz"
UiLabelCopy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_en.4bpp32x16.plz"
UiLabelErase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_en.4bpp32x16.plz"
UiLabelBattle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_en.4bpp32x16.plz"
UiLabelTransfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_en.4bpp32x16.plz"
UiLabelUpdate:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_en.4bpp32x16.plz"
UiLabelPause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_en.4bpp32x16.plz"
UiLabelSettings:
	.incbin "games/tla/src/graphics/ui/menu_titles/settings_en.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_DE
UiLabelSave:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_de.4bpp32x16.plz"
UiLabelContinue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_de.4bpp32x16.plz"
UiLabelCopy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_de.4bpp32x16.plz"
UiLabelErase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_de.4bpp32x16.plz"
UiLabelBattle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_de.4bpp32x16.plz"
UiLabelTransfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_en.4bpp32x16.plz"
UiLabelUpdate:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_en.4bpp32x16.plz"
UiLabelPause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_de.4bpp32x16.plz"
UiLabelSettings:
	.incbin "games/tla/src/graphics/ui/menu_titles/settings_de.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_ES
UiLabelSave:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_es.4bpp32x16.plz"
UiLabelContinue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_es.4bpp32x16.plz"
UiLabelCopy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_es.4bpp32x16.plz"
UiLabelErase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_es.4bpp32x16.plz"
UiLabelBattle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_es.4bpp32x16.plz"
UiLabelTransfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_es.4bpp32x16.plz"
UiLabelUpdate:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_es.4bpp32x16.plz"
UiLabelPause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
UiLabelSettings:
	.incbin "games/tla/src/graphics/ui/menu_titles/settings_es.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_FR
UiLabelSave:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_fr.4bpp32x16.plz"
UiLabelContinue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_fr.4bpp32x16.plz"
UiLabelCopy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_fr.4bpp32x16.plz"
UiLabelErase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_fr.4bpp32x16.plz"
UiLabelBattle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_fr.4bpp32x16.plz"
UiLabelTransfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_fr.4bpp32x16.plz"
UiLabelUpdate:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_fr.4bpp32x16.plz"
UiLabelPause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_fr.4bpp32x16.plz"
UiLabelSettings:
	.incbin "games/tla/src/graphics/ui/menu_titles/settings_fr.4bpp32x16.plz"
	.endif
	.ifdef TLA_EDITION_IT
UiLabelSave:
	.incbin "games/tla/src/graphics/ui/menu_titles/save_it.4bpp32x16.plz"
UiLabelContinue:
	.incbin "games/tla/src/graphics/ui/menu_titles/cont_it.4bpp32x16.plz"
UiLabelCopy:
	.incbin "games/tla/src/graphics/ui/menu_titles/copy_it.4bpp32x16.plz"
UiLabelErase:
	.incbin "games/tla/src/graphics/ui/menu_titles/erase_it.4bpp32x16.plz"
UiLabelBattle:
	.incbin "games/tla/src/graphics/ui/menu_titles/battle_it.4bpp32x16.plz"
UiLabelTransfer:
	.incbin "games/tla/src/graphics/ui/menu_titles/transfer_it.4bpp32x16.plz"
UiLabelUpdate:
	.incbin "games/tla/src/graphics/ui/menu_titles/update_it.4bpp32x16.plz"
UiLabelPause:
	.incbin "games/tla/src/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
UiLabelSettings:
	.incbin "games/tla/src/graphics/ui/menu_titles/settings_it.4bpp32x16.plz"
	.endif
	.balign 4, 0
