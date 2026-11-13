	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map256
Field_Map256:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 90, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 66, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map256, .LField_Map256_CELL_PICTURES
	map_block Field_Map256, .LField_Map256_SHAPES
	map_block Field_Map256, .LField_Map256_GRID
	map_block Field_Map256, .LField_Map256_SHAPE_GRID
	map_block Field_Map256, .LField_Map256_ANIMATION
	map_block Field_Map256, .LField_Map256_BLEND
	map_block Field_Map256, .LField_Map256_SCENE
.LField_Map256_CELL_PICTURES:
	.incbin "games/tla/src/map/map_256/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map256_SHAPES:
	.incbin "games/tla/src/map/map_256/shapes.table.lz"
	.balign 4, 0
.LField_Map256_GRID:
	.incbin "games/tla/src/map/map_256/map_ja.grid.lz"
	.balign 4, 0
.LField_Map256_SHAPE_GRID:
	.incbin "games/tla/src/map/map_256/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map256_ANIMATION:
	.incbin "games/tla/src/map/map_256/animation.script.lz"
	.balign 4, 0
.LField_Map256_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map256_SCENE:
	.incbin "games/tla/src/map/map_256/scene_ja.parts.lz"
	.else
	map_header 0, 0, 90, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 66, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map256, .LField_Map256_CELL_PICTURES
	map_block Field_Map256, .LField_Map256_SHAPES
	map_block Field_Map256, .LField_Map256_GRID
	map_block Field_Map256, .LField_Map256_SHAPE_GRID
	map_block Field_Map256, .LField_Map256_ANIMATION
	map_block Field_Map256, .LField_Map256_BLEND
	map_block Field_Map256, .LField_Map256_SCENE
.LField_Map256_CELL_PICTURES:
	.incbin "games/tla/src/map/map_256/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map256_SHAPES:
	.incbin "games/tla/src/map/map_256/shapes.table.lz"
	.balign 4, 0
.LField_Map256_GRID:
	.incbin "games/tla/src/map/map_256/map.grid.lz"
	.balign 4, 0
.LField_Map256_SHAPE_GRID:
	.incbin "games/tla/src/map/map_256/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map256_ANIMATION:
	.incbin "games/tla/src/map/map_256/animation.script.lz"
	.balign 4, 0
.LField_Map256_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map256_SCENE:
	.incbin "games/tla/src/map/map_256/scene.parts.lz"
	.endif
	.balign 4, 0
	.global Tileset_Palette59
Tileset_Palette59:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_59.gbapal.lz"
	.byte 0x98, 0x2d	@ PACKER: left in the buffer by Field_Map256+0x162
