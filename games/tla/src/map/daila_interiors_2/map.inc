	.global Field_DailaInteriors2
Field_DailaInteriors2:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DailaInteriors2, .LField_DailaInteriors2_CELL_PICTURES
	map_block Field_DailaInteriors2, .LField_DailaInteriors2_SHAPES
	map_block Field_DailaInteriors2, .LField_DailaInteriors2_GRID
	map_block Field_DailaInteriors2, .LField_DailaInteriors2_SHAPE_GRID
	map_block Field_DailaInteriors2, .LField_DailaInteriors2_ANIMATION
	map_block Field_DailaInteriors2, .LField_DailaInteriors2_BLEND
	map_block Field_DailaInteriors2, .LField_DailaInteriors2_SCENE
.LField_DailaInteriors2_CELL_PICTURES:
	.incbin "games/tla/src/map/daila_interiors_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_DailaInteriors2_SHAPES:
	.incbin "games/tla/src/map/daila_interiors_2/shapes.table.lz"
	.balign 4, 0
.LField_DailaInteriors2_GRID:
	.incbin "games/tla/src/map/daila_interiors_2/map.grid.lz"
	.balign 4, 0
.LField_DailaInteriors2_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_interiors_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_DailaInteriors2_ANIMATION:
	.incbin "games/tla/src/map/daila_interiors_2/animation.script.lz"
	.balign 4, 0
.LField_DailaInteriors2_BLEND:
	.incbin "games/tla/src/map/daila_interiors_2/blend.script.lz"
	.balign 4, 0
.LField_DailaInteriors2_SCENE:
	.incbin "games/tla/src/map/daila_interiors_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DailaInteriors2Palette
Tileset_DailaInteriors2Palette:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_2/palette.gbapal.lz"
	.byte 0x80	@ PACKER: left in the buffer by Field_DailaInteriors2+0x197
	.global Tileset_DailaInteriors2TilesA
Tileset_DailaInteriors2TilesA:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_2/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_DailaInteriors2TilesB
Tileset_DailaInteriors2TilesB:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_2/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_DailaInteriors2TilesA+0x17b3
	.global Tileset_DailaInteriors2TilesC
Tileset_DailaInteriors2TilesC:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_2/tiles_c.4bpp.mtf"
	.byte 0x8e, 0x9f	@ PACKER: left in the buffer by Tileset_DailaInteriors2TilesB+0xf9a
	.global Tileset_DailaInteriors2TilesD
Tileset_DailaInteriors2TilesD:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_2/tiles_d.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.balign 4, 0
