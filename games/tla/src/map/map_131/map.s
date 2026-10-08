	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map131
Field_Map131:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map131, .LField_Map131_METATILES
	map_block Field_Map131, .LField_Map131_SHAPES
	map_block Field_Map131, .LField_Map131_GRID
	map_block Field_Map131, .LField_Map131_SHAPE_GRID
	map_block Field_Map131, .LField_Map131_ANIMATION
	map_block Field_Map131, .LField_Map131_BLEND
	map_block Field_Map131, .LField_Map131_SCENE
.LField_Map131_METATILES:
	.incbin "map/map_131/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map131_SHAPES:
	.incbin "map/map_131/shapes.table.lz"
	.balign 4, 0
.LField_Map131_GRID:
	.incbin "map/map_131/grid.parts.lz"
	.balign 4, 0
.LField_Map131_SHAPE_GRID:
	.incbin "map/map_131/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map131_ANIMATION:
	.incbin "map/map_131/animation.script.lz"
	.balign 4, 0
.LField_Map131_BLEND:
	.incbin "common/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map131_SCENE:
	.incbin "map/map_131/scene.parts.lz"
