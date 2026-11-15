	.global Field_MikasallaInteriors
Field_MikasallaInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MikasallaInteriors, .LField_MikasallaInteriors_CELL_PICTURES
	map_block Field_MikasallaInteriors, .LField_MikasallaInteriors_SHAPES
	map_block Field_MikasallaInteriors, .LField_MikasallaInteriors_GRID
	map_block Field_MikasallaInteriors, .LField_MikasallaInteriors_SHAPE_GRID
	map_block Field_MikasallaInteriors, .LField_MikasallaInteriors_ANIMATION
	map_block Field_MikasallaInteriors, .LField_MikasallaInteriors_BLEND
	map_block Field_MikasallaInteriors, .LField_MikasallaInteriors_SCENE
.LField_MikasallaInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/mikasalla_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MikasallaInteriors_SHAPES:
	.incbin "games/tla/src/map/mikasalla_interiors/shapes.table.lz"
	.balign 4, 0
.LField_MikasallaInteriors_GRID:
	.incbin "games/tla/src/map/mikasalla_interiors/map.grid.lz"
	.balign 4, 0
.LField_MikasallaInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/mikasalla_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_MikasallaInteriors_ANIMATION:
	.incbin "games/tla/src/map/mikasalla_interiors/animation.script.lz"
	.balign 4, 0
.LField_MikasallaInteriors_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_MikasallaInteriors_SCENE:
	.incbin "games/tla/src/map/mikasalla_interiors/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MikasallaInteriorsPalette
Tileset_MikasallaInteriorsPalette:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors/palette.gbapal.lz"
	.global Tileset_MikasallaInteriorsTilesA
Tileset_MikasallaInteriorsTilesA:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors/tiles_a.4bpp.mtf"
	.byte 0x40, 0x4e, 0x01	@ PACKER: left in the buffer by Field_MikasallaInteriors+0x20ed
	.global Tileset_MikasallaInteriorsTilesB
Tileset_MikasallaInteriorsTilesB:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors/tiles_b.4bpp.mtf"
	.byte 0x43, 0x23	@ PACKER: left in the buffer by Tileset_MikasallaInteriorsTilesA+0x1a1a
	.global Tileset_MikasallaInteriorsTilesC
Tileset_MikasallaInteriorsTilesC:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors/tiles_c.4bpp.mtf"
	.byte 0x6e	@ PACKER: left in the buffer by Tileset_MikasallaInteriorsTilesB+0x110b
	.global Tileset_MikasallaInteriorsTilesD
Tileset_MikasallaInteriorsTilesD:
	.incbin "games/tla/src/graphics/tileset/mikasalla_interiors/tiles_d.4bpp.mtf"
