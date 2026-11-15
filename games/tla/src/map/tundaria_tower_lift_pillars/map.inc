	.global Field_TundariaTowerLiftPillars
Field_TundariaTowerLiftPillars:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerLiftPillars, .LField_TundariaTowerLiftPillars_CELL_PICTURES
	map_block Field_TundariaTowerLiftPillars, .LField_TundariaTowerLiftPillars_SHAPES
	map_block Field_TundariaTowerLiftPillars, .LField_TundariaTowerLiftPillars_GRID
	map_block Field_TundariaTowerLiftPillars, .LField_TundariaTowerLiftPillars_SHAPE_GRID
	map_block Field_TundariaTowerLiftPillars, .LField_TundariaTowerLiftPillars_ANIMATION
	map_block Field_TundariaTowerLiftPillars, .LField_TundariaTowerLiftPillars_BLEND
	map_block Field_TundariaTowerLiftPillars, .LField_TundariaTowerLiftPillars_SCENE
.LField_TundariaTowerLiftPillars_CELL_PICTURES:
	.incbin "games/tla/src/map/tundaria_tower_lift_pillars/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TundariaTowerLiftPillars_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_lift_pillars/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerLiftPillars_GRID:
	.incbin "games/tla/src/map/tundaria_tower_lift_pillars/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerLiftPillars_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_lift_pillars/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerLiftPillars_ANIMATION:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerLiftPillars_BLEND:
	.incbin "games/tla/src/map/tundaria_tower_reflux/blend.script.lz"
	.balign 4, 0
.LField_TundariaTowerLiftPillars_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_lift_pillars/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTower6
Tileset_PaletteTundariaTower6:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_lift_pillars.gbapal.lz"
