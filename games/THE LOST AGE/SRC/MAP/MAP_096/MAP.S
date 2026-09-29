@ Field_Map096: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map096
Field_Map096:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map096, .LField_Map096_METATILES
	map_block Field_Map096, .LField_Map096_SHAPES
	map_block Field_Map096, .LField_Map096_GRID
	map_block Field_Map096, .LField_Map096_SHAPE_GRID
	map_block Field_Map096, .LField_Map096_ANIMATION
	map_block Field_Map096, .LField_Map096_BLEND
	map_block Field_Map096, .LField_Map096_SCENE
.LField_Map096_METATILES:
	.incbin "MAP/MAP_096/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map096_SHAPES:
	.incbin "MAP/MAP_096/SHAPES.table.lz"
	.balign 4, 0
.LField_Map096_GRID:
	.incbin "MAP/MAP_096/GRID.parts.lz"
	.balign 4, 0
.LField_Map096_SHAPE_GRID:
	.incbin "MAP/MAP_096/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map096_ANIMATION:
	.incbin "MAP/MAP_096/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map096_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map096_SCENE:
	.incbin "MAP/MAP_096/SCENE.parts.lz"
