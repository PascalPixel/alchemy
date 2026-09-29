@ Ui_WindowTiles: 256 tiles of 4-bit colour for windows, their frames, the
@ text font and button marks, drawn with the window palette.
	.section .rodata
	.global Ui_WindowTiles
Ui_WindowTiles:
	.ifdef TBS_EDITION_JA
	.incbin "GRAPHICS/UI/WINDOW_TILES_JA.4bpp"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/UI/WINDOW_TILES_DE.4bpp"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/UI/WINDOW_TILES_ES.4bpp"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/UI/WINDOW_TILES_FR.4bpp"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/UI/WINDOW_TILES_IT.4bpp"
	.else
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/UI/WINDOW_TILES_TLA_JA.4bpp"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/UI/WINDOW_TILES_DE.4bpp"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/UI/WINDOW_TILES_ES.4bpp"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/UI/WINDOW_TILES_FR.4bpp"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/UI/WINDOW_TILES_IT.4bpp"
	.else
	.incbin "GRAPHICS/UI/WINDOW_TILES.4bpp"
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
