	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map179
Field_Map179:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map179, .LField_Map179_METATILES
	map_block Field_Map179, .LField_Map179_SHAPES
	map_block Field_Map179, .LField_Map179_GRID
	map_block Field_Map179, .LField_Map179_SHAPE_GRID
	map_block Field_Map179, .LField_Map179_ANIMATION
	map_block Field_Map179, .LField_Map179_BLEND
	map_block Field_Map179, .LField_Map179_SCENE
.LField_Map179_METATILES:
	.incbin "map/map_179/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map179_SHAPES:
	.incbin "common/map/map_015/shapes.table.lz"
	.balign 4, 0
.LField_Map179_GRID:
	.incbin "map/map_179/grid.parts.lz"
	.balign 4, 0
.LField_Map179_SHAPE_GRID:
	.incbin "map/map_179/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map179_ANIMATION:
	.incbin "map/map_179/animation.script.lz"
	.balign 4, 0
.LField_Map179_BLEND:
	.incbin "map/map_179/blend.script.lz"
	.balign 4, 0
.LField_Map179_SCENE:
	.incbin "map/map_179/scene.parts.lz"
