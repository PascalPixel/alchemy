	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map128
Field_Map128:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map128, .LField_Map128_CELL_PICTURES
	map_block Field_Map128, .LField_Map128_SHAPES
	map_block Field_Map128, .LField_Map128_GRID
	map_block Field_Map128, .LField_Map128_SHAPE_GRID
	map_block Field_Map128, .LField_Map128_ANIMATION
	map_block Field_Map128
	map_block Field_Map128, .LField_Map128_SCENE
.LField_Map128_CELL_PICTURES:
	.incbin "games/tla/src/map/map_128/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map128_SHAPES:
	.incbin "games/tla/src/map/map_128/shapes.table.lz"
	.balign 4, 0
.LField_Map128_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_128/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/map_128/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Map128_SHAPE_GRID:
	.incbin "games/tla/src/map/map_128/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map128_ANIMATION:
	.incbin "games/tla/src/map/map_008/animation.script.lz"
	.balign 4, 0
.LField_Map128_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_128/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/map_128/scene.scene.lz"
	.endif
	.global Tileset_Palette23
Tileset_Palette23:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_23.gbapal.lz"
	.byte 0xe0, 0xe4	@ PACKER: left in the buffer by Field_Map128+0x18e
