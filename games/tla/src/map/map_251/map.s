	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map251
Field_Map251:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map251, .LField_Map251_CELL_PICTURES
	map_block Field_Map251, .LField_Map251_SHAPES
	map_block Field_Map251, .LField_Map251_GRID
	map_block Field_Map251, .LField_Map251_SHAPE_GRID
	map_block Field_Map251, .LField_Map251_ANIMATION
	map_block Field_Map251, .LField_Map251_BLEND
	map_block Field_Map251, .LField_Map251_SCENE
.LField_Map251_CELL_PICTURES:
	.incbin "games/tla/src/map/map_251/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map251_SHAPES:
	.incbin "games/tla/src/map/map_251/shapes.table.lz"
	.balign 4, 0
.LField_Map251_GRID:
	.incbin "games/tla/src/map/map_251/map.grid.lz"
	.balign 4, 0
.LField_Map251_SHAPE_GRID:
	.incbin "games/tla/src/map/map_251/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map251_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map251_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map251_SCENE:
	.incbin "games/tla/src/map/map_251/scene.parts.lz"
	.global Tileset_Set105Palette
Tileset_Set105Palette:
	.incbin "games/tla/src/graphics/tileset/set_105/palette.gbapal.lz"
	.byte 0xa3	@ PACKER: left in the buffer by Field_Map251+0x11f
	.global Tileset_Set105TilesA
Tileset_Set105TilesA:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_a.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Set103TilesB+0x25cf
	.global Tileset_Set105TilesB
Tileset_Set105TilesB:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_b.4bpp.mtf"
	.byte 0xc4, 0x0d, 0xef	@ PACKER: left in the buffer by Tileset_Set105TilesA+0x2311
	.global Tileset_Set105TilesC
Tileset_Set105TilesC:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_c.4bpp.mtf"
	.byte 0xf2, 0x00, 0x7f	@ PACKER: left in the buffer by Tileset_Set105TilesB+0x871
	.global Tileset_Set105TilesD
Tileset_Set105TilesD:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_d.4bpp.mtf"
	.byte 0xf5	@ PACKER: left in the buffer by Tileset_Set105TilesC+0x13f
