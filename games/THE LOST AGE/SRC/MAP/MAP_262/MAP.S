@ Field_Map262: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map262
Field_Map262:
	.ifdef TLA_EDITION_JA
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map262, .LField_Map262_METATILES
	map_block Field_Map262, .LField_Map262_SHAPES
	map_block Field_Map262, .LField_Map262_GRID
	map_block Field_Map262, .LField_Map262_SHAPE_GRID
	map_block Field_Map262, .LField_Map262_ANIMATION
	map_block Field_Map262, .LField_Map262_BLEND
	map_block Field_Map262, .LField_Map262_SCENE
.LField_Map262_METATILES:
	.incbin "MAP/MAP_262/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map262_SHAPES:
	.incbin "MAP/MAP_262/SHAPES_JA.table.lz"
	.balign 4, 0
.LField_Map262_GRID:
	.incbin "MAP/MAP_262/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map262_SHAPE_GRID:
	.incbin "MAP/MAP_262/SHAPE_GRID_JA.plane.lz"
	.balign 4, 0
.LField_Map262_ANIMATION:
	.incbin "MAP/MAP_259/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map262_BLEND:
	.incbin "MAP/MAP_260/BLEND.script.lz"
	.balign 4, 0
.LField_Map262_SCENE:
	.incbin "MAP/MAP_262/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map262, .LField_Map262_METATILES
	map_block Field_Map262, .LField_Map262_SHAPES
	map_block Field_Map262, .LField_Map262_GRID
	map_block Field_Map262, .LField_Map262_SHAPE_GRID
	map_block Field_Map262, .LField_Map262_ANIMATION
	map_block Field_Map262, .LField_Map262_BLEND
	map_block Field_Map262, .LField_Map262_SCENE
.LField_Map262_METATILES:
	.incbin "MAP/MAP_262/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map262_SHAPES:
	.incbin "MAP/MAP_262/SHAPES.table.lz"
	.balign 4, 0
.LField_Map262_GRID:
	.incbin "MAP/MAP_262/GRID.parts.lz"
	.balign 4, 0
.LField_Map262_SHAPE_GRID:
	.incbin "MAP/MAP_262/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map262_ANIMATION:
	.incbin "MAP/MAP_259/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map262_BLEND:
	.incbin "MAP/MAP_260/BLEND.script.lz"
	.balign 4, 0
.LField_Map262_SCENE:
	.incbin "MAP/MAP_262/SCENE.parts.lz"
	.endif
