	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map025
Field_Map025:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map025, .LField_Map025_CELL_PICTURES
	map_block Field_Map025, .LField_Map025_SHAPES
	map_block Field_Map025, .LField_Map025_GRID
	map_block Field_Map025, .LField_Map025_SHAPE_GRID
	map_block Field_Map025, .LField_Map025_ANIMATION
	map_block Field_Map025, .LField_Map025_BLEND
	map_block Field_Map025, .LField_Map025_SCENE
.LField_Map025_CELL_PICTURES:
	.incbin "games/tla/src/map/map_025/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map025_SHAPES:
	.incbin "games/tla/src/map/map_025/shapes.table.lz"
	.balign 4, 0
.LField_Map025_GRID:
	.incbin "games/tla/src/map/map_025/grid.parts.lz"
	.balign 4, 0
.LField_Map025_SHAPE_GRID:
	.incbin "games/tla/src/map/map_025/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map025_ANIMATION:
	.incbin "games/tla/src/map/map_025/animation.script.lz"
	.balign 4, 0
.LField_Map025_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map025_SCENE:
	.incbin "games/tla/src/map/map_025/scene.parts.lz"
