	.section .rodata
@ Field effect particles, 16x16 sprite pictures packed one after another

@ A copy of The Broken Seal's gWeather_ParticleTiles
	.global gWeather_ParticleTiles
gWeather_ParticleTiles:
	.incbin "games/tla/src/field/ground.4bpp16x16.plz"

@ A mote shrinking away, in six frames
	.global gFieldEffect_MoteTiles
gFieldEffect_MoteTiles:
	.incbin "games/tla/src/field/mote.4bpp16x16.plz"

@ A copy of The Broken Seal's gFieldEffect_SparkleTiles
	.global gFieldEffect_SparkleTiles
gFieldEffect_SparkleTiles:
	.incbin "games/tla/src/field/sparkle.4bpp16x16.plz"

@ A short streak, in two frames
	.global gFieldEffect_StreakTiles
gFieldEffect_StreakTiles:
	.incbin "games/tla/src/field/streak.4bpp16x16.plz"
