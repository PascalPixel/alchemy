	.global Field_IsletsTurtle
Field_IsletsTurtle:
	map_header 0, 0, 58, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IsletsTurtle, .LField_IsletsTurtle_CELL_TILES
	map_block Field_IsletsTurtle, .LField_IsletsTurtle_SHAPES
	map_block Field_IsletsTurtle, .LField_IsletsTurtle_GRID
	map_block Field_IsletsTurtle, .LField_IsletsTurtle_SHAPE_GRID
	map_block Field_IsletsTurtle, .LField_IsletsTurtle_ANIMATION
	map_block Field_IsletsTurtle, .LField_IsletsTurtle_BLEND
	map_block Field_IsletsTurtle, .LField_IsletsTurtle_SCENE
.LField_IsletsTurtle_CELL_TILES:
	.incbin "games/tla/src/map/islets_turtle/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_IsletsTurtle_SHAPES:
	.incbin "games/tla/src/map/islets_turtle/shapes.table.lz"
	.balign 4, 0
.LField_IsletsTurtle_GRID:
	.incbin "games/tla/src/map/islets_turtle/map.grid.lz"
	.balign 4, 0
.LField_IsletsTurtle_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_turtle/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletsTurtle_ANIMATION:
	.incbin "games/tla/src/map/islets_turtle/animation.script.lz"
	.balign 4, 0
.LField_IsletsTurtle_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_IsletsTurtle_SCENE:
	.incbin "games/tla/src/map/islets_turtle/scene.scene.lz"
	.balign 4, 0
	.global Tileset_IsletsTurtlePalette
Tileset_IsletsTurtlePalette:
	.incbin "games/tla/src/graphics/tileset/islets_turtle/palette.gbapal.lz"
	.byte 0x80, 0x0e	@ PACKER: left in the buffer by Field_Islets5+0x13a
	.global Tileset_IsletsTurtleTilesA
Tileset_IsletsTurtleTilesA:
	.incbin "games/tla/src/graphics/tileset/islets_turtle/tiles_a.4bpp.mtf"
	.byte 0x06, 0x0d, 0x54	@ PACKER: left in the buffer by Field_Islets5+0x1505
	.global Tileset_IsletsTurtleTilesB
Tileset_IsletsTurtleTilesB:
	.incbin "games/tla/src/graphics/tileset/islets_turtle/tiles_b.4bpp.mtf"
	.global Tileset_IsletsTurtleTilesC
Tileset_IsletsTurtleTilesC:
	.incbin "games/tla/src/graphics/tileset/islets_turtle/tiles_c.4bpp.mtf"
	.byte 0x00, 0x5c	@ PACKER: left in the buffer by Tileset_IsletsTurtleTilesB+0x15e2
	.global Tileset_IsletsTurtleTilesD
Tileset_IsletsTurtleTilesD:
	.incbin "games/tla/src/graphics/tileset/islets_turtle/tiles_d.4bpp.mtf"
	.byte 0xa1, 0x4f, 0x64	@ PACKER: left in the buffer by Tileset_IsletsTurtleTilesC+0x1445
