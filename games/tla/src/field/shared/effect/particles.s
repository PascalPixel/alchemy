	.section .rodata
@ Field effect particles, 16x16 sprite frames packed one after another

@ A copy of The Broken Seal's gFieldEffect_GroundParticleTiles
	.global gData_080f3798
gData_080f3798:
	.incbin "games/tla/src/field/shared/effect/ground.4bpp16x16.plz"

@ A mote shrinking away, in six frames
	.global gData_080f385e
gData_080f385e:
	.incbin "games/tla/src/field/shared/effect/mote.4bpp16x16.plz"

@ A copy of The Broken Seal's gFieldEffect_SparkleTiles
	.global gData_080f38b2
gData_080f38b2:
	.incbin "games/tla/src/field/shared/effect/sparkle.4bpp16x16.plz"

@ A short streak, in two frames
	.global Data_080f3901
Data_080f3901:
	.incbin "games/tla/src/field/shared/effect/streak.4bpp16x16.plz"
