	.global Field_TundariaTowerIceColumns
Field_TundariaTowerIceColumns:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerIceColumns, .LField_TundariaTowerIceColumns_CELL_TILES
	map_block Field_TundariaTowerIceColumns, .LField_TundariaTowerIceColumns_SHAPES
	map_block Field_TundariaTowerIceColumns, .LField_TundariaTowerIceColumns_GRID
	map_block Field_TundariaTowerIceColumns, .LField_TundariaTowerIceColumns_SHAPE_GRID
	map_block Field_TundariaTowerIceColumns, .LField_TundariaTowerIceColumns_ANIMATION
	map_block Field_TundariaTowerIceColumns, .LField_TundariaTowerIceColumns_BLEND
	map_block Field_TundariaTowerIceColumns, .LField_TundariaTowerIceColumns_SCENE
.LField_TundariaTowerIceColumns_CELL_TILES:
	.incbin "games/tla/src/map/tundaria_tower_ice_columns/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TundariaTowerIceColumns_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_ice_columns/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerIceColumns_GRID:
	.incbin "games/tla/src/map/tundaria_tower_ice_columns/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerIceColumns_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_ice_columns/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerIceColumns_ANIMATION:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerIceColumns_BLEND:
	.incbin "games/tla/src/map/tundaria_tower_reflux/blend.script.lz"
	.balign 4, 0
.LField_TundariaTowerIceColumns_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_ice_columns/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTower5
Tileset_PaletteTundariaTower5:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_ice_columns.gbapal.lz"
