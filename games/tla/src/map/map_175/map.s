	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map175
Field_Map175:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map175, .LField_Map175_CELL_PICTURES
	map_block Field_Map175, .LField_Map175_SHAPES
	map_block Field_Map175, .LField_Map175_GRID
	map_block Field_Map175, .LField_Map175_SHAPE_GRID
	map_block Field_Map175, .LField_Map175_ANIMATION
	map_block Field_Map175, .LField_Map175_BLEND
	map_block Field_Map175, .LField_Map175_SCENE
.LField_Map175_CELL_PICTURES:
	.incbin "games/tla/src/map/map_175/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map175_SHAPES:
	.incbin "games/tla/src/map/map_175/shapes.table.lz"
	.balign 4, 0
.LField_Map175_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_175/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/map_175/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Map175_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_175/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/map_175/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_Map175_ANIMATION:
	.incbin "games/tla/src/map/map_175/animation.script.lz"
	.balign 4, 0
.LField_Map175_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map175_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_175/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/map_175/scene.scene.lz"
	.endif
	.balign 4, 0
