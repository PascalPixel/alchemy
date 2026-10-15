	.include "games/common/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map175
Field_Map175:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map175, .LField_Map175_METATILES
	map_block Field_Map175, .LField_Map175_SHAPES
	map_block Field_Map175, .LField_Map175_GRID
	map_block Field_Map175, .LField_Map175_SHAPE_GRID
	map_block Field_Map175, .LField_Map175_ANIMATION
	map_block Field_Map175, .LField_Map175_BLEND
	map_block Field_Map175, .LField_Map175_SCENE
.LField_Map175_METATILES:
	.incbin "games/tla/src/map/map_175/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map175_SHAPES:
	.incbin "games/tla/src/map/map_175/shapes.table.lz"
	.balign 4, 0
.LField_Map175_GRID:
	.incbin "games/tla/src/map/map_175/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map175_SHAPE_GRID:
	.incbin "games/tla/src/map/map_175/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map175_ANIMATION:
	.incbin "games/common/src/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map175_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map175_SCENE:
	.incbin "games/tla/src/map/map_175/scene_ja.parts.lz"
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map175, .LField_Map175_METATILES
	map_block Field_Map175, .LField_Map175_SHAPES
	map_block Field_Map175, .LField_Map175_GRID
	map_block Field_Map175, .LField_Map175_SHAPE_GRID
	map_block Field_Map175, .LField_Map175_ANIMATION
	map_block Field_Map175, .LField_Map175_BLEND
	map_block Field_Map175, .LField_Map175_SCENE
.LField_Map175_METATILES:
	.incbin "games/tla/src/map/map_175/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map175_SHAPES:
	.incbin "games/tla/src/map/map_175/shapes.table.lz"
	.balign 4, 0
.LField_Map175_GRID:
	.incbin "games/tla/src/map/map_175/grid.parts.lz"
	.balign 4, 0
.LField_Map175_SHAPE_GRID:
	.incbin "games/tla/src/map/map_175/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map175_ANIMATION:
	.incbin "games/common/src/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map175_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map175_SCENE:
	.incbin "games/tla/src/map/map_175/scene.parts.lz"
	.endif
	.balign 4, 0
