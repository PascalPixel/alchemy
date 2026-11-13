	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map103
Field_Map103:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map103, .LField_Map103_CELL_PICTURES
	map_block Field_Map103, .LField_Map103_SHAPES
	map_block Field_Map103, .LField_Map103_GRID
	map_block Field_Map103, .LField_Map103_SHAPE_GRID
	map_block Field_Map103, .LField_Map103_ANIMATION
	map_block Field_Map103, .LField_Map103_BLEND
	map_block Field_Map103, .LField_Map103_SCENE
.LField_Map103_CELL_PICTURES:
	.incbin "games/tla/src/map/map_103/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map103_SHAPES:
	.incbin "games/tla/src/map/map_103/shapes.table.lz"
	.balign 4, 0
.LField_Map103_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_103/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/map_103/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Map103_SHAPE_GRID:
	.incbin "games/tla/src/map/map_103/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map103_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map103_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map103_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_103/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/map_103/scene.scene.lz"
	.endif
	.global Tileset_Set52Palette
Tileset_Set52Palette:
	.incbin "games/tla/src/graphics/tileset/set_52/palette.gbapal.lz"
	.byte 0x3c, 0x88, 0x03	@ PACKER: left in the buffer by Field_Map103+0x11d
	.global Tileset_Set52TilesA
Tileset_Set52TilesA:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_a.4bpp.mtf"
	.byte 0x5b	@ PACKER: left in the buffer by Tileset_Set51TilesA+0x28ab
	.global Tileset_Set52TilesB
Tileset_Set52TilesB:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_b.4bpp.mtf"
	.byte 0xc8, 0xf7	@ PACKER: left in the buffer by Tileset_Set52TilesA+0x1f92
	.global Tileset_Set52TilesC
Tileset_Set52TilesC:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_c.4bpp.mtf"
	.byte 0xad	@ PACKER: left in the buffer by Tileset_Set52TilesB+0x101f
	.global Tileset_Set52TilesD
Tileset_Set52TilesD:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Set52TilesC+0x13f
