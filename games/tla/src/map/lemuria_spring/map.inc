	.global Field_LemuriaSpring
Field_LemuriaSpring:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemuriaSpring, .LField_LemuriaSpring_CELL_PICTURES
	map_block Field_LemuriaSpring, .LField_LemuriaSpring_SHAPES
	map_block Field_LemuriaSpring, .LField_LemuriaSpring_GRID
	map_block Field_LemuriaSpring, .LField_LemuriaSpring_SHAPE_GRID
	map_block Field_LemuriaSpring, .LField_LemuriaSpring_ANIMATION
	map_block Field_LemuriaSpring, .LField_LemuriaSpring_BLEND
	map_block Field_LemuriaSpring, .LField_LemuriaSpring_SCENE
.LField_LemuriaSpring_CELL_PICTURES:
	.incbin "games/tla/src/map/lemuria_spring/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LemuriaSpring_SHAPES:
	.incbin "games/tla/src/map/lemuria_spring/shapes.table.lz"
	.balign 4, 0
.LField_LemuriaSpring_GRID:
	.incbin "games/tla/src/map/lemuria_spring/map.grid.lz"
	.balign 4, 0
.LField_LemuriaSpring_SHAPE_GRID:
	.incbin "games/tla/src/map/lemuria_spring/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemuriaSpring_ANIMATION:
	.incbin "games/tla/src/map/lemuria_spring/animation.script.lz"
	.balign 4, 0
.LField_LemuriaSpring_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_LemuriaSpring_SCENE:
	.incbin "games/tla/src/map/lemuria_spring/scene.scene.lz"
	.balign 4, 0
	.global Tileset_LemuriaSpringPalette
Tileset_LemuriaSpringPalette:
	.incbin "games/tla/src/graphics/tileset/lemuria_spring/palette.gbapal.lz"
	.byte 0x18, 0x61, 0xb5	@ PACKER: left in the buffer by Field_LemuriaSpring+0x1a9
	.global Tileset_LemuriaSpringTilesA
Tileset_LemuriaSpringTilesA:
	.incbin "games/tla/src/graphics/tileset/lemuria_spring/tiles_a.4bpp.mtf"
	.byte 0xd5, 0x83	@ PACKER: left in the buffer by Tileset_LemuriaInteriorsTilesA+0x1b0a
	.global Tileset_LemuriaSpringTilesB
Tileset_LemuriaSpringTilesB:
	.incbin "games/tla/src/graphics/tileset/lemuria_spring/tiles_b.4bpp.mtf"
	.global Tileset_LemuriaSpringTilesC
Tileset_LemuriaSpringTilesC:
	.incbin "games/tla/src/graphics/tileset/lemuria_spring/tiles_c.4bpp.mtf"
	.byte 0x29	@ PACKER: left in the buffer by Tileset_LemuriaSpringTilesA+0xe77
	.global Tileset_LemuriaSpringTilesD
Tileset_LemuriaSpringTilesD:
	.incbin "games/tla/src/graphics/tileset/lemuria_spring/tiles_d.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.balign 4, 0
