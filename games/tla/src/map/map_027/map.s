	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map027
Field_Map027:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map027, .LField_Map027_METATILES
	map_block Field_Map027, .LField_Map027_SHAPES
	map_block Field_Map027, .LField_Map027_GRID
	map_block Field_Map027, .LField_Map027_SHAPE_GRID
	map_block Field_Map027, .LField_Map027_ANIMATION
	map_block Field_Map027, .LField_Map027_BLEND
	map_block Field_Map027, .LField_Map027_SCENE
.LField_Map027_METATILES:
	.incbin "games/tla/src/map/map_027/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map027_SHAPES:
	.incbin "games/tla/src/map/map_027/shapes.table.lz"
	.balign 4, 0
.LField_Map027_GRID:
	.incbin "games/tla/src/map/map_027/grid.parts.lz"
	.balign 4, 0
.LField_Map027_SHAPE_GRID:
	.incbin "games/tla/src/map/map_027/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map027_ANIMATION:
	.incbin "games/tla/src/map/map_027/animation.script.lz"
	.balign 4, 0
.LField_Map027_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map027_SCENE:
	.incbin "games/tla/src/map/map_027/scene.parts.lz"
