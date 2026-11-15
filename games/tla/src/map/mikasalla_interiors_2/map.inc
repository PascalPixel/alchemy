	.global Field_MikasallaInteriors2
Field_MikasallaInteriors2:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MikasallaInteriors2, .LField_MikasallaInteriors2_CELL_PICTURES
	map_block Field_MikasallaInteriors2, .LField_MikasallaInteriors2_SHAPES
	map_block Field_MikasallaInteriors2, .LField_MikasallaInteriors2_GRID
	map_block Field_MikasallaInteriors2, .LField_MikasallaInteriors2_SHAPE_GRID
	map_block Field_MikasallaInteriors2, .LField_MikasallaInteriors2_ANIMATION
	map_block Field_MikasallaInteriors2, .LField_MikasallaInteriors2_BLEND
	map_block Field_MikasallaInteriors2, .LField_MikasallaInteriors2_SCENE
.LField_MikasallaInteriors2_CELL_PICTURES:
	.incbin "games/tla/src/map/mikasalla_interiors_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MikasallaInteriors2_SHAPES:
	.incbin "games/tla/src/map/mikasalla_interiors_2/shapes.table.lz"
	.balign 4, 0
.LField_MikasallaInteriors2_GRID:
	.incbin "games/tla/src/map/mikasalla_interiors_2/map.grid.lz"
	.balign 4, 0
.LField_MikasallaInteriors2_SHAPE_GRID:
	.incbin "games/tla/src/map/mikasalla_interiors_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_MikasallaInteriors2_ANIMATION:
	.incbin "games/tla/src/map/madra_interiors_2/animation.script.lz"
	.balign 4, 0
.LField_MikasallaInteriors2_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_MikasallaInteriors2_SCENE:
	.incbin "games/tla/src/map/mikasalla_interiors_2/scene.scene.lz"
	.global Tileset_MikasallaInteriors2Palette
Tileset_MikasallaInteriors2Palette:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_2/palette.gbapal.lz"
	.byte 0xe7, 0x0b	@ PACKER: left in the buffer by Field_MikasallaInteriors2+0x186
	.global Tileset_MikasallaInteriors2TilesA
Tileset_MikasallaInteriors2TilesA:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_2/tiles_a.4bpp.mtf"
	.byte 0x8c, 0xeb	@ PACKER: left in the buffer by Field_MikasallaInteriors2+0x11f6
	.global Tileset_MikasallaInteriors2TilesB
Tileset_MikasallaInteriors2TilesB:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_2/tiles_b.4bpp.mtf"
	.byte 0xc8, 0x4f, 0x0d	@ PACKER: left in the buffer by Tileset_MikasallaInteriors2TilesA+0xec1
	.global Tileset_MikasallaInteriors2TilesC
Tileset_MikasallaInteriors2TilesC:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_2/tiles_c.4bpp.mtf"
	.global Tileset_MikasallaInteriors2TilesD
Tileset_MikasallaInteriors2TilesD:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_2/tiles_d.4bpp.mtf"
	.byte 0x20, 0x3c	@ PACKER: left in the buffer by Tileset_MikasallaInteriors2TilesC+0xf1e
