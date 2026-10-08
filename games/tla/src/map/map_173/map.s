	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map173
Field_Map173:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map173, .LField_Map173_METATILES
	map_block Field_Map173, .LField_Map173_SHAPES
	map_block Field_Map173, .LField_Map173_GRID
	map_block Field_Map173, .LField_Map173_SHAPE_GRID
	map_block Field_Map173, .LField_Map173_ANIMATION
	map_block Field_Map173, .LField_Map173_BLEND
	map_block Field_Map173, .LField_Map173_SCENE
.LField_Map173_METATILES:
	.incbin "map/map_173/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map173_SHAPES:
	.incbin "map/map_173/shapes.table.lz"
	.balign 4, 0
.LField_Map173_GRID:
	.incbin "map/map_173/grid.parts.lz"
	.balign 4, 0
.LField_Map173_SHAPE_GRID:
	.incbin "map/map_173/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map173_ANIMATION:
	.incbin "map/map_161/animation.script.lz"
	.balign 4, 0
.LField_Map173_BLEND:
	.incbin "map/map_161/blend.script.lz"
	.balign 4, 0
.LField_Map173_SCENE:
	.incbin "map/map_173/scene.parts.lz"
