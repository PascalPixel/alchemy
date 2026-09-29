@ Field_Map203: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map203
Field_Map203:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map203, .LField_Map203_METATILES
	map_block Field_Map203, .LField_Map203_SHAPES
	map_block Field_Map203, .LField_Map203_GRID
	map_block Field_Map203, .LField_Map203_SHAPE_GRID
	map_block Field_Map203, .LField_Map203_ANIMATION
	map_block Field_Map203, .LField_Map203_BLEND
	map_block Field_Map203, .LField_Map203_SCENE
.LField_Map203_METATILES:
	.incbin "MAP/MAP_203/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map203_SHAPES:
	.incbin "MAP/MAP_203/SHAPES.table.lz"
	.balign 4, 0
.LField_Map203_GRID:
	.incbin "MAP/MAP_203/GRID.parts.lz"
	.balign 4, 0
.LField_Map203_SHAPE_GRID:
	.incbin "MAP/MAP_203/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map203_ANIMATION:
	.incbin "MAP/MAP_203/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map203_BLEND:
	.incbin "MAP/MAP_203/BLEND.script.lz"
	.balign 4, 0
.LField_Map203_SCENE:
	.incbin "MAP/MAP_203/SCENE.parts.lz"
