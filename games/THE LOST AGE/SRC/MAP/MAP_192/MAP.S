@ Field_Map192: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map192
Field_Map192:
	.ifdef TLA_EDITION_JA
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map192, .LField_Map192_METATILES
	map_block Field_Map192, .LField_Map192_SHAPES
	map_block Field_Map192, .LField_Map192_GRID
	map_block Field_Map192, .LField_Map192_SHAPE_GRID
	map_block Field_Map192, .LField_Map192_ANIMATION
	map_block Field_Map192
	map_block Field_Map192, .LField_Map192_SCENE
.LField_Map192_METATILES:
	.incbin "MAP/MAP_192/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map192_SHAPES:
	.incbin "MAP/MAP_192/SHAPES.table.lz"
	.balign 4, 0
.LField_Map192_GRID:
	.incbin "MAP/MAP_192/GRID.parts.lz"
	.balign 4, 0
.LField_Map192_SHAPE_GRID:
	.incbin "MAP/MAP_192/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map192_ANIMATION:
	.incbin "MAP/MAP_192/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map192_SCENE:
	.incbin "MAP/MAP_192/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map192, .LField_Map192_METATILES
	map_block Field_Map192, .LField_Map192_SHAPES
	map_block Field_Map192, .LField_Map192_GRID
	map_block Field_Map192, .LField_Map192_SHAPE_GRID
	map_block Field_Map192, .LField_Map192_ANIMATION
	map_block Field_Map192
	map_block Field_Map192, .LField_Map192_SCENE
.LField_Map192_METATILES:
	.incbin "MAP/MAP_192/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map192_SHAPES:
	.incbin "MAP/MAP_192/SHAPES.table.lz"
	.balign 4, 0
.LField_Map192_GRID:
	.incbin "MAP/MAP_192/GRID.parts.lz"
	.balign 4, 0
.LField_Map192_SHAPE_GRID:
	.incbin "MAP/MAP_192/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map192_ANIMATION:
	.incbin "MAP/MAP_192/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map192_SCENE:
	.incbin "MAP/MAP_192/SCENE.parts.lz"
	.endif
