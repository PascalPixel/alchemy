@ Field_Map156: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map156
Field_Map156:
	map_header 0, 0, 64, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map156, .LField_Map156_METATILES
	map_block Field_Map156, .LField_Map156_SHAPES
	map_block Field_Map156, .LField_Map156_GRID
	map_block Field_Map156, .LField_Map156_SHAPE_GRID
	map_block Field_Map156, .LField_Map156_ANIMATION
	map_block Field_Map156, .LField_Map156_BLEND
	map_block Field_Map156, .LField_Map156_SCENE
.LField_Map156_METATILES:
	.incbin "MAP/MAP_156/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map156_SHAPES:
	.incbin "MAP/MAP_156/SHAPES.table.lz"
	.balign 4, 0
.LField_Map156_GRID:
	.incbin "MAP/MAP_156/GRID.parts.lz"
	.balign 4, 0
.LField_Map156_SHAPE_GRID:
	.incbin "MAP/MAP_156/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map156_ANIMATION:
	.incbin "MAP/MAP_156/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map156_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map156_SCENE:
	.incbin "MAP/MAP_156/SCENE.parts.lz"
