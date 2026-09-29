@ Field_Map032: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map032
Field_Map032:
	.ifdef TLA_EDITION_JA
	map_header 0, 0, 128, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map032, .LField_Map032_METATILES
	map_block Field_Map032, .LField_Map032_SHAPES
	map_block Field_Map032, .LField_Map032_GRID
	map_block Field_Map032, .LField_Map032_SHAPE_GRID
	map_block Field_Map032, .LField_Map032_ANIMATION
	map_block Field_Map032, .LField_Map032_BLEND
	map_block Field_Map032, .LField_Map032_SCENE
.LField_Map032_METATILES:
	.incbin "MAP/MAP_032/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map032_SHAPES:
	.incbin "MAP/MAP_032/SHAPES.table.lz"
	.balign 4, 0
.LField_Map032_GRID:
	.incbin "MAP/MAP_032/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map032_SHAPE_GRID:
	.incbin "MAP/MAP_032/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map032_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map032_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map032_SCENE:
	.incbin "MAP/MAP_032/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 128, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map032, .LField_Map032_METATILES
	map_block Field_Map032, .LField_Map032_SHAPES
	map_block Field_Map032, .LField_Map032_GRID
	map_block Field_Map032, .LField_Map032_SHAPE_GRID
	map_block Field_Map032, .LField_Map032_ANIMATION
	map_block Field_Map032, .LField_Map032_BLEND
	map_block Field_Map032, .LField_Map032_SCENE
.LField_Map032_METATILES:
	.incbin "MAP/MAP_032/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map032_SHAPES:
	.incbin "MAP/MAP_032/SHAPES.table.lz"
	.balign 4, 0
.LField_Map032_GRID:
	.incbin "MAP/MAP_032/GRID.parts.lz"
	.balign 4, 0
.LField_Map032_SHAPE_GRID:
	.incbin "MAP/MAP_032/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map032_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map032_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map032_SCENE:
	.incbin "MAP/MAP_032/SCENE.parts.lz"
	.endif
