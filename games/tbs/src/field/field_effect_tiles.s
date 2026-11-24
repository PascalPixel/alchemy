@ The ground particles', motes of light's and sparkles' 16 by 16 tiles, packed.
	.section .rodata
	.global gWeather_ParticleTiles
gWeather_ParticleTiles:
	.incbin "games/tbs/src/field/ground.4bpp16x16.plz"
	.global gFieldEffect_MoteTiles
gFieldEffect_MoteTiles:
	.incbin "games/tbs/src/field/mote.4bpp16x16.plz"
	.global gFieldEffect_SparkleTiles
gFieldEffect_SparkleTiles:
	.incbin "games/tbs/src/field/sparkle.4bpp16x16.plz"
	.balign 4, 0
