	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map174
Field_Map174:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map174, .LField_Map174_METATILES
	map_block Field_Map174, .LField_Map174_SHAPES
	map_block Field_Map174, .LField_Map174_GRID
	map_block Field_Map174, .LField_Map174_SHAPE_GRID
	map_block Field_Map174, .LField_Map174_ANIMATION
	map_block Field_Map174, .LField_Map174_BLEND
	map_block Field_Map174, .LField_Map174_SCENE
.LField_Map174_METATILES:
	.incbin "map/map_174/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map174_SHAPES:
	.incbin "map/map_174/shapes.table.lz"
	.balign 4, 0
.LField_Map174_GRID:
	.incbin "map/map_174/grid.parts.lz"
	.balign 4, 0
.LField_Map174_SHAPE_GRID:
	.incbin "map/map_174/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map174_ANIMATION:
	.incbin "map/map_174/animation.script.lz"
	.balign 4, 0
.LField_Map174_BLEND:
	.incbin "map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map174_SCENE:
	.incbin "map/map_174/scene.parts.lz"
