@ Field_Map035: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map035
Field_Map035:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035, .LField_Map035_SHAPE_GRID
	map_block Field_Map035, .LField_Map035_ANIMATION
	map_block Field_Map035, .LField_Map035_BLEND
	map_block Field_Map035, .LField_Map035_SCENE
.LField_Map035_METATILES:
	.incbin "MAP/MAP_035/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "MAP/MAP_035/SHAPES.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "MAP/MAP_035/GRID.parts.lz"
	.balign 4, 0
.LField_Map035_SHAPE_GRID:
	.incbin "MAP/MAP_035/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map035_ANIMATION:
	.incbin "MAP/MAP_035/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map035_BLEND:
	.incbin "MAP/MAP_035/BLEND.script.lz"
	.balign 4, 0
.LField_Map035_SCENE:
	.incbin "MAP/MAP_035/SCENE.parts.lz"
