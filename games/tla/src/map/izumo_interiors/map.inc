	.global Field_IzumoInteriors
Field_IzumoInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IzumoInteriors, .LField_IzumoInteriors_CELL_PICTURES
	map_block Field_IzumoInteriors, .LField_IzumoInteriors_SHAPES
	map_block Field_IzumoInteriors, .LField_IzumoInteriors_GRID
	map_block Field_IzumoInteriors, .LField_IzumoInteriors_SHAPE_GRID
	map_block Field_IzumoInteriors, .LField_IzumoInteriors_ANIMATION
	map_block Field_IzumoInteriors, .LField_IzumoInteriors_BLEND
	map_block Field_IzumoInteriors, .LField_IzumoInteriors_SCENE
.LField_IzumoInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/izumo_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_IzumoInteriors_SHAPES:
	.incbin "games/tla/src/map/izumo_interiors/shapes.table.lz"
	.balign 4, 0
.LField_IzumoInteriors_GRID:
	.incbin "games/tla/src/map/izumo_interiors/map.grid.lz"
	.balign 4, 0
.LField_IzumoInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/izumo_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_IzumoInteriors_ANIMATION:
	.incbin "games/tla/src/map/izumo_interiors/animation.script.lz"
	.balign 4, 0
.LField_IzumoInteriors_BLEND:
	.incbin "games/tla/src/map/izumo_interiors/blend.script.lz"
	.balign 4, 0
.LField_IzumoInteriors_SCENE:
	.incbin "games/tla/src/map/izumo_interiors/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set74Palette
Tileset_Set74Palette:
	.incbin "games/tla/src/graphics/tileset/set_74/palette.gbapal.lz"
	.byte 0xcc, 0x0c, 0x26	@ PACKER: left in the buffer by Field_IzumoInteriors+0x1b1
	.global Tileset_Set74TilesA
Tileset_Set74TilesA:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_a.4bpp.mtf"
	.byte 0x05, 0x80, 0x00	@ PACKER: left in the buffer by Field_IzumoInteriors+0x1701
	.global Tileset_Set74TilesB
Tileset_Set74TilesB:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_b.4bpp.mtf"
	.global Tileset_Set74TilesC
Tileset_Set74TilesC:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_c.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Tileset_Set74TilesB+0xc8f
	.global Tileset_Set74TilesD
Tileset_Set74TilesD:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_d.4bpp.mtf"
	.byte 0xb1, 0x74, 0x4f	@ PACKER: left in the buffer by Tileset_Set74TilesB+0x1349
