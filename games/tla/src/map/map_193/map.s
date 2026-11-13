	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map193
Field_Map193:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map193, .LField_Map193_CELL_PICTURES
	map_block Field_Map193, .LField_Map193_SHAPES
	map_block Field_Map193, .LField_Map193_GRID
	map_block Field_Map193, .LField_Map193_SHAPE_GRID
	map_block Field_Map193, .LField_Map193_ANIMATION
	map_block Field_Map193, .LField_Map193_BLEND
	map_block Field_Map193, .LField_Map193_SCENE
.LField_Map193_CELL_PICTURES:
	.incbin "games/tla/src/map/map_193/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map193_SHAPES:
	.incbin "games/tla/src/map/map_193/shapes.table.lz"
	.balign 4, 0
.LField_Map193_GRID:
	.incbin "games/tla/src/map/map_193/map.grid.lz"
	.balign 4, 0
.LField_Map193_SHAPE_GRID:
	.incbin "games/tla/src/map/map_193/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map193_ANIMATION:
	.incbin "games/tla/src/map/map_193/animation.script.lz"
	.balign 4, 0
.LField_Map193_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map193_SCENE:
	.incbin "games/tla/src/map/map_193/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set85Palette
Tileset_Set85Palette:
	.incbin "games/tla/src/graphics/tileset/set_85/palette.gbapal.lz"
	.byte 0x18, 0x61, 0xb5	@ PACKER: left in the buffer by Field_Map193+0x1a9
	.global Tileset_Set85TilesA
Tileset_Set85TilesA:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_a.4bpp.mtf"
	.byte 0xd5, 0x83	@ PACKER: left in the buffer by Tileset_Set84TilesA+0x1b0a
	.global Tileset_Set85TilesB
Tileset_Set85TilesB:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_b.4bpp.mtf"
	.global Tileset_Set85TilesC
Tileset_Set85TilesC:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_c.4bpp.mtf"
	.byte 0x29	@ PACKER: left in the buffer by Tileset_Set85TilesA+0xe77
	.global Tileset_Set85TilesD
Tileset_Set85TilesD:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_d.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
