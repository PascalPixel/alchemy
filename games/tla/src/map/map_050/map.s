	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map050
Field_Map050:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map050, .LField_Map050_CELL_PICTURES
	map_block Field_Map050, .LField_Map050_SHAPES
	map_block Field_Map050, .LField_Map050_GRID
	map_block Field_Map050, .LField_Map050_SHAPE_GRID
	map_block Field_Map050, .LField_Map050_ANIMATION
	map_block Field_Map050, .LField_Map050_BLEND
	map_block Field_Map050, .LField_Map050_SCENE
.LField_Map050_CELL_PICTURES:
	.incbin "games/tla/src/map/map_050/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map050_SHAPES:
	.incbin "games/tla/src/map/map_050/shapes.table.lz"
	.balign 4, 0
.LField_Map050_GRID:
	.incbin "games/tla/src/map/map_050/grid.parts.lz"
	.balign 4, 0
.LField_Map050_SHAPE_GRID:
	.incbin "games/tla/src/map/map_050/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map050_ANIMATION:
	.incbin "games/tla/src/map/map_050/animation.script.lz"
	.balign 4, 0
.LField_Map050_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map050_SCENE:
	.incbin "games/tla/src/map/map_050/scene.parts.lz"
