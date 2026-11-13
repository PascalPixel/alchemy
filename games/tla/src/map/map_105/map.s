	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map105
Field_Map105:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_CELL_PICTURES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_SHAPE_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_SCENE
.LField_Map105_CELL_PICTURES:
	.incbin "games/tla/src/map/map_105/cell_pictures_ja.delta1.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "games/tla/src/map/map_103/shapes.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "games/tla/src/map/map_105/map_ja.grid.lz"
	.balign 4, 0
.LField_Map105_SHAPE_GRID:
	.incbin "games/tla/src/map/map_103/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map105_SCENE:
	.incbin "games/tla/src/map/map_105/scene_ja.parts.lz"
	.else
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_CELL_PICTURES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_SHAPE_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_SCENE
.LField_Map105_CELL_PICTURES:
	.incbin "games/tla/src/map/map_105/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "games/tla/src/map/map_103/shapes.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "games/tla/src/map/map_105/map.grid.lz"
	.balign 4, 0
.LField_Map105_SHAPE_GRID:
	.incbin "games/tla/src/map/map_103/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map105_SCENE:
	.incbin "games/tla/src/map/map_105/scene.parts.lz"
	.endif
	.balign 4, 0
	.global Tileset_Set54Palette
Tileset_Set54Palette:
	.incbin "games/tla/src/graphics/tileset/set_54/palette.gbapal.lz"
	.byte 0x3c, 0x78, 0xf0	@ PACKER: left in the buffer by Field_Map105+0x165
	.global Tileset_Set54TilesA
Tileset_Set54TilesA:
	.incbin "games/tla/src/graphics/tileset/set_54/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map104+0x27f7
	.global Tileset_Set54TilesB
Tileset_Set54TilesB:
	.incbin "games/tla/src/graphics/tileset/set_54/tiles_b.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x2033
	.global Tileset_Set54TilesC
Tileset_Set54TilesC:
	.incbin "games/tla/src/graphics/tileset/set_54/tiles_c.4bpp.mtf"
	.global Tileset_Set54TilesD
Tileset_Set54TilesD:
	.incbin "games/tla/src/graphics/tileset/set_54/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x7c	@ PACKER: left in the buffer by Tileset_Set54TilesC+0xbae
