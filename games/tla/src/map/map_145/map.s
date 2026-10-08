	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map145
Field_Map145:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map145, .LField_Map145_METATILES
	map_block Field_Map145, .LField_Map145_SHAPES
	map_block Field_Map145, .LField_Map145_GRID
	map_block Field_Map145, .LField_Map145_SHAPE_GRID
	map_block Field_Map145, .LField_Map145_ANIMATION
	map_block Field_Map145, .LField_Map145_BLEND
	map_block Field_Map145, .LField_Map145_SCENE
.LField_Map145_METATILES:
	.incbin "map/map_145/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map145_SHAPES:
	.incbin "map/map_145/shapes.table.lz"
	.balign 4, 0
.LField_Map145_GRID:
	.incbin "map/map_145/grid.parts.lz"
	.balign 4, 0
.LField_Map145_SHAPE_GRID:
	.incbin "map/map_145/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map145_ANIMATION:
	.incbin "map/map_145/animation.script.lz"
	.balign 4, 0
.LField_Map145_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map145_SCENE:
	.incbin "map/map_145/scene.parts.lz"
