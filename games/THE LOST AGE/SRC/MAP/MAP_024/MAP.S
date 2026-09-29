@ Field_Map024: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map024
Field_Map024:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map024, .LField_Map024_METATILES
	map_block Field_Map024, .LField_Map024_SHAPES
	map_block Field_Map024, .LField_Map024_GRID
	map_block Field_Map024, .LField_Map024_SHAPE_GRID
	map_block Field_Map024, .LField_Map024_ANIMATION
	map_block Field_Map024, .LField_Map024_BLEND
	map_block Field_Map024, .LField_Map024_SCENE
.LField_Map024_METATILES:
	.incbin "MAP/MAP_024/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "MAP/MAP_024/SHAPES.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "MAP/MAP_024/GRID.parts.lz"
	.balign 4, 0
.LField_Map024_SHAPE_GRID:
	.incbin "MAP/MAP_024/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "MAP/MAP_024/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map024_SCENE:
	.incbin "MAP/MAP_024/SCENE.parts.lz"
