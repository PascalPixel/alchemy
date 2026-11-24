	.global Field_TaopoSwampLavaRocks
Field_TaopoSwampLavaRocks:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_TaopoSwampLavaRocks, .LField_TaopoSwampLavaRocks_CELL_TILES
	map_block Field_TaopoSwampLavaRocks, .LField_TaopoSwampLavaRocks_SHAPES
	map_block Field_TaopoSwampLavaRocks, .LField_TaopoSwampLavaRocks_GRID
	map_block Field_TaopoSwampLavaRocks, .LField_TaopoSwampLavaRocks_SHAPE_GRID
	map_block Field_TaopoSwampLavaRocks, .LField_TaopoSwampLavaRocks_ANIMATION
	map_block Field_TaopoSwampLavaRocks, .LField_TaopoSwampLavaRocks_BLEND
	map_block Field_TaopoSwampLavaRocks, .LField_TaopoSwampLavaRocks_SCENE
.LField_TaopoSwampLavaRocks_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TaopoSwampLavaRocks_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampLavaRocks_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/map.grid.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwampLavaRocks_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwampLavaRocks_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampLavaRocks_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampLavaRocks_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_lava_rocks/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_PaletteTaopoSwamp9
Tileset_PaletteTaopoSwamp9:
	.incbin "games/tla/src/graphics/tileset/palettes/taopo_swamp_lava_rocks.gbapal.lz"
	.byte 0x01, 0x2f	@ PACKER: left in the buffer by Field_TaopoSwampLavaRocks+0x1ae
