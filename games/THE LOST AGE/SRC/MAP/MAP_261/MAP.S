	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Field_Map261: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map261
Field_Map261:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map261, .LField_Map261_METATILES
	map_block Field_Map261, .LField_Map261_SHAPES
	map_block Field_Map261, .LField_Map261_GRID
	map_block Field_Map261, .LField_Map261_SHAPE_GRID
	map_block Field_Map261, .LField_Map261_ANIMATION
	map_block Field_Map261, .LField_Map261_BLEND
	map_block Field_Map261, .LField_Map261_SCENE
.LField_Map261_METATILES:
	.incbin "MAP/MAP_261/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map261_SHAPES:
	.incbin "MAP/MAP_261/SHAPES.table.lz"
	.balign 4, 0
.LField_Map261_GRID:
	.incbin "MAP/MAP_261/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map261_SHAPE_GRID:
	.incbin "MAP/MAP_261/SHAPE_GRID_JA.plane.lz"
	.balign 4, 0
.LField_Map261_ANIMATION:
	.incbin "MAP/MAP_259/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map261_BLEND:
	.incbin "MAP/MAP_260/BLEND.script.lz"
	.balign 4, 0
.LField_Map261_SCENE:
	.incbin "MAP/MAP_261/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map261, .LField_Map261_METATILES
	map_block Field_Map261, .LField_Map261_SHAPES
	map_block Field_Map261, .LField_Map261_GRID
	map_block Field_Map261, .LField_Map261_SHAPE_GRID
	map_block Field_Map261, .LField_Map261_ANIMATION
	map_block Field_Map261, .LField_Map261_BLEND
	map_block Field_Map261, .LField_Map261_SCENE
.LField_Map261_METATILES:
	.incbin "MAP/MAP_261/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map261_SHAPES:
	.incbin "MAP/MAP_261/SHAPES.table.lz"
	.balign 4, 0
.LField_Map261_GRID:
	.incbin "MAP/MAP_261/GRID.parts.lz"
	.balign 4, 0
.LField_Map261_SHAPE_GRID:
	.incbin "MAP/MAP_261/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map261_ANIMATION:
	.incbin "MAP/MAP_259/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map261_BLEND:
	.incbin "MAP/MAP_260/BLEND.script.lz"
	.balign 4, 0
.LField_Map261_SCENE:
	.incbin "MAP/MAP_261/SCENE.parts.lz"
	.endif
