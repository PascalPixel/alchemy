@ Field_Map171: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map171
Field_Map171:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map171, .LField_Map171_METATILES
	map_block Field_Map171, .LField_Map171_SHAPES
	map_block Field_Map171, .LField_Map171_GRID
	map_block Field_Map171, .LField_Map171_SHAPE_GRID
	map_block Field_Map171, .LField_Map171_ANIMATION
	map_block Field_Map171, .LField_Map171_BLEND
	map_block Field_Map171, .LField_Map171_SCENE
.LField_Map171_METATILES:
	.incbin "MAP/MAP_171/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map171_SHAPES:
	.incbin "MAP/MAP_171/SHAPES.table.lz"
	.balign 4, 0
.LField_Map171_GRID:
	.incbin "MAP/MAP_171/GRID.parts.lz"
	.balign 4, 0
.LField_Map171_SHAPE_GRID:
	.incbin "MAP/MAP_171/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map171_ANIMATION:
	.incbin "MAP/MAP_168/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map171_BLEND:
	.incbin "MAP/MAP_169/BLEND.script.lz"
	.balign 4, 0
.LField_Map171_SCENE:
	.incbin "MAP/MAP_171/SCENE.parts.lz"
