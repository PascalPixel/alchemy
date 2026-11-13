	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Shop_CursorPictureA
Shop_CursorPictureA:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_a.8bpp"
	.global Shop_CursorPictureB
Shop_CursorPictureB:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_b.8bpp"
	.global Shop_CursorPictureC
Shop_CursorPictureC:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_c.8bpp"
	.global Shop_CursorPictureD
Shop_CursorPictureD:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_d.8bpp"
	.global Shop_CursorPictureE
Shop_CursorPictureE:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_e.8bpp"
	.global Shop_CursorPictureF
Shop_CursorPictureF:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_f.8bpp"
	.global Shop_CursorPictureG
Shop_CursorPictureG:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_g.8bpp"
	.global Shop_CursorPictureH
Shop_CursorPictureH:
	.incbin "games/tla/src/graphics/ui/menu_cursor/cursor_h.8bpp"
@ Four pictures of the window border's eight tiles, one picture a row, in the
@ window palette. Window_StepSwellEffect copies a picture over background tiles 8 to 15
@ as a window's timer runs down (pictures 3, 2, 1, 2, 3), and picture 0, the
@ border Ui_WindowTiles draws, when it ends.
	.global Ui_WindowBorderFlash
Ui_WindowBorderFlash:
	.incbin "games/tla/src/graphics/ui/window_border_flash.4bpp"
	.global Ui_Icons
Ui_Icons:
	.incbin "games/tla/src/graphics/ui/icons/icons.icons4"
	.global Ui_CommandIcons
Ui_CommandIcons:
	.incbin "games/tla/src/graphics/ui/command_icons.icons"
	.byte 0x00, 0x00	@ PACKER: left in the buffer by Ui_Icons+0x3c86
@ Two 16x16 sprites of 16 colours, one under the other: a red flag numbered
@ 1 and a blue flag numbered 2, stored unpacked. No loader of the resource
@ was found; the picture reads in the first row of the shared OBJ palette
@ (black, red, blue and white in colours 1, 2, 8 and 15), which it carries.
	.global Ui_Flags
Ui_Flags:
	.incbin "games/tla/src/graphics/ui/flags.4bpp16x16"
	.section .rodata.a
	.global Ui_WindowTiles
Ui_WindowTiles:
	.ifeq EDITION_INTERNATIONAL
	.ifdef TLA_EDITION_JA
	.incbin "games/tla/src/graphics/ui/window_tiles_tla_ja.4bpp"
	.else
	.incbin "games/tla/src/graphics/ui/window_tiles_ja.4bpp"
	.endif
	.else
	.ifdef TBS_EDITION_DE
	.incbin "games/tla/src/graphics/ui/window_tiles_de.4bpp"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tla/src/graphics/ui/window_tiles_es.4bpp"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tla/src/graphics/ui/window_tiles_fr.4bpp"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tla/src/graphics/ui/window_tiles_it.4bpp"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/ui/window_tiles_de.4bpp"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/ui/window_tiles_es.4bpp"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/ui/window_tiles_fr.4bpp"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/ui/window_tiles_it.4bpp"
	.else
	.incbin "games/tla/src/graphics/ui/window_tiles.4bpp"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Text_SmallFont
Text_SmallFont:
	.incbin "games/tla/src/graphics/ui/small_font.glyphs"
