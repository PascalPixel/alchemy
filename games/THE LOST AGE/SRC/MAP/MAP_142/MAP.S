@ Field_Map142: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map142
Field_Map142:
	.ifdef TLA_EDITION_JA
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map142, .LField_Map142_METATILES
	map_block Field_Map142, .LField_Map142_SHAPES
	map_block Field_Map142, .LField_Map142_GRID
	map_block Field_Map142, .LField_Map142_SHAPE_GRID
	map_block Field_Map142, .LField_Map142_ANIMATION
	map_block Field_Map142, .LField_Map142_BLEND
	map_block Field_Map142, .LField_Map142_SCENE
.LField_Map142_METATILES:
	.incbin "MAP/MAP_142/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map142_SHAPES:
	.incbin "MAP/MAP_142/SHAPES.table.lz"
	.balign 4, 0
.LField_Map142_GRID:
	.incbin "MAP/MAP_142/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map142_SHAPE_GRID:
	.incbin "MAP/MAP_142/SHAPE_GRID_JA.plane.lz"
	.balign 4, 0
.LField_Map142_ANIMATION:
	.incbin "MAP/MAP_142/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map142_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map142_SCENE:
	.incbin "MAP/MAP_142/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map142, .LField_Map142_METATILES
	map_block Field_Map142, .LField_Map142_SHAPES
	map_block Field_Map142, .LField_Map142_GRID
	map_block Field_Map142, .LField_Map142_SHAPE_GRID
	map_block Field_Map142, .LField_Map142_ANIMATION
	map_block Field_Map142, .LField_Map142_BLEND
	map_block Field_Map142, .LField_Map142_SCENE
.LField_Map142_METATILES:
	.incbin "MAP/MAP_142/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map142_SHAPES:
	.incbin "MAP/MAP_142/SHAPES.table.lz"
	.balign 4, 0
.LField_Map142_GRID:
	.incbin "MAP/MAP_142/GRID.parts.lz"
	.balign 4, 0
.LField_Map142_SHAPE_GRID:
	.incbin "MAP/MAP_142/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map142_ANIMATION:
	.incbin "MAP/MAP_142/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map142_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map142_SCENE:
	.incbin "MAP/MAP_142/SCENE.parts.lz"
	.endif
