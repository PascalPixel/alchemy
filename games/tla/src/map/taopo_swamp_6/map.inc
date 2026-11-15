	.global Field_TaopoSwamp6
Field_TaopoSwamp6:
	map_header 0, 0, 90, 100, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwamp6, .LField_TaopoSwamp6_CELL_PICTURES
	map_block Field_TaopoSwamp6, .LField_TaopoSwamp6_SHAPES
	map_block Field_TaopoSwamp6, .LField_TaopoSwamp6_GRID
	map_block Field_TaopoSwamp6, .LField_TaopoSwamp6_SHAPE_GRID
	map_block Field_TaopoSwamp6, .LField_TaopoSwamp6_ANIMATION
	map_block Field_TaopoSwamp6, .LField_TaopoSwamp6_BLEND
	map_block Field_TaopoSwamp6, .LField_TaopoSwamp6_SCENE
.LField_TaopoSwamp6_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TaopoSwamp6_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_6/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp6_GRID:
	.incbin "games/tla/src/map/taopo_swamp_6/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwamp6_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwamp6_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_6/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp6_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp6_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_6/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TaopoSwamp6Palette
Tileset_TaopoSwamp6Palette:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_6/palette.gbapal.lz"
	.byte 0xe1, 0x40, 0x88	@ PACKER: left in the buffer by Field_TaopoSwamp6+0x16d
	.global Tileset_TaopoSwamp6TilesA
Tileset_TaopoSwamp6TilesA:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_6/tiles_a.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global Tileset_TaopoSwamp6TilesB
Tileset_TaopoSwamp6TilesB:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_6/tiles_b.4bpp.mtf"
	.global Tileset_TaopoSwamp6TilesC
Tileset_TaopoSwamp6TilesC:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_6/tiles_c.4bpp.mtf"
	.byte 0x50, 0x22	@ PACKER: left in the buffer by Tileset_TaopoSwamp6TilesA+0x10d6
	.global Tileset_TaopoSwamp6TilesD
Tileset_TaopoSwamp6TilesD:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_6/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x26, 0x82	@ PACKER: left in the buffer by Tileset_TaopoSwamp6TilesA+0x1969
