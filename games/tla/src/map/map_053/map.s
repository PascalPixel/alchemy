	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map053
Field_Map053:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map053, .LField_Map053_METATILES
	map_block Field_Map053, .LField_Map053_SHAPES
	map_block Field_Map053, .LField_Map053_GRID
	map_block Field_Map053, .LField_Map053_SHAPE_GRID
	map_block Field_Map053, .LField_Map053_ANIMATION
	map_block Field_Map053, .LField_Map053_BLEND
	map_block Field_Map053, .LField_Map053_SCENE
.LField_Map053_METATILES:
	.incbin "map/map_053/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map053_SHAPES:
	.incbin "map/map_053/shapes.table.lz"
	.balign 4, 0
.LField_Map053_GRID:
	.incbin "map/map_053/grid.parts.lz"
	.balign 4, 0
.LField_Map053_SHAPE_GRID:
	.incbin "map/map_053/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map053_ANIMATION:
	.incbin "map/map_041/animation.script.lz"
	.balign 4, 0
.LField_Map053_BLEND:
	.incbin "common/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map053_SCENE:
	.incbin "map/map_053/scene.parts.lz"
