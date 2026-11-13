	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map060
Field_Map060:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map060, .LField_Map060_CELL_PICTURES
	map_block Field_Map060, .LField_Map060_SHAPES
	map_block Field_Map060, .LField_Map060_GRID
	map_block Field_Map060, .LField_Map060_SHAPE_GRID
	map_block Field_Map060, .LField_Map060_ANIMATION
	map_block Field_Map060, .LField_Map060_BLEND
	map_block Field_Map060, .LField_Map060_SCENE
.LField_Map060_CELL_PICTURES:
	.incbin "games/tla/src/map/map_060/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map060_SHAPES:
	.incbin "games/tla/src/map/map_060/shapes.table.lz"
	.balign 4, 0
.LField_Map060_GRID:
	.incbin "games/tla/src/map/map_060/grid.parts.lz"
	.balign 4, 0
.LField_Map060_SHAPE_GRID:
	.incbin "games/tla/src/map/map_060/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map060_ANIMATION:
	.incbin "games/tla/src/map/map_060/animation.script.lz"
	.balign 4, 0
.LField_Map060_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map060_SCENE:
	.incbin "games/tla/src/map/map_060/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set35Palette
Tileset_Set35Palette:
	.incbin "games/tla/src/graphics/tileset/set_35/palette.gbapal.lz"
	.byte 0x87, 0x37, 0x18	@ PACKER: left in the buffer by Field_Map060+0x12d
	.global Tileset_Set35TilesA
Tileset_Set35TilesA:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_a.4bpp.mtf"
	.byte 0xe6, 0x08	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x2ab6
	.global Tileset_Set35TilesB
Tileset_Set35TilesB:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_b.4bpp.mtf"
	.byte 0xa9, 0x03	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x1b56
	.global Tileset_Set35TilesC
Tileset_Set35TilesC:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_c.4bpp.mtf"
	.byte 0xf5, 0xc0, 0x8e	@ PACKER: left in the buffer by Tileset_Set35TilesB+0xf45
	.global Tileset_Set35TilesD
Tileset_Set35TilesD:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_d.4bpp.mtf"
	.byte 0x6f, 0xd1	@ PACKER: left in the buffer by Tileset_Set35TilesB+0x1a06
