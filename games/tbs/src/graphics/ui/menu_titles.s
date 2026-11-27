	.include "games/tbs/include/edition.inc"
	.section .rodata
	.balign 4
@ 64x16 text headings, each made of two 32x16 sprites
	.global MenuTitlePictures
@ mnTitleLoadFirstTile reaches the table by three more names, one a case.
	.global MenuTitlePicturesCaseA
	.global MenuTitlePicturesCaseB
	.global MenuTitlePicturesCaseC
MenuTitlePictures:
MenuTitlePicturesCaseA:
MenuTitlePicturesCaseB:
MenuTitlePicturesCaseC:
	.4byte MenuTitleSave, MenuTitleContinue, MenuTitleCopy, MenuTitleErase
	.4byte MenuTitleBattle, MenuTitleLink, MenuTitlePause, MenuTitleSettings
	.ifdef TBS_EDITION_JA
MenuTitleSave:
	.incbin "games/common/graphics/ui/menu_titles/save_ja.4bpp32x16.plz"
MenuTitleContinue:
	.incbin "games/common/graphics/ui/menu_titles/cont_ja.4bpp32x16.plz"
MenuTitleCopy:
	.incbin "games/common/graphics/ui/menu_titles/copy_ja.4bpp32x16.plz"
MenuTitleErase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_ja.4bpp32x16.plz"
MenuTitleBattle:
	.incbin "games/common/graphics/ui/menu_titles/battle_ja.4bpp32x16.plz"
MenuTitleLink:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_ja.4bpp32x16.plz"
MenuTitlePause:
	.incbin "games/common/graphics/ui/menu_titles/pause_ja.4bpp32x16.plz"
MenuTitleSettings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_ja.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_EN
MenuTitleSave:
	.incbin "games/common/graphics/ui/menu_titles/save_en.4bpp32x16.plz"
MenuTitleContinue:
	.incbin "games/common/graphics/ui/menu_titles/cont_en.4bpp32x16.plz"
MenuTitleCopy:
	.incbin "games/common/graphics/ui/menu_titles/copy_en.4bpp32x16.plz"
MenuTitleErase:
	.incbin "games/common/graphics/ui/menu_titles/erase_en.4bpp32x16.plz"
MenuTitleBattle:
	.incbin "games/common/graphics/ui/menu_titles/battle_en.4bpp32x16.plz"
MenuTitleLink:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_en.4bpp32x16.plz"
MenuTitlePause:
	.incbin "games/common/graphics/ui/menu_titles/pause_en.4bpp32x16.plz"
MenuTitleSettings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_en.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_DE
MenuTitleSave:
	.incbin "games/common/graphics/ui/menu_titles/save_de.4bpp32x16.plz"
MenuTitleContinue:
	.incbin "games/common/graphics/ui/menu_titles/cont_de.4bpp32x16.plz"
MenuTitleCopy:
	.incbin "games/common/graphics/ui/menu_titles/copy_de.4bpp32x16.plz"
MenuTitleErase:
	.incbin "games/common/graphics/ui/menu_titles/erase_de.4bpp32x16.plz"
MenuTitleBattle:
	.incbin "games/common/graphics/ui/menu_titles/battle_de.4bpp32x16.plz"
MenuTitleLink:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_de.4bpp32x16.plz"
MenuTitlePause:
	.incbin "games/common/graphics/ui/menu_titles/pause_de.4bpp32x16.plz"
MenuTitleSettings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_de.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_ES
MenuTitleSave:
	.incbin "games/common/graphics/ui/menu_titles/save_es.4bpp32x16.plz"
MenuTitleContinue:
	.incbin "games/common/graphics/ui/menu_titles/cont_es.4bpp32x16.plz"
MenuTitleCopy:
	.incbin "games/common/graphics/ui/menu_titles/copy_es.4bpp32x16.plz"
MenuTitleErase:
	.incbin "games/common/graphics/ui/menu_titles/erase_es.4bpp32x16.plz"
MenuTitleBattle:
	.incbin "games/common/graphics/ui/menu_titles/battle_es.4bpp32x16.plz"
MenuTitleLink:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_es.4bpp32x16.plz"
MenuTitlePause:
	.incbin "games/common/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
MenuTitleSettings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_es.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_FR
MenuTitleSave:
	.incbin "games/common/graphics/ui/menu_titles/save_fr.4bpp32x16.plz"
MenuTitleContinue:
	.incbin "games/common/graphics/ui/menu_titles/cont_fr.4bpp32x16.plz"
MenuTitleCopy:
	.incbin "games/common/graphics/ui/menu_titles/copy_fr.4bpp32x16.plz"
MenuTitleErase:
	.incbin "games/tbs/src/graphics/ui/menu_titles/erase_fr.4bpp32x16.plz"
MenuTitleBattle:
	.incbin "games/common/graphics/ui/menu_titles/battle_fr.4bpp32x16.plz"
MenuTitleLink:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_fr.4bpp32x16.plz"
MenuTitlePause:
	.incbin "games/common/graphics/ui/menu_titles/pause_de.4bpp32x16.plz"
MenuTitleSettings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_fr.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_IT
MenuTitleSave:
	.incbin "games/common/graphics/ui/menu_titles/save_it.4bpp32x16.plz"
MenuTitleContinue:
	.incbin "games/common/graphics/ui/menu_titles/cont_it.4bpp32x16.plz"
MenuTitleCopy:
	.incbin "games/common/graphics/ui/menu_titles/copy_it.4bpp32x16.plz"
MenuTitleErase:
	.incbin "games/common/graphics/ui/menu_titles/erase_it.4bpp32x16.plz"
MenuTitleBattle:
	.incbin "games/common/graphics/ui/menu_titles/battle_it.4bpp32x16.plz"
MenuTitleLink:
	.incbin "games/tbs/src/graphics/ui/menu_titles/link_it.4bpp32x16.plz"
MenuTitlePause:
	.incbin "games/common/graphics/ui/menu_titles/pause_es.4bpp32x16.plz"
MenuTitleSettings:
	.incbin "games/tbs/src/graphics/ui/menu_titles/setup_it.4bpp32x16.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
