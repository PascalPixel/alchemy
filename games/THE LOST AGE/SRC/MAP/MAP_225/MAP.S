@ Field_Map225: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map225
Field_Map225:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map225, .LField_Map225_METATILES
	map_block Field_Map225, .LField_Map225_SHAPES
	map_block Field_Map225, .LField_Map225_GRID
	map_block Field_Map225, .LField_Map225_SHAPE_GRID
	map_block Field_Map225, .LField_Map225_ANIMATION
	map_block Field_Map225, .LField_Map225_BLEND
	map_block Field_Map225, .LField_Map225_SCENE
.LField_Map225_METATILES:
	.incbin "MAP/MAP_225/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map225_SHAPES:
	.incbin "MAP/MAP_225/SHAPES.table.lz"
	.balign 4, 0
.LField_Map225_GRID:
	.incbin "MAP/MAP_225/GRID.parts.lz"
	.balign 4, 0
.LField_Map225_SHAPE_GRID:
	.incbin "MAP/MAP_225/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map225_ANIMATION:
	.incbin "MAP/MAP_225/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map225_BLEND:
	.incbin "MAP/MAP_225/BLEND.script.lz"
	.balign 4, 0
.LField_Map225_SCENE:
	.incbin "MAP/MAP_225/SCENE.parts.lz"
