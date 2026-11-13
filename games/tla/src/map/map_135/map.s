	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map135
Field_Map135:
	map_header 0, 0, 90, 100, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map135, .LField_Map135_CELL_PICTURES
	map_block Field_Map135, .LField_Map135_SHAPES
	map_block Field_Map135, .LField_Map135_GRID
	map_block Field_Map135, .LField_Map135_SHAPE_GRID
	map_block Field_Map135, .LField_Map135_ANIMATION
	map_block Field_Map135, .LField_Map135_BLEND
	map_block Field_Map135, .LField_Map135_SCENE
.LField_Map135_CELL_PICTURES:
	.incbin "games/tla/src/map/map_135/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "games/tla/src/map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "games/tla/src/map/map_135/grid.parts.lz"
	.balign 4, 0
.LField_Map135_SHAPE_GRID:
	.incbin "games/tla/src/map/map_135/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "games/tla/src/map/map_135/animation.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map135_SCENE:
	.incbin "games/tla/src/map/map_135/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set66Palette
Tileset_Set66Palette:
	.incbin "games/tla/src/graphics/tileset/set_66/palette.gbapal.lz"
	.byte 0xe1, 0x40, 0x88	@ PACKER: left in the buffer by Field_Map135+0x16d
	.global Tileset_Set66TilesA
Tileset_Set66TilesA:
	.incbin "games/tla/src/graphics/tileset/set_66/tiles_a.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global Tileset_Set66TilesB
Tileset_Set66TilesB:
	.incbin "games/tla/src/graphics/tileset/set_66/tiles_b.4bpp.mtf"
	.global Tileset_Set66TilesC
Tileset_Set66TilesC:
	.incbin "games/tla/src/graphics/tileset/set_66/tiles_c.4bpp.mtf"
	.byte 0x50, 0x22	@ PACKER: left in the buffer by Tileset_Set66TilesA+0x10d6
	.global Tileset_Set66TilesD
Tileset_Set66TilesD:
	.incbin "games/tla/src/graphics/tileset/set_66/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x26, 0x82	@ PACKER: left in the buffer by Tileset_Set66TilesA+0x1969
