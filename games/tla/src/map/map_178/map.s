	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map178
Field_Map178:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map178, .LField_Map178_METATILES
	map_block Field_Map178, .LField_Map178_SHAPES
	map_block Field_Map178, .LField_Map178_GRID
	map_block Field_Map178, .LField_Map178_SHAPE_GRID
	map_block Field_Map178, .LField_Map178_ANIMATION
	map_block Field_Map178, .LField_Map178_BLEND
	map_block Field_Map178, .LField_Map178_SCENE
.LField_Map178_METATILES:
	.incbin "games/tla/src/map/map_178/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map178_SHAPES:
	.incbin "games/tla/src/map/map_178/shapes.table.lz"
	.balign 4, 0
.LField_Map178_GRID:
	.incbin "games/tla/src/map/map_178/grid.parts.lz"
	.balign 4, 0
.LField_Map178_SHAPE_GRID:
	.incbin "games/tla/src/map/map_178/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map178_ANIMATION:
	.incbin "games/tla/src/map/map_041/animation.script.lz"
	.balign 4, 0
.LField_Map178_BLEND:
	.incbin "games/common/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map178_SCENE:
	.incbin "games/tla/src/map/map_178/scene.parts.lz"
