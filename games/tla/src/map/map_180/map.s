	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map180
Field_Map180:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map180, .LField_Map180_CELL_PICTURES
	map_block Field_Map180, .LField_Map180_SHAPES
	map_block Field_Map180, .LField_Map180_GRID
	map_block Field_Map180, .LField_Map180_SHAPE_GRID
	map_block Field_Map180, .LField_Map180_ANIMATION
	map_block Field_Map180, .LField_Map180_BLEND
	map_block Field_Map180, .LField_Map180_SCENE
.LField_Map180_CELL_PICTURES:
	.incbin "games/tla/src/map/map_180/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map180_SHAPES:
	.incbin "games/tla/src/map/map_180/shapes.table.lz"
	.balign 4, 0
.LField_Map180_GRID:
	.incbin "games/tla/src/map/map_180/grid.parts.lz"
	.balign 4, 0
.LField_Map180_SHAPE_GRID:
	.incbin "games/tla/src/map/map_180/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map180_ANIMATION:
	.incbin "games/tla/src/map/map_179/animation.script.lz"
	.balign 4, 0
.LField_Map180_BLEND:
	.incbin "games/tla/src/map/map_179/blend.script.lz"
	.balign 4, 0
.LField_Map180_SCENE:
	.incbin "games/tla/src/map/map_180/scene.parts.lz"
	.balign 4, 0
