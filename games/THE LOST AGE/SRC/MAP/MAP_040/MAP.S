@ Field_Map040: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map040
Field_Map040:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map040, .LField_Map040_METATILES
	map_block Field_Map040, .LField_Map040_SHAPES
	map_block Field_Map040, .LField_Map040_GRID
	map_block Field_Map040, .LField_Map040_SHAPE_GRID
	map_block Field_Map040, .LField_Map040_ANIMATION
	map_block Field_Map040, .LField_Map040_BLEND
	map_block Field_Map040, .LField_Map040_SCENE
.LField_Map040_METATILES:
	.incbin "MAP/MAP_040/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map040_SHAPES:
	.incbin "MAP/MAP_040/SHAPES.table.lz"
	.balign 4, 0
.LField_Map040_GRID:
	.incbin "MAP/MAP_040/GRID.parts.lz"
	.balign 4, 0
.LField_Map040_SHAPE_GRID:
	.incbin "MAP/MAP_040/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map040_ANIMATION:
	.incbin "MAP/MAP_040/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map040_BLEND:
	.incbin "MAP/MAP_040/BLEND.script.lz"
	.balign 4, 0
.LField_Map040_SCENE:
	.incbin "MAP/MAP_040/SCENE.parts.lz"
