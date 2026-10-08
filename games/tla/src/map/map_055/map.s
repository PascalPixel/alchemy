	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map055
Field_Map055:
	map_header 0, 0, 128, 128, 2, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 11, 14, 0, 0, 0x1f, 0x1f
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map055, .LField_Map055_METATILES
	map_block Field_Map055, .LField_Map055_SHAPES
	map_block Field_Map055, .LField_Map055_GRID
	map_block Field_Map055, .LField_Map055_SHAPE_GRID
	map_block Field_Map055, .LField_Map055_ANIMATION
	map_block Field_Map055, .LField_Map055_BLEND
	map_block Field_Map055, .LField_Map055_SCENE
.LField_Map055_METATILES:
	.incbin "map/map_055/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map055_SHAPES:
	.incbin "map/map_055/shapes.table.lz"
	.balign 4, 0
.LField_Map055_GRID:
	.incbin "map/map_055/grid.parts.lz"
	.balign 4, 0
.LField_Map055_SHAPE_GRID:
	.incbin "map/map_055/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map055_ANIMATION:
	.incbin "map/map_055/animation.script.lz"
	.balign 4, 0
.LField_Map055_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map055_SCENE:
	.incbin "map/map_055/scene.parts.lz"
