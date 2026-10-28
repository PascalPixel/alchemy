	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map215
Field_Map215:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map215, .LField_Map215_CELL_PICTURES
	map_block Field_Map215, .LField_Map215_SHAPES
	map_block Field_Map215, .LField_Map215_GRID
	map_block Field_Map215, .LField_Map215_SHAPE_GRID
	map_block Field_Map215, .LField_Map215_ANIMATION
	map_block Field_Map215, .LField_Map215_BLEND
	map_block Field_Map215, .LField_Map215_SCENE
.LField_Map215_CELL_PICTURES:
	.incbin "games/tla/src/map/map_215/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map215_SHAPES:
	.incbin "games/tla/src/map/map_215/shapes.table.lz"
	.balign 4, 0
.LField_Map215_GRID:
	.incbin "games/tla/src/map/map_215/grid.parts.lz"
	.balign 4, 0
.LField_Map215_SHAPE_GRID:
	.incbin "games/tla/src/map/map_215/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map215_ANIMATION:
	.incbin "games/tla/src/map/map_211/animation.script.lz"
	.balign 4, 0
.LField_Map215_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map215_SCENE:
	.incbin "games/tla/src/map/map_215/scene.parts.lz"
	.balign 4, 0
