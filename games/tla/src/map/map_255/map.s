	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map255
Field_Map255:
	map_header 0, 0, 90, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map255, .LField_Map255_METATILES
	map_block Field_Map255, .LField_Map255_SHAPES
	map_block Field_Map255, .LField_Map255_GRID
	map_block Field_Map255, .LField_Map255_SHAPE_GRID
	map_block Field_Map255, .LField_Map255_ANIMATION
	map_block Field_Map255, .LField_Map255_BLEND
	map_block Field_Map255, .LField_Map255_SCENE
.LField_Map255_METATILES:
	.incbin "map/map_255/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map255_SHAPES:
	.incbin "map/map_255/shapes.table.lz"
	.balign 4, 0
.LField_Map255_GRID:
	.incbin "map/map_255/grid.parts.lz"
	.balign 4, 0
.LField_Map255_SHAPE_GRID:
	.incbin "map/map_255/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map255_ANIMATION:
	.incbin "map/map_254/animation.script.lz"
	.balign 4, 0
.LField_Map255_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map255_SCENE:
	.incbin "map/map_255/scene.parts.lz"
