	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.balign 4
@ 文字入りの 64x16 見出し (32x16 二枚)
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.4byte UiLabel_Save, UiLabel_Continue, UiLabel_Copy, UiLabel_Erase
	.4byte UiLabel_Battle, UiLabel_Link, UiLabel_Pause, UiLabel_Setup
	.ifdef TBS_EDITION_JA
UiLabel_Save:
	.incbin "GRAPHICS/RENDER/LABELS/SAVE_JA.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "GRAPHICS/RENDER/LABELS/CONT_JA.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "GRAPHICS/RENDER/LABELS/COPY_JA.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "GRAPHICS/RENDER/LABELS/ERASE_JA.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "GRAPHICS/RENDER/LABELS/BATTLE_JA.4bpp32x16.plz"
UiLabel_Link:
	.incbin "GRAPHICS/RENDER/LABELS/LINK_JA.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "GRAPHICS/RENDER/LABELS/PAUSE_JA.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "GRAPHICS/RENDER/LABELS/SETUP_JA.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_EN
UiLabel_Save:
	.incbin "GRAPHICS/RENDER/LABELS/SAVE_EN.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "GRAPHICS/RENDER/LABELS/CONT_EN.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "GRAPHICS/RENDER/LABELS/COPY_EN.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "GRAPHICS/RENDER/LABELS/ERASE_EN.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "GRAPHICS/RENDER/LABELS/BATTLE_EN.4bpp32x16.plz"
UiLabel_Link:
	.incbin "GRAPHICS/RENDER/LABELS/LINK_EN.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "GRAPHICS/RENDER/LABELS/PAUSE_EN.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "GRAPHICS/RENDER/LABELS/SETUP_EN.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_DE
UiLabel_Save:
	.incbin "GRAPHICS/RENDER/LABELS/SAVE_DE.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "GRAPHICS/RENDER/LABELS/CONT_DE.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "GRAPHICS/RENDER/LABELS/COPY_DE.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "GRAPHICS/RENDER/LABELS/ERASE_DE.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "GRAPHICS/RENDER/LABELS/BATTLE_DE.4bpp32x16.plz"
UiLabel_Link:
	.incbin "GRAPHICS/RENDER/LABELS/LINK_DE.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "GRAPHICS/RENDER/LABELS/PAUSE_DE.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "GRAPHICS/RENDER/LABELS/SETUP_DE.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_ES
UiLabel_Save:
	.incbin "GRAPHICS/RENDER/LABELS/SAVE_ES.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "GRAPHICS/RENDER/LABELS/CONT_ES.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "GRAPHICS/RENDER/LABELS/COPY_ES.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "GRAPHICS/RENDER/LABELS/ERASE_ES.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "GRAPHICS/RENDER/LABELS/BATTLE_ES.4bpp32x16.plz"
UiLabel_Link:
	.incbin "GRAPHICS/RENDER/LABELS/LINK_ES.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "GRAPHICS/RENDER/LABELS/PAUSE_ES.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "GRAPHICS/RENDER/LABELS/SETUP_ES.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_FR
UiLabel_Save:
	.incbin "GRAPHICS/RENDER/LABELS/SAVE_FR.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "GRAPHICS/RENDER/LABELS/CONT_FR.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "GRAPHICS/RENDER/LABELS/COPY_FR.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "GRAPHICS/RENDER/LABELS/ERASE_FR.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "GRAPHICS/RENDER/LABELS/BATTLE_FR.4bpp32x16.plz"
UiLabel_Link:
	.incbin "GRAPHICS/RENDER/LABELS/LINK_FR.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "GRAPHICS/RENDER/LABELS/PAUSE_DE.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "GRAPHICS/RENDER/LABELS/SETUP_FR.4bpp32x16.plz"
	.else
	.ifdef TBS_EDITION_IT
UiLabel_Save:
	.incbin "GRAPHICS/RENDER/LABELS/SAVE_IT.4bpp32x16.plz"
UiLabel_Continue:
	.incbin "GRAPHICS/RENDER/LABELS/CONT_IT.4bpp32x16.plz"
UiLabel_Copy:
	.incbin "GRAPHICS/RENDER/LABELS/COPY_IT.4bpp32x16.plz"
UiLabel_Erase:
	.incbin "GRAPHICS/RENDER/LABELS/ERASE_IT.4bpp32x16.plz"
UiLabel_Battle:
	.incbin "GRAPHICS/RENDER/LABELS/BATTLE_IT.4bpp32x16.plz"
UiLabel_Link:
	.incbin "GRAPHICS/RENDER/LABELS/LINK_IT.4bpp32x16.plz"
UiLabel_Pause:
	.incbin "GRAPHICS/RENDER/LABELS/PAUSE_ES.4bpp32x16.plz"
UiLabel_Setup:
	.incbin "GRAPHICS/RENDER/LABELS/SETUP_IT.4bpp32x16.plz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
