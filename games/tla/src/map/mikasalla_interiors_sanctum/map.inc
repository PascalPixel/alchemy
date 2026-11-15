	.global Field_MikasallaInteriorsSanctum
Field_MikasallaInteriorsSanctum:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MikasallaInteriorsSanctum, .LField_MikasallaInteriorsSanctum_CELL_PICTURES
	map_block Field_MikasallaInteriorsSanctum, .LField_MikasallaInteriorsSanctum_SHAPES
	map_block Field_MikasallaInteriorsSanctum, .LField_MikasallaInteriorsSanctum_GRID
	map_block Field_MikasallaInteriorsSanctum, .LField_MikasallaInteriorsSanctum_SHAPE_GRID
	map_block Field_MikasallaInteriorsSanctum, .LField_MikasallaInteriorsSanctum_ANIMATION
	map_block Field_MikasallaInteriorsSanctum, .LField_MikasallaInteriorsSanctum_BLEND
	map_block Field_MikasallaInteriorsSanctum, .LField_MikasallaInteriorsSanctum_SCENE
.LField_MikasallaInteriorsSanctum_CELL_PICTURES:
	.incbin "games/tla/src/map/mikasalla_interiors_sanctum/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MikasallaInteriorsSanctum_SHAPES:
	.incbin "games/tla/src/map/mikasalla_interiors_sanctum/shapes.table.lz"
	.balign 4, 0
.LField_MikasallaInteriorsSanctum_GRID:
	.incbin "games/tla/src/map/mikasalla_interiors_sanctum/map.grid.lz"
	.balign 4, 0
.LField_MikasallaInteriorsSanctum_SHAPE_GRID:
	.incbin "games/tla/src/map/mikasalla_interiors_sanctum/shape_grid.plane.lz"
	.balign 4, 0
.LField_MikasallaInteriorsSanctum_ANIMATION:
	.incbin "games/tla/src/map/madra_interiors_cavern_homes/animation.script.lz"
	.balign 4, 0
.LField_MikasallaInteriorsSanctum_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_MikasallaInteriorsSanctum_SCENE:
	.incbin "games/tla/src/map/mikasalla_interiors_sanctum/scene.scene.lz"
	.global Tileset_MikasallaInteriorsSanctumPalette
Tileset_MikasallaInteriorsSanctumPalette:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_sanctum/palette.gbapal.lz"
	.byte 0xe7, 0x0b	@ PACKER: left in the buffer by Field_MikasallaInteriors2+0x186
	.global Tileset_MikasallaInteriorsSanctumTilesA
Tileset_MikasallaInteriorsSanctumTilesA:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_sanctum/tiles_a.4bpp.mtf"
	.byte 0x8c, 0xeb	@ PACKER: left in the buffer by Field_MikasallaInteriors2+0x11f6
	.global Tileset_MikasallaInteriorsSanctumTilesB
Tileset_MikasallaInteriorsSanctumTilesB:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_sanctum/tiles_b.4bpp.mtf"
	.byte 0xc8, 0x4f, 0x0d	@ PACKER: left in the buffer by Tileset_MikasallaInteriorsSanctumTilesA+0xec1
	.global Tileset_MikasallaInteriorsSanctumTilesC
Tileset_MikasallaInteriorsSanctumTilesC:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_sanctum/tiles_c.4bpp.mtf"
	.global Tileset_MikasallaInteriorsSanctumTilesD
Tileset_MikasallaInteriorsSanctumTilesD:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors_sanctum/tiles_d.4bpp.mtf"
	.byte 0x20, 0x3c	@ PACKER: left in the buffer by Tileset_MikasallaInteriorsSanctumTilesC+0xf1e
