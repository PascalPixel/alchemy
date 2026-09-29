@ Field_Map257: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map257
Field_Map257:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map257, .LField_Map257_METATILES
	map_block Field_Map257, .LField_Map257_SHAPES
	map_block Field_Map257, .LField_Map257_GRID
	map_block Field_Map257, .LField_Map257_SHAPE_GRID
	map_block Field_Map257, .LField_Map257_ANIMATION
	map_block Field_Map257, .LField_Map257_BLEND
	map_block Field_Map257, .LField_Map257_SCENE
.LField_Map257_METATILES:
	.incbin "MAP/MAP_257/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map257_SHAPES:
	.incbin "MAP/MAP_257/SHAPES.table.lz"
	.balign 4, 0
.LField_Map257_GRID:
	.incbin "MAP/MAP_257/GRID.parts.lz"
	.balign 4, 0
.LField_Map257_SHAPE_GRID:
	.incbin "MAP/MAP_257/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map257_ANIMATION:
	.incbin "MAP/MAP_206/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map257_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map257_SCENE:
	.incbin "MAP/MAP_257/SCENE.parts.lz"
