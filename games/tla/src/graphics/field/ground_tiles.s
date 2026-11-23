@ One sprite sheet in two field overlays (679, 67b): the pictures of the shared
@ ground effect (field/ground.png), drawn in colours 1 to 4.
	.section .rodata
	.global gField_GroundTiles
gField_GroundTiles:
	.incbin "games/tla/src/graphics/field/ground_tiles.4bpp16x16.plz"
