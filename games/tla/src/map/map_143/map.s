	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map143
Field_Map143:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map143, .LField_Map143_METATILES
	map_block Field_Map143, .LField_Map143_SHAPES
	map_block Field_Map143, .LField_Map143_GRID
	map_block Field_Map143, .LField_Map143_SHAPE_GRID
	map_block Field_Map143, .LField_Map143_ANIMATION
	map_block Field_Map143, .LField_Map143_BLEND
	map_block Field_Map143, .LField_Map143_SCENE
.LField_Map143_METATILES:
	.incbin "map/map_143/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map143_SHAPES:
	.incbin "map/map_143/shapes.table.lz"
	.balign 4, 0
.LField_Map143_GRID:
	.incbin "map/map_143/grid.parts.lz"
	.balign 4, 0
.LField_Map143_SHAPE_GRID:
	.incbin "map/map_143/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map143_ANIMATION:
	.incbin "map/map_142/animation.script.lz"
	.balign 4, 0
.LField_Map143_BLEND:
	.incbin "map/map_143/blend.script.lz"
	.balign 4, 0
.LField_Map143_SCENE:
	.incbin "map/map_143/scene.parts.lz"
