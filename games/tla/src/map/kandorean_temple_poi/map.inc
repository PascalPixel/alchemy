	.global Field_KandoreanTemplePoi
Field_KandoreanTemplePoi:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTemplePoi, .LField_KandoreanTemplePoi_CELL_TILES
	map_block Field_KandoreanTemplePoi, .LField_KandoreanTemplePoi_SHAPES
	map_block Field_KandoreanTemplePoi, .LField_KandoreanTemplePoi_GRID
	map_block Field_KandoreanTemplePoi, .LField_KandoreanTemplePoi_SHAPE_GRID
	map_block Field_KandoreanTemplePoi, .LField_KandoreanTemplePoi_ANIMATION
	map_block Field_KandoreanTemplePoi, .LField_KandoreanTemplePoi_BLEND
	map_block Field_KandoreanTemplePoi, .LField_KandoreanTemplePoi_SCENE
.LField_KandoreanTemplePoi_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_poi/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_KandoreanTemplePoi_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_poi/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTemplePoi_GRID:
	.incbin "games/tla/src/map/kandorean_temple_poi/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTemplePoi_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_poi/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTemplePoi_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTemplePoi_BLEND:
	.incbin "games/tla/src/map/kandorean_temple_poi/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTemplePoi_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_poi/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KandoreanTemplePoiPalette
Tileset_KandoreanTemplePoiPalette:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_poi/palette.gbapal.lz"
	.global Tileset_KandoreanTemplePoiTilesA
Tileset_KandoreanTemplePoiTilesA:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_poi/tiles_a.4bpp.mtf"
	.global Tileset_KandoreanTemplePoiTilesB
Tileset_KandoreanTemplePoiTilesB:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_poi/tiles_b.4bpp.mtf"
	.global Tileset_KandoreanTemplePoiTilesC
Tileset_KandoreanTemplePoiTilesC:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_poi/tiles_c.4bpp.mtf"
	.byte 0xc6	@ PACKER: left in the buffer by Tileset_KandoreanTemplePoiTilesA+0x2287
	.global Tileset_KandoreanTemplePoiTilesD
Tileset_KandoreanTemplePoiTilesD:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_poi/tiles_d.4bpp.mtf"
	.byte 0x3d, 0x87, 0xa4	@ PACKER: left in the buffer by Tileset_KandoreanTemplePoiTilesC+0x4d1
