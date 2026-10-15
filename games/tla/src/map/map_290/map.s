	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map290
Field_Map290:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map290, .LField_Map290_METATILES
	map_block Field_Map290, .LField_Map290_SHAPES
	map_block Field_Map290, .LField_Map290_GRID
	map_block Field_Map290, .LField_Map290_SHAPE_GRID
	map_block Field_Map290, .LField_Map290_ANIMATION
	map_block Field_Map290, .LField_Map290_BLEND
	map_block Field_Map290, .LField_Map290_SCENE
.LField_Map290_METATILES:
	.incbin "games/tla/src/map/map_290/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map290_SHAPES:
	.incbin "games/tla/src/map/map_290/shapes.table.lz"
	.balign 4, 0
.LField_Map290_GRID:
	.incbin "games/tla/src/map/map_290/grid.parts.lz"
	.balign 4, 0
.LField_Map290_SHAPE_GRID:
	.incbin "games/tla/src/map/map_290/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map290_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map290_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map290_SCENE:
	.incbin "games/tla/src/map/map_290/scene.parts.lz"
	.balign 4, 0
