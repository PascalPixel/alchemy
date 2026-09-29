@ Field_Map010: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map010
Field_Map010:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map010, .LField_Map010_METATILES
	map_block Field_Map010, .LField_Map010_SHAPES
	map_block Field_Map010, .LField_Map010_GRID
	map_block Field_Map010, .LField_Map010_SHAPE_GRID
	map_block Field_Map010, .LField_Map010_ANIMATION
	map_block Field_Map010, .LField_Map010_BLEND
	map_block Field_Map010, .LField_Map010_SCENE
.LField_Map010_METATILES:
	.incbin "MAP/MAP_010/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map010_SHAPES:
	.incbin "MAP/MAP_010/SHAPES.table.lz"
	.balign 4, 0
.LField_Map010_GRID:
	.incbin "MAP/MAP_010/GRID.parts.lz"
	.balign 4, 0
.LField_Map010_SHAPE_GRID:
	.incbin "MAP/MAP_010/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map010_ANIMATION:
	.incbin "MAP/MAP_010/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map010_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map010_SCENE:
	.incbin "MAP/MAP_010/SCENE.parts.lz"
