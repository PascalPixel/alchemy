@ Field_Map016: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map016
Field_Map016:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map016, .LField_Map016_METATILES
	map_block Field_Map016, .LField_Map016_SHAPES
	map_block Field_Map016, .LField_Map016_GRID
	map_block Field_Map016, .LField_Map016_SHAPE_GRID
	map_block Field_Map016, .LField_Map016_ANIMATION
	map_block Field_Map016, .LField_Map016_BLEND
	map_block Field_Map016, .LField_Map016_SCENE
.LField_Map016_METATILES:
	.incbin "MAP/MAP_016/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map016_SHAPES:
	.incbin "MAP/MAP_016/SHAPES.table.lz"
	.balign 4, 0
.LField_Map016_GRID:
	.incbin "MAP/MAP_016/GRID.parts.lz"
	.balign 4, 0
.LField_Map016_SHAPE_GRID:
	.incbin "MAP/MAP_016/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map016_ANIMATION:
	.incbin "MAP/MAP_016/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map016_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map016_SCENE:
	.incbin "MAP/MAP_016/SCENE.parts.lz"
