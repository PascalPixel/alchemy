@ Field_Map161: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map161
Field_Map161:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map161, .LField_Map161_METATILES
	map_block Field_Map161, .LField_Map161_SHAPES
	map_block Field_Map161, .LField_Map161_GRID
	map_block Field_Map161, .LField_Map161_SHAPE_GRID
	map_block Field_Map161, .LField_Map161_ANIMATION
	map_block Field_Map161, .LField_Map161_BLEND
	map_block Field_Map161, .LField_Map161_SCENE
.LField_Map161_METATILES:
	.incbin "MAP/MAP_161/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map161_SHAPES:
	.incbin "MAP/MAP_161/SHAPES.table.lz"
	.balign 4, 0
.LField_Map161_GRID:
	.incbin "MAP/MAP_161/GRID.parts.lz"
	.balign 4, 0
.LField_Map161_SHAPE_GRID:
	.incbin "MAP/MAP_161/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map161_ANIMATION:
	.incbin "MAP/MAP_161/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map161_BLEND:
	.incbin "MAP/MAP_161/BLEND.script.lz"
	.balign 4, 0
.LField_Map161_SCENE:
	.incbin "MAP/MAP_161/SCENE.parts.lz"
