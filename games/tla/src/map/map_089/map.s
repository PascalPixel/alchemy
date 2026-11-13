	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map089
Field_Map089:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map089, .LField_Map089_CELL_PICTURES
	map_block Field_Map089, .LField_Map089_SHAPES
	map_block Field_Map089, .LField_Map089_GRID
	map_block Field_Map089, .LField_Map089_SHAPE_GRID
	map_block Field_Map089, .LField_Map089_ANIMATION
	map_block Field_Map089, .LField_Map089_BLEND
	map_block Field_Map089, .LField_Map089_SCENE
.LField_Map089_CELL_PICTURES:
	.incbin "games/tla/src/map/map_089/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "games/tla/src/map/map_089/shapes.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_089/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/map_089/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Map089_SHAPE_GRID:
	.incbin "games/tla/src/map/map_089/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "games/tla/src/map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map089_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_089/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/map_089/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_Set42Palette
Tileset_Set42Palette:
	.incbin "games/tla/src/graphics/tileset/set_42/palette.gbapal.lz"
	.global Tileset_Set42TilesA
Tileset_Set42TilesA:
	.incbin "games/tla/src/graphics/tileset/set_42/tiles_a.4bpp.mtf"
	.byte 0x70, 0x0e	@ PACKER: left in the buffer by Field_Map089+0x15e6
