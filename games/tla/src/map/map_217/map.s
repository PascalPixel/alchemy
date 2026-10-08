	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map217
Field_Map217:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map217, .LField_Map217_METATILES
	map_block Field_Map217, .LField_Map217_SHAPES
	map_block Field_Map217, .LField_Map217_GRID
	map_block Field_Map217, .LField_Map217_SHAPE_GRID
	map_block Field_Map217, .LField_Map217_ANIMATION
	map_block Field_Map217
	map_block Field_Map217, .LField_Map217_SCENE
.LField_Map217_METATILES:
	.incbin "map/map_217/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map217_SHAPES:
	.incbin "map/map_217/shapes.table.lz"
	.balign 4, 0
.LField_Map217_GRID:
	.incbin "map/map_217/grid.parts.lz"
	.balign 4, 0
.LField_Map217_SHAPE_GRID:
	.incbin "map/map_217/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map217_ANIMATION:
	.incbin "map/map_217/animation.script.lz"
	.balign 4, 0
.LField_Map217_SCENE:
	.incbin "map/map_217/scene.parts.lz"
