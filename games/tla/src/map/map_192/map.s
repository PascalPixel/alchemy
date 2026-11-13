	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map192
Field_Map192:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map192, .LField_Map192_CELL_PICTURES
	map_block Field_Map192, .LField_Map192_SHAPES
	map_block Field_Map192, .LField_Map192_GRID
	map_block Field_Map192, .LField_Map192_SHAPE_GRID
	map_block Field_Map192, .LField_Map192_ANIMATION
	map_block Field_Map192
	map_block Field_Map192, .LField_Map192_SCENE
.LField_Map192_CELL_PICTURES:
	.incbin "games/tla/src/map/map_192/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map192_SHAPES:
	.incbin "games/tla/src/map/map_192/shapes.table.lz"
	.balign 4, 0
.LField_Map192_GRID:
	.incbin "games/tla/src/map/map_192/map.grid.lz"
	.balign 4, 0
.LField_Map192_SHAPE_GRID:
	.incbin "games/tla/src/map/map_192/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map192_ANIMATION:
	.incbin "games/tla/src/map/map_192/animation.script.lz"
	.balign 4, 0
.LField_Map192_SCENE:
	.incbin "games/tla/src/map/map_192/scene_ja.scene.lz"
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map192, .LField_Map192_CELL_PICTURES
	map_block Field_Map192, .LField_Map192_SHAPES
	map_block Field_Map192, .LField_Map192_GRID
	map_block Field_Map192, .LField_Map192_SHAPE_GRID
	map_block Field_Map192, .LField_Map192_ANIMATION
	map_block Field_Map192
	map_block Field_Map192, .LField_Map192_SCENE
.LField_Map192_CELL_PICTURES:
	.incbin "games/tla/src/map/map_192/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map192_SHAPES:
	.incbin "games/tla/src/map/map_192/shapes.table.lz"
	.balign 4, 0
.LField_Map192_GRID:
	.incbin "games/tla/src/map/map_192/map.grid.lz"
	.balign 4, 0
.LField_Map192_SHAPE_GRID:
	.incbin "games/tla/src/map/map_192/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map192_ANIMATION:
	.incbin "games/tla/src/map/map_192/animation.script.lz"
	.balign 4, 0
.LField_Map192_SCENE:
	.incbin "games/tla/src/map/map_192/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_Set84Palette
Tileset_Set84Palette:
	.incbin "games/tla/src/graphics/tileset/set_84/palette.gbapal.lz"
	.byte 0x9d, 0xf8	@ PACKER: left in the buffer by Field_Map192+0x1d2
	.global Tileset_Set84TilesA
Tileset_Set84TilesA:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_a.4bpp.mtf"
	.byte 0x0a, 0x16	@ PACKER: left in the buffer by Field_Map192+0x203e
	.global Tileset_Set84TilesB
Tileset_Set84TilesB:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_b.4bpp.mtf"
	.byte 0x40, 0x60, 0xb9	@ PACKER: left in the buffer by Tileset_Set84TilesA+0x13f5
	.global Tileset_Set84TilesC
Tileset_Set84TilesC:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_c.4bpp.mtf"
	.byte 0x44, 0xd3	@ PACKER: left in the buffer by Tileset_Set84TilesB+0xb3a
	.global Tileset_Set84TilesD
Tileset_Set84TilesD:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_d.4bpp.mtf"
	.byte 0xbf, 0x45, 0xca	@ PACKER: left in the buffer by Tileset_Set84TilesB+0x1169
