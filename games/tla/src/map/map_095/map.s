	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map095
Field_Map095:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_CELL_PICTURES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_SHAPE_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_SCENE
.LField_Map095_CELL_PICTURES:
	.incbin "games/tla/src/map/map_095/cell_pictures_ja.delta0.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "games/tla/src/map/map_095/shapes.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "games/tla/src/map/map_095/grid.parts.lz"
	.balign 4, 0
.LField_Map095_SHAPE_GRID:
	.incbin "games/tla/src/map/map_095/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map095_SCENE:
	.incbin "games/tla/src/map/map_095/scene_ja.parts.lz"
	.else
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_CELL_PICTURES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_SHAPE_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_SCENE
.LField_Map095_CELL_PICTURES:
	.incbin "games/tla/src/map/map_095/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "games/tla/src/map/map_095/shapes.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "games/tla/src/map/map_095/grid.parts.lz"
	.balign 4, 0
.LField_Map095_SHAPE_GRID:
	.incbin "games/tla/src/map/map_095/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map095_SCENE:
	.incbin "games/tla/src/map/map_095/scene.parts.lz"
	.endif
	.balign 4, 0
	.global Tileset_Set46Palette
Tileset_Set46Palette:
	.incbin "games/tla/src/graphics/tileset/set_46/palette.gbapal.lz"
	.byte 0x29, 0xc7	@ PACKER: left in the buffer by Field_Map095+0x132
	.global Tileset_Set46TilesA
Tileset_Set46TilesA:
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_a.4bpp.mtf"
	.byte 0x94, 0xc6	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x276a
	.global Tileset_Set46TilesB
Tileset_Set46TilesB:
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_b.4bpp.mtf"
	.byte 0xc8, 0x02, 0x58	@ PACKER: left in the buffer by Tileset_Set46TilesA+0x1dd1
	.global Tileset_Set46TilesC
Tileset_Set46TilesC:
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_c.4bpp.mtf"
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_d_ja.4bpp.mtf"
	.else
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_d.4bpp.mtf"
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set46TilesC+0x13f
	.endif
