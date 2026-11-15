	.global Field_YallamInteriorsShops
Field_YallamInteriorsShops:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YallamInteriorsShops, .LField_YallamInteriorsShops_CELL_PICTURES
	map_block Field_YallamInteriorsShops, .LField_YallamInteriorsShops_SHAPES
	map_block Field_YallamInteriorsShops, .LField_YallamInteriorsShops_GRID
	map_block Field_YallamInteriorsShops, .LField_YallamInteriorsShops_SHAPE_GRID
	map_block Field_YallamInteriorsShops, .LField_YallamInteriorsShops_ANIMATION
	map_block Field_YallamInteriorsShops, .LField_YallamInteriorsShops_BLEND
	map_block Field_YallamInteriorsShops, .LField_YallamInteriorsShops_SCENE
.LField_YallamInteriorsShops_CELL_PICTURES:
	.incbin "games/tla/src/map/yallam_interiors_shops/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YallamInteriorsShops_SHAPES:
	.incbin "games/tla/src/map/yallam_interiors_shops/shapes.table.lz"
	.balign 4, 0
.LField_YallamInteriorsShops_GRID:
	.incbin "games/tla/src/map/yallam_interiors_shops/map.grid.lz"
	.balign 4, 0
.LField_YallamInteriorsShops_SHAPE_GRID:
	.incbin "games/tla/src/map/yallam_interiors_shops/shape_grid.plane.lz"
	.balign 4, 0
.LField_YallamInteriorsShops_ANIMATION:
	.incbin "games/tla/src/map/madra_interiors_2/animation.script.lz"
	.balign 4, 0
.LField_YallamInteriorsShops_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_YallamInteriorsShops_SCENE:
	.incbin "games/tla/src/map/yallam_interiors_shops/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteYallamInteriorsShopsA
Tileset_PaletteYallamInteriorsShopsA:
	.incbin "games/tla/src/graphics/tileset/palettes/yallam_interiors_shops_a.gbapal.lz"
	.byte 0xf6, 0x1c, 0xf7	@ PACKER: left in the buffer by Field_YallamInteriorsShops+0x199
	.global Tileset_PaletteYallamInteriorsShopsB
Tileset_PaletteYallamInteriorsShopsB:
	.incbin "games/tla/src/graphics/tileset/palettes/yallam_interiors_shops_b.gbapal.lz"
	.byte 0x44	@ PACKER: left in the buffer by Tileset_PaletteYallamInteriorsShopsA+0x15b
