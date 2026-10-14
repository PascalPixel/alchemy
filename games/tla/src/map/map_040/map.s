	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map040
Field_Map040:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map040, .LField_Map040_METATILES
	map_block Field_Map040, .LField_Map040_SHAPES
	map_block Field_Map040, .LField_Map040_GRID
	map_block Field_Map040, .LField_Map040_SHAPE_GRID
	map_block Field_Map040, .LField_Map040_ANIMATION
	map_block Field_Map040, .LField_Map040_BLEND
	map_block Field_Map040, .LField_Map040_SCENE
.LField_Map040_METATILES:
	.incbin "games/tla/src/map/map_040/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map040_SHAPES:
	.incbin "games/tla/src/map/map_040/shapes.table.lz"
	.balign 4, 0
.LField_Map040_GRID:
	.incbin "games/tla/src/map/map_040/grid.parts.lz"
	.balign 4, 0
.LField_Map040_SHAPE_GRID:
	.incbin "games/tla/src/map/map_040/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map040_ANIMATION:
	.incbin "games/tla/src/map/map_040/animation.script.lz"
	.balign 4, 0
.LField_Map040_BLEND:
	.incbin "games/common/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map040_SCENE:
	.incbin "games/tla/src/map/map_040/scene.parts.lz"
