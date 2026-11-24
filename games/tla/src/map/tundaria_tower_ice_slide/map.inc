	.global Field_TundariaTowerIceSlide
Field_TundariaTowerIceSlide:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerIceSlide, .LField_TundariaTowerIceSlide_CELL_TILES
	map_block Field_TundariaTowerIceSlide, .LField_TundariaTowerIceSlide_SHAPES
	map_block Field_TundariaTowerIceSlide, .LField_TundariaTowerIceSlide_GRID
	map_block Field_TundariaTowerIceSlide, .LField_TundariaTowerIceSlide_SHAPE_GRID
	map_block Field_TundariaTowerIceSlide, .LField_TundariaTowerIceSlide_ANIMATION
	map_block Field_TundariaTowerIceSlide, .LField_TundariaTowerIceSlide_BLEND
	map_block Field_TundariaTowerIceSlide, .LField_TundariaTowerIceSlide_SCENE
.LField_TundariaTowerIceSlide_CELL_TILES:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TundariaTowerIceSlide_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerIceSlide_GRID:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerIceSlide_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerIceSlide_ANIMATION:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerIceSlide_BLEND:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/blend.script.lz"
	.balign 4, 0
.LField_TundariaTowerIceSlide_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTower3
Tileset_PaletteTundariaTower3:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_ice_slide.gbapal.lz"
