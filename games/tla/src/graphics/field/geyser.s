@ The geysers' tiles, a vent and the column over it, in four field overlays
@ (64d, 661, 66b, 697): Gondowan Cliffs' geysers (Geyser_Start), Yampi Desert's
@ rising sand, Trial Road's fountains and Venus Lighthouse's entrance load
@ them into one VRAM cache entry.
	.section .rodata
	.global gField_GeyserTiles
gField_GeyserTiles:
	.incbin "games/tla/src/graphics/field/geyser.8bpp.lz"
