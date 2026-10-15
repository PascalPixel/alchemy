	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map058
Field_Map058:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map058, .LField_Map058_METATILES
	map_block Field_Map058, .LField_Map058_SHAPES
	map_block Field_Map058, .LField_Map058_GRID
	map_block Field_Map058, .LField_Map058_SHAPE_GRID
	map_block Field_Map058, .LField_Map058_ANIMATION
	map_block Field_Map058, .LField_Map058_BLEND
	map_block Field_Map058, .LField_Map058_SCENE
.LField_Map058_METATILES:
	.incbin "games/tla/src/map/map_058/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map058_SHAPES:
	.incbin "games/tla/src/map/map_058/shapes.table.lz"
	.balign 4, 0
.LField_Map058_GRID:
	.incbin "games/tla/src/map/map_058/grid.parts.lz"
	.balign 4, 0
.LField_Map058_SHAPE_GRID:
	.incbin "games/tla/src/map/map_058/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map058_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map058_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map058_SCENE:
	.incbin "games/tla/src/map/map_058/scene.parts.lz"
	.balign 4, 0
