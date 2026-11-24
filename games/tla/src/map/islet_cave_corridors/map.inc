	.global Field_IsletCaveCorridors
Field_IsletCaveCorridors:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IsletCaveCorridors, .LField_IsletCaveCorridors_CELL_TILES
	map_block Field_IsletCaveCorridors, .LField_IsletCaveCorridors_SHAPES
	map_block Field_IsletCaveCorridors, .LField_IsletCaveCorridors_GRID
	map_block Field_IsletCaveCorridors, .LField_IsletCaveCorridors_SHAPE_GRID
	map_block Field_IsletCaveCorridors, .LField_IsletCaveCorridors_ANIMATION
	map_block Field_IsletCaveCorridors, .LField_IsletCaveCorridors_BLEND
	map_block Field_IsletCaveCorridors, .LField_IsletCaveCorridors_SCENE
.LField_IsletCaveCorridors_CELL_TILES:
	.incbin "games/tla/src/map/islet_cave_corridors/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_IsletCaveCorridors_SHAPES:
	.incbin "games/tla/src/map/islet_cave_corridors/shapes.table.lz"
	.balign 4, 0
.LField_IsletCaveCorridors_GRID:
	.incbin "games/tla/src/map/islet_cave_corridors/map.grid.lz"
	.balign 4, 0
.LField_IsletCaveCorridors_SHAPE_GRID:
	.incbin "games/tla/src/map/islet_cave_corridors/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletCaveCorridors_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_IsletCaveCorridors_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_IsletCaveCorridors_SCENE:
	.incbin "games/tla/src/map/islet_cave_corridors/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteIsletCaveCorridors
Tileset_PaletteIsletCaveCorridors:
	.incbin "games/tla/src/graphics/tileset/palettes/islet_cave_corridors.gbapal.lz"
