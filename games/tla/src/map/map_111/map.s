	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map111
Field_Map111:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map111, .LField_Map111_METATILES
	map_block Field_Map111, .LField_Map111_SHAPES
	map_block Field_Map111, .LField_Map111_GRID
	map_block Field_Map111, .LField_Map111_SHAPE_GRID
	map_block Field_Map111, .LField_Map111_ANIMATION
	map_block Field_Map111, .LField_Map111_BLEND
	map_block Field_Map111, .LField_Map111_SCENE
.LField_Map111_METATILES:
	.incbin "games/tla/src/map/map_111/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map111_SHAPES:
	.incbin "games/tla/src/map/map_109/shapes.table.lz"
	.balign 4, 0
.LField_Map111_GRID:
	.incbin "games/tla/src/map/map_111/grid.parts.lz"
	.balign 4, 0
.LField_Map111_SHAPE_GRID:
	.incbin "games/tla/src/map/map_111/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map111_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map111_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map111_SCENE:
	.incbin "games/tla/src/map/map_111/scene.parts.lz"
