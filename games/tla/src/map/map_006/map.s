	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map006
Field_Map006:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map006, .LField_Map006_CELL_PICTURES
	map_block Field_Map006, .LField_Map006_SHAPES
	map_block Field_Map006, .LField_Map006_GRID
	map_block Field_Map006, .LField_Map006_SHAPE_GRID
	map_block Field_Map006, .LField_Map006_ANIMATION
	map_block Field_Map006, .LField_Map006_BLEND
	map_block Field_Map006, .LField_Map006_SCENE
.LField_Map006_CELL_PICTURES:
	.incbin "games/tla/src/map/map_006/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map006_SHAPES:
	.incbin "games/tla/src/map/map_006/shapes.table.lz"
	.balign 4, 0
.LField_Map006_GRID:
	.incbin "games/tla/src/map/map_006/grid.parts.lz"
	.balign 4, 0
.LField_Map006_SHAPE_GRID:
	.incbin "games/tla/src/map/map_006/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map006_ANIMATION:
	.incbin "games/tla/src/map/map_006/animation.script.lz"
	.balign 4, 0
.LField_Map006_BLEND:
	.incbin "games/tla/src/map/map_006/blend.script.lz"
	.balign 4, 0
.LField_Map006_SCENE:
	.incbin "games/tla/src/map/map_006/scene.parts.lz"
	.global Tileset_Set04Palette
Tileset_Set04Palette:
	.incbin "games/tla/src/graphics/tileset/set_04/pal_42.gbapal.lz"
	.byte 0x1f, 0x44	@ PACKER: left in the buffer by Field_Map006+0x1b6
	.global Tileset_Set04TilesA
Tileset_Set04TilesA:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_a.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x2afb
	.global Tileset_Set04TilesB
Tileset_Set04TilesB:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_b.4bpp.mtf"
	.byte 0x00, 0xa0, 0xbb	@ PACKER: left in the buffer by Tileset_Set04TilesA+0x20dd
	.global Tileset_Set04TilesC
Tileset_Set04TilesC:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_c.4bpp.mtf"
	.byte 0x27, 0xe9, 0x0b	@ PACKER: left in the buffer by Tileset_Set04TilesA+0x2249
	.global Tileset_Set04TilesD
Tileset_Set04TilesD:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_d.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x13f
