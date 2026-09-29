@ Field_Map160: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map160
Field_Map160:
	map_header 0, 0, 68, 88, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map160, .LField_Map160_METATILES
	map_block Field_Map160, .LField_Map160_SHAPES
	map_block Field_Map160, .LField_Map160_GRID
	map_block Field_Map160, .LField_Map160_SHAPE_GRID
	map_block Field_Map160, .LField_Map160_ANIMATION
	map_block Field_Map160, .LField_Map160_BLEND
	map_block Field_Map160, .LField_Map160_SCENE
.LField_Map160_METATILES:
	.incbin "MAP/MAP_160/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map160_SHAPES:
	.incbin "MAP/MAP_160/SHAPES.table.lz"
	.balign 4, 0
.LField_Map160_GRID:
	.incbin "MAP/MAP_160/GRID.parts.lz"
	.balign 4, 0
.LField_Map160_SHAPE_GRID:
	.incbin "MAP/MAP_160/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map160_ANIMATION:
	.incbin "MAP/MAP_160/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map160_BLEND:
	.incbin "MAP/MAP_160/BLEND.script.lz"
	.balign 4, 0
.LField_Map160_SCENE:
	.incbin "MAP/MAP_160/SCENE.parts.lz"
