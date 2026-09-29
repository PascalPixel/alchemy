@ Field_Map168: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map168
Field_Map168:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map168, .LField_Map168_METATILES
	map_block Field_Map168, .LField_Map168_SHAPES
	map_block Field_Map168, .LField_Map168_GRID
	map_block Field_Map168, .LField_Map168_SHAPE_GRID
	map_block Field_Map168, .LField_Map168_ANIMATION
	map_block Field_Map168, .LField_Map168_BLEND
	map_block Field_Map168, .LField_Map168_SCENE
.LField_Map168_METATILES:
	.incbin "MAP/MAP_168/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map168_SHAPES:
	.incbin "MAP/MAP_168/SHAPES.table.lz"
	.balign 4, 0
.LField_Map168_GRID:
	.incbin "MAP/MAP_168/GRID.parts.lz"
	.balign 4, 0
.LField_Map168_SHAPE_GRID:
	.incbin "MAP/MAP_168/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map168_ANIMATION:
	.incbin "MAP/MAP_168/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map168_BLEND:
	.incbin "MAP/MAP_168/BLEND.script.lz"
	.balign 4, 0
.LField_Map168_SCENE:
	.incbin "MAP/MAP_168/SCENE.parts.lz"
