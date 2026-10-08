	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map212
Field_Map212:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map212, .LField_Map212_METATILES
	map_block Field_Map212, .LField_Map212_SHAPES
	map_block Field_Map212, .LField_Map212_GRID
	map_block Field_Map212, .LField_Map212_SHAPE_GRID
	map_block Field_Map212, .LField_Map212_ANIMATION
	map_block Field_Map212, .LField_Map212_BLEND
	map_block Field_Map212, .LField_Map212_SCENE
.LField_Map212_METATILES:
	.incbin "map/map_212/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map212_SHAPES:
	.incbin "map/map_212/shapes.table.lz"
	.balign 4, 0
.LField_Map212_GRID:
	.incbin "map/map_212/grid.parts.lz"
	.balign 4, 0
.LField_Map212_SHAPE_GRID:
	.incbin "map/map_212/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map212_ANIMATION:
	.incbin "map/map_211/animation.script.lz"
	.balign 4, 0
.LField_Map212_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map212_SCENE:
	.incbin "map/map_212/scene.parts.lz"
