	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map198
Field_Map198:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map198, .LField_Map198_METATILES
	map_block Field_Map198, .LField_Map198_SHAPES
	map_block Field_Map198, .LField_Map198_GRID
	map_block Field_Map198, .LField_Map198_SHAPE_GRID
	map_block Field_Map198, .LField_Map198_ANIMATION
	map_block Field_Map198, .LField_Map198_BLEND
	map_block Field_Map198, .LField_Map198_SCENE
.LField_Map198_METATILES:
	.incbin "games/tla/src/map/map_198/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map198_SHAPES:
	.incbin "games/tla/src/map/map_198/shapes.table.lz"
	.balign 4, 0
.LField_Map198_GRID:
	.incbin "games/tla/src/map/map_198/grid.parts.lz"
	.balign 4, 0
.LField_Map198_SHAPE_GRID:
	.incbin "games/tla/src/map/map_198/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map198_ANIMATION:
	.incbin "games/tla/src/map/map_195/animation.script.lz"
	.balign 4, 0
.LField_Map198_BLEND:
	.incbin "games/tla/src/map/map_196/blend.script.lz"
	.balign 4, 0
.LField_Map198_SCENE:
	.incbin "games/tla/src/map/map_198/scene.parts.lz"
	.balign 4, 0
