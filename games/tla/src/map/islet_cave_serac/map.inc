	.global Field_IsletCaveSerac
Field_IsletCaveSerac:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IsletCaveSerac, .LField_IsletCaveSerac_CELL_PICTURES
	map_block Field_IsletCaveSerac, .LField_IsletCaveSerac_SHAPES
	map_block Field_IsletCaveSerac, .LField_IsletCaveSerac_GRID
	map_block Field_IsletCaveSerac, .LField_IsletCaveSerac_SHAPE_GRID
	map_block Field_IsletCaveSerac, .LField_IsletCaveSerac_ANIMATION
	map_block Field_IsletCaveSerac, .LField_IsletCaveSerac_BLEND
	map_block Field_IsletCaveSerac, .LField_IsletCaveSerac_SCENE
.LField_IsletCaveSerac_CELL_PICTURES:
	.incbin "games/tla/src/map/islet_cave_serac/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_IsletCaveSerac_SHAPES:
	.incbin "games/tla/src/map/islet_cave_serac/shapes.table.lz"
	.balign 4, 0
.LField_IsletCaveSerac_GRID:
	.incbin "games/tla/src/map/islet_cave_serac/map.grid.lz"
	.balign 4, 0
.LField_IsletCaveSerac_SHAPE_GRID:
	.incbin "games/tla/src/map/islet_cave_serac/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletCaveSerac_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_IsletCaveSerac_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_IsletCaveSerac_SCENE:
	.incbin "games/tla/src/map/islet_cave_serac/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteIsletCaveSerac
Tileset_PaletteIsletCaveSerac:
	.incbin "games/tla/src/graphics/tileset/palettes/islet_cave_serac.gbapal.lz"
