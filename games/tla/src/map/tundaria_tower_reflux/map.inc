	.global Field_TundariaTowerReflux
Field_TundariaTowerReflux:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerReflux, .LField_TundariaTowerReflux_CELL_TILES
	map_block Field_TundariaTowerReflux, .LField_TundariaTowerReflux_SHAPES
	map_block Field_TundariaTowerReflux, .LField_TundariaTowerReflux_GRID
	map_block Field_TundariaTowerReflux, .LField_TundariaTowerReflux_SHAPE_GRID
	map_block Field_TundariaTowerReflux, .LField_TundariaTowerReflux_ANIMATION
	map_block Field_TundariaTowerReflux, .LField_TundariaTowerReflux_BLEND
	map_block Field_TundariaTowerReflux, .LField_TundariaTowerReflux_SCENE
.LField_TundariaTowerReflux_CELL_TILES:
	.incbin "games/tla/src/map/tundaria_tower_reflux/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TundariaTowerReflux_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerReflux_GRID:
	.incbin "games/tla/src/map/tundaria_tower_reflux/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerReflux_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_reflux/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerReflux_ANIMATION:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerReflux_BLEND:
	.incbin "games/tla/src/map/tundaria_tower_reflux/blend.script.lz"
	.balign 4, 0
.LField_TundariaTowerReflux_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_reflux/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTowerReflux
Tileset_PaletteTundariaTowerReflux:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_reflux.gbapal.lz"
