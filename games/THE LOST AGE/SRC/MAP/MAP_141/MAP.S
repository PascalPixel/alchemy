	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Field_Map141: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map141
Field_Map141:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map141, .LField_Map141_METATILES
	map_block Field_Map141, .LField_Map141_SHAPES
	map_block Field_Map141, .LField_Map141_GRID
	map_block Field_Map141, .LField_Map141_SHAPE_GRID
	map_block Field_Map141, .LField_Map141_ANIMATION
	map_block Field_Map141, .LField_Map141_BLEND
	map_block Field_Map141, .LField_Map141_SCENE
.LField_Map141_METATILES:
	.incbin "MAP/MAP_141/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map141_SHAPES:
	.incbin "MAP/MAP_141/SHAPES.table.lz"
	.balign 4, 0
.LField_Map141_GRID:
	.incbin "MAP/MAP_141/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map141_SHAPE_GRID:
	.incbin "MAP/MAP_141/SHAPE_GRID_JA.plane.lz"
	.balign 4, 0
.LField_Map141_ANIMATION:
	.incbin "MAP/MAP_141/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map141_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map141_SCENE:
	.incbin "MAP/MAP_141/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map141, .LField_Map141_METATILES
	map_block Field_Map141, .LField_Map141_SHAPES
	map_block Field_Map141, .LField_Map141_GRID
	map_block Field_Map141, .LField_Map141_SHAPE_GRID
	map_block Field_Map141, .LField_Map141_ANIMATION
	map_block Field_Map141, .LField_Map141_BLEND
	map_block Field_Map141, .LField_Map141_SCENE
.LField_Map141_METATILES:
	.incbin "MAP/MAP_141/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map141_SHAPES:
	.incbin "MAP/MAP_141/SHAPES.table.lz"
	.balign 4, 0
.LField_Map141_GRID:
	.incbin "MAP/MAP_141/GRID.parts.lz"
	.balign 4, 0
.LField_Map141_SHAPE_GRID:
	.incbin "MAP/MAP_141/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map141_ANIMATION:
	.incbin "MAP/MAP_141/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map141_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map141_SCENE:
	.incbin "MAP/MAP_141/SCENE.parts.lz"
	.endif
