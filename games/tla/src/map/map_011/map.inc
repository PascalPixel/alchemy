	.global Field_Map011
Field_Map011:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map011, .LField_Map011_CELL_PICTURES
	map_block Field_Map011, .LField_Map011_SHAPES
	map_block Field_Map011, .LField_Map011_GRID
	map_block Field_Map011, .LField_Map011_SHAPE_GRID
	map_block Field_Map011, .LField_Map011_ANIMATION
	map_block Field_Map011, .LField_Map011_BLEND
	map_block Field_Map011, .LField_Map011_SCENE
.LField_Map011_CELL_PICTURES:
	.incbin "games/tla/src/map/map_011/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map011_SHAPES:
	.incbin "games/tla/src/map/map_011/shapes.table.lz"
	.balign 4, 0
.LField_Map011_GRID:
	.incbin "games/tla/src/map/map_011/map.grid.lz"
	.balign 4, 0
.LField_Map011_SHAPE_GRID:
	.incbin "games/tla/src/map/map_011/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map011_ANIMATION:
	.incbin "games/tla/src/map/map_011/animation.script.lz"
	.balign 4, 0
.LField_Map011_BLEND:
	.incbin "games/tla/src/map/map_011/blend.script.lz"
	.balign 4, 0
.LField_Map011_SCENE:
	.incbin "games/tla/src/map/map_011/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "games/tla/src/graphics/tileset/set_08/palette.gbapal.lz"
	.byte 0x80	@ PACKER: left in the buffer by Field_Map011+0x197
	.global Tileset_Set08TilesA
Tileset_Set08TilesA:
	.incbin "games/tla/src/graphics/tileset/set_08/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set08TilesB
Tileset_Set08TilesB:
	.incbin "games/tla/src/graphics/tileset/set_08/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x17b3
	.global Tileset_Set08TilesC
Tileset_Set08TilesC:
	.incbin "games/tla/src/graphics/tileset/set_08/tiles_c.4bpp.mtf"
	.byte 0x8e, 0x9f	@ PACKER: left in the buffer by Tileset_Set08TilesB+0xf9a
	.global Tileset_Set08TilesD
Tileset_Set08TilesD:
	.incbin "games/tla/src/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.balign 4, 0
