	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map093
Field_Map093:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map093, .LField_Map093_CELL_PICTURES
	map_block Field_Map093, .LField_Map093_SHAPES
	map_block Field_Map093, .LField_Map093_GRID
	map_block Field_Map093, .LField_Map093_SHAPE_GRID
	map_block Field_Map093, .LField_Map093_ANIMATION
	map_block Field_Map093, .LField_Map093_BLEND
	map_block Field_Map093, .LField_Map093_SCENE
.LField_Map093_CELL_PICTURES:
	.incbin "games/tla/src/map/map_093/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map093_SHAPES:
	.incbin "games/tla/src/map/map_093/shapes.table.lz"
	.balign 4, 0
.LField_Map093_GRID:
	.incbin "games/tla/src/map/map_093/grid.parts.lz"
	.balign 4, 0
.LField_Map093_SHAPE_GRID:
	.incbin "games/tla/src/map/map_093/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map093_ANIMATION:
	.incbin "games/tla/src/map/map_093/animation.script.lz"
	.balign 4, 0
.LField_Map093_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map093_SCENE:
	.incbin "games/tla/src/map/map_093/scene.parts.lz"
	.global Tileset_Set45Palette
Tileset_Set45Palette:
	.incbin "games/tla/src/graphics/tileset/set_45/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Map093+0x19b
	.global Tileset_Set45TilesA
Tileset_Set45TilesA:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_a.4bpp.mtf"
	.byte 0xdd, 0x56	@ PACKER: left in the buffer by Field_Map075+0x2c8a
	.global Tileset_Set45TilesB
Tileset_Set45TilesB:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_b.4bpp.mtf"
	.byte 0x8b, 0x70	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x104a
	.global Tileset_Set45TilesC
Tileset_Set45TilesC:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_c.4bpp.mtf"
	.byte 0x4a	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x10fb
	.global Tileset_Set45TilesD
Tileset_Set45TilesD:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_d.4bpp.mtf"
	.byte 0x87	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x17d3
