	.global Field_AquaRock1
Field_AquaRock1:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRock1, .LField_AquaRock1_CELL_PICTURES
	map_block Field_AquaRock1, .LField_AquaRock1_SHAPES
	map_block Field_AquaRock1, .LField_AquaRock1_GRID
	map_block Field_AquaRock1, .LField_AquaRock1_SHAPE_GRID
	map_block Field_AquaRock1, .LField_AquaRock1_ANIMATION
	map_block Field_AquaRock1, .LField_AquaRock1_BLEND
	map_block Field_AquaRock1, .LField_AquaRock1_SCENE
.LField_AquaRock1_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_AquaRock1_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_1/shapes.table.lz"
	.balign 4, 0
.LField_AquaRock1_GRID:
	.incbin "games/tla/src/map/aqua_rock_1/map.grid.lz"
	.balign 4, 0
.LField_AquaRock1_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRock1_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_1/animation.script.lz"
	.balign 4, 0
.LField_AquaRock1_BLEND:
	.incbin "games/tla/src/map/aqua_rock_1/blend.script.lz"
	.balign 4, 0
.LField_AquaRock1_SCENE:
	.incbin "games/tla/src/map/aqua_rock_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AquaRock1Palette
Tileset_AquaRock1Palette:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_1/palette.gbapal.lz"
	.byte 0x55, 0x71, 0xd4	@ PACKER: left in the buffer by Field_AquaRock1+0x161
	.global Tileset_AquaRock1TilesA
Tileset_AquaRock1TilesA:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_1/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x92, 0x40	@ PACKER: left in the buffer by Field_AquaRock1+0x26dd
	.global Tileset_AquaRock1TilesB
Tileset_AquaRock1TilesB:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_1/tiles_b.4bpp.mtf"
	.global Tileset_AquaRock1TilesC
Tileset_AquaRock1TilesC:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_1/tiles_c.4bpp.mtf"
	.byte 0x7c, 0x3c, 0x07	@ PACKER: left in the buffer by Tileset_AquaRock1TilesB+0x118d
	.global Tileset_AquaRock1TilesD
Tileset_AquaRock1TilesD:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_1/tiles_d.4bpp.mtf"
	.byte 0x8c, 0x03	@ PACKER: left in the buffer by Tileset_AquaRock1TilesB+0x1572
