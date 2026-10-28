	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map121
Field_Map121:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map121, .LField_Map121_CELL_PICTURES
	map_block Field_Map121, .LField_Map121_SHAPES
	map_block Field_Map121, .LField_Map121_GRID
	map_block Field_Map121, .LField_Map121_SHAPE_GRID
	map_block Field_Map121, .LField_Map121_ANIMATION
	map_block Field_Map121, .LField_Map121_BLEND
	map_block Field_Map121, .LField_Map121_SCENE
.LField_Map121_CELL_PICTURES:
	.incbin "games/tla/src/map/map_121/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map121_SHAPES:
	.incbin "games/tla/src/map/map_121/shapes.table.lz"
	.balign 4, 0
.LField_Map121_GRID:
	.incbin "games/tla/src/map/map_121/grid.parts.lz"
	.balign 4, 0
.LField_Map121_SHAPE_GRID:
	.incbin "games/tla/src/map/map_121/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map121_ANIMATION:
	.incbin "games/tla/src/map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map121_BLEND:
	.incbin "games/tla/src/map/map_021/blend.script.lz"
	.balign 4, 0
.LField_Map121_SCENE:
	.incbin "games/tla/src/map/map_121/scene.parts.lz"
	.balign 4, 0
