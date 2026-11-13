@ Trial Road's progress display: its 16-colour palette and the 18-tile
@ picture drawn with it, of which Spanish and Italian each have their own.
	.section .rodata
	.global gTrialRoad_ProgressPalette
gTrialRoad_ProgressPalette:
	.incbin "games/tla/src/graphics/trial_road/picture.gbapal"
	.global gTrialRoad_ProgressPicture
gTrialRoad_ProgressPicture:
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/trial_road/pictr_es.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/trial_road/pictr_it.4bpp.lz"
	.else
	.incbin "games/tla/src/graphics/trial_road/picture.4bpp.lz"
	.endif
	.endif
