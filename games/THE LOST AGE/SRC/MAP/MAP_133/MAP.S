@ Field_Map133: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map133
Field_Map133:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_METATILES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_SHAPE_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_SCENE
.LField_Map133_METATILES:
	.incbin "MAP/MAP_133/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "MAP/MAP_133/SHAPES.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "MAP/MAP_133/GRID.parts.lz"
	.balign 4, 0
.LField_Map133_SHAPE_GRID:
	.incbin "MAP/MAP_133/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "MAP/MAP_133/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "MAP/MAP_133/BLEND.script.lz"
	.balign 4, 0
.LField_Map133_SCENE:
	.incbin "MAP/MAP_133/SCENE.parts.lz"
