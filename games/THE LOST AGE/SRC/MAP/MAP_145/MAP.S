@ Field_Map145: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map145
Field_Map145:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map145, .LField_Map145_METATILES
	map_block Field_Map145, .LField_Map145_SHAPES
	map_block Field_Map145, .LField_Map145_GRID
	map_block Field_Map145, .LField_Map145_SHAPE_GRID
	map_block Field_Map145, .LField_Map145_ANIMATION
	map_block Field_Map145, .LField_Map145_BLEND
	map_block Field_Map145, .LField_Map145_SCENE
.LField_Map145_METATILES:
	.incbin "MAP/MAP_145/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map145_SHAPES:
	.incbin "MAP/MAP_145/SHAPES.table.lz"
	.balign 4, 0
.LField_Map145_GRID:
	.incbin "MAP/MAP_145/GRID.parts.lz"
	.balign 4, 0
.LField_Map145_SHAPE_GRID:
	.incbin "MAP/MAP_145/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map145_ANIMATION:
	.incbin "MAP/MAP_145/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map145_BLEND:
	.incbin "MAP/MAP_145/BLEND.script.lz"
	.balign 4, 0
.LField_Map145_SCENE:
	.incbin "MAP/MAP_145/SCENE.parts.lz"
