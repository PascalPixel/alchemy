	.section .rodata
@ Field effect particles, 16x16 sprite pictures packed one after another

@ A copy of The Broken Seal's gFieldEffect_GroundParticleTiles
	.global gFieldEffect_GroundParticleTiles
gFieldEffect_GroundParticleTiles:
	.incbin "games/tla/src/field/field_effect/ground.4bpp16x16.plz"

@ A mote shrinking away, in six frames
	.global gData_080f385e
gData_080f385e:
	.incbin "games/tla/src/field/field_effect/mote.4bpp16x16.plz"

@ A copy of The Broken Seal's gFieldEffect_SparkleTiles
	.global gFieldEffect_SparkleTiles
gFieldEffect_SparkleTiles:
	.incbin "games/tla/src/field/field_effect/sparkle.4bpp16x16.plz"

@ A short streak, in two frames
	.global Data_080f3901
Data_080f3901:
	.incbin "games/tla/src/field/field_effect/streak.4bpp16x16.plz"
