	.section .rodata
@ Field effect particles, 16x16 sprite frames packed one after another

@ The Broken Seal's FieldFx_GroundParticleTiles, byte for byte
	.global Data_080f3798
Data_080f3798:
	.incbin "games/common/src/field/common/effect/ground.4bpp16x16.plz"

@ A mote shrinking away, in six frames
	.global Data_080f385e
Data_080f385e:
	.incbin "games/tla/src/field/common/effect/mote.4bpp16x16.plz"

@ The Broken Seal's FieldFx_SparkleTiles, byte for byte
	.global Data_080f38b2
Data_080f38b2:
	.incbin "games/common/src/field/common/effect/sparkle.4bpp16x16.plz"

@ A short streak, in two frames
	.global Data_080f3901
Data_080f3901:
	.incbin "games/tla/src/field/common/effect/streak.4bpp16x16.plz"
