	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map093
Field_Map093:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map093, .LField_Map093_METATILES
	map_block Field_Map093, .LField_Map093_SHAPES
	map_block Field_Map093, .LField_Map093_GRID
	map_block Field_Map093, .LField_Map093_SHAPE_GRID
	map_block Field_Map093, .LField_Map093_ANIMATION
	map_block Field_Map093, .LField_Map093_BLEND
	map_block Field_Map093, .LField_Map093_SCENE
.LField_Map093_METATILES:
	.incbin "games/tla/src/map/map_093/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map093_SHAPES:
	.incbin "games/tla/src/map/map_093/shapes.table.lz"
	.balign 4, 0
.LField_Map093_GRID:
	.incbin "games/tla/src/map/map_093/grid.parts.lz"
	.balign 4, 0
.LField_Map093_SHAPE_GRID:
	.incbin "games/tla/src/map/map_093/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map093_ANIMATION:
	.incbin "games/tla/src/map/map_093/animation.script.lz"
	.balign 4, 0
.LField_Map093_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map093_SCENE:
	.incbin "games/tla/src/map/map_093/scene.parts.lz"
