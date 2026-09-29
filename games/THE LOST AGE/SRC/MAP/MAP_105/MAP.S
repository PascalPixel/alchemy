@ Field_Map105: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map105
Field_Map105:
	.ifdef TLA_EDITION_JA
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_METATILES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_SHAPE_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_SCENE
.LField_Map105_METATILES:
	.incbin "MAP/MAP_105/METATILES_JA.delta1.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "MAP/MAP_103/SHAPES.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "MAP/MAP_105/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map105_SHAPE_GRID:
	.incbin "MAP/MAP_103/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map105_SCENE:
	.incbin "MAP/MAP_105/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_METATILES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_SHAPE_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_SCENE
.LField_Map105_METATILES:
	.incbin "MAP/MAP_105/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "MAP/MAP_103/SHAPES.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "MAP/MAP_105/GRID.parts.lz"
	.balign 4, 0
.LField_Map105_SHAPE_GRID:
	.incbin "MAP/MAP_103/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map105_SCENE:
	.incbin "MAP/MAP_105/SCENE.parts.lz"
	.endif
