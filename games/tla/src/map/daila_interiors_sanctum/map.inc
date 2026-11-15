	.global Field_DailaInteriorsSanctum
Field_DailaInteriorsSanctum:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DailaInteriorsSanctum, .LField_DailaInteriorsSanctum_CELL_PICTURES
	map_block Field_DailaInteriorsSanctum, .LField_DailaInteriorsSanctum_SHAPES
	map_block Field_DailaInteriorsSanctum, .LField_DailaInteriorsSanctum_GRID
	map_block Field_DailaInteriorsSanctum, .LField_DailaInteriorsSanctum_SHAPE_GRID
	map_block Field_DailaInteriorsSanctum, .LField_DailaInteriorsSanctum_ANIMATION
	map_block Field_DailaInteriorsSanctum, .LField_DailaInteriorsSanctum_BLEND
	map_block Field_DailaInteriorsSanctum, .LField_DailaInteriorsSanctum_SCENE
.LField_DailaInteriorsSanctum_CELL_PICTURES:
	.incbin "games/tla/src/map/daila_interiors_sanctum/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_DailaInteriorsSanctum_SHAPES:
	.incbin "games/tla/src/map/daila_interiors_sanctum/shapes.table.lz"
	.balign 4, 0
.LField_DailaInteriorsSanctum_GRID:
	.incbin "games/tla/src/map/daila_interiors_sanctum/map.grid.lz"
	.balign 4, 0
.LField_DailaInteriorsSanctum_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_interiors_sanctum/shape_grid.plane.lz"
	.balign 4, 0
.LField_DailaInteriorsSanctum_ANIMATION:
	.incbin "games/tla/src/map/daila_interiors_sanctum/animation.script.lz"
	.balign 4, 0
.LField_DailaInteriorsSanctum_BLEND:
	.incbin "games/tla/src/map/daila_interiors_sanctum/blend.script.lz"
	.balign 4, 0
.LField_DailaInteriorsSanctum_SCENE:
	.incbin "games/tla/src/map/daila_interiors_sanctum/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DailaInteriorsSanctumPalette
Tileset_DailaInteriorsSanctumPalette:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_sanctum/palette.gbapal.lz"
	.byte 0x80	@ PACKER: left in the buffer by Field_DailaInteriors2+0x197
	.global Tileset_DailaInteriorsSanctumTilesA
Tileset_DailaInteriorsSanctumTilesA:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_sanctum/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_DailaInteriorsSanctumTilesB
Tileset_DailaInteriorsSanctumTilesB:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_sanctum/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_DailaInteriorsSanctumTilesA+0x17b3
	.global Tileset_DailaInteriorsSanctumTilesC
Tileset_DailaInteriorsSanctumTilesC:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_sanctum/tiles_c.4bpp.mtf"
	.byte 0x8e, 0x9f	@ PACKER: left in the buffer by Tileset_DailaInteriorsSanctumTilesB+0xf9a
	.global Tileset_DailaInteriorsSanctumTilesD
Tileset_DailaInteriorsSanctumTilesD:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_sanctum/tiles_d.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.balign 4, 0
