	.global Field_Caverns1
Field_Caverns1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Caverns1, .LField_Caverns1_CELL_PICTURES
	map_block Field_Caverns1, .LField_Caverns1_SHAPES
	map_block Field_Caverns1, .LField_Caverns1_GRID
	map_block Field_Caverns1, .LField_Caverns1_SHAPE_GRID
	map_block Field_Caverns1, .LField_Caverns1_ANIMATION
	map_block Field_Caverns1, .LField_Caverns1_BLEND
	map_block Field_Caverns1, .LField_Caverns1_SCENE
.LField_Caverns1_CELL_PICTURES:
	.incbin "games/tla/src/map/caverns_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Caverns1_SHAPES:
	.incbin "games/tla/src/map/caverns_1/shapes.table.lz"
	.balign 4, 0
.LField_Caverns1_GRID:
	.incbin "games/tla/src/map/caverns_1/map.grid.lz"
	.balign 4, 0
.LField_Caverns1_SHAPE_GRID:
	.incbin "games/tla/src/map/caverns_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Caverns1_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_4/animation.script.lz"
	.balign 4, 0
.LField_Caverns1_BLEND:
	.incbin "games/tla/src/map/dehkan_plateau_4/blend.script.lz"
	.balign 4, 0
.LField_Caverns1_SCENE:
	.incbin "games/tla/src/map/caverns_1/scene.scene.lz"
	.global Tileset_PaletteCaverns1
Tileset_PaletteCaverns1:
	.incbin "games/tla/src/graphics/tileset/palettes/caverns_1.gbapal.lz"
