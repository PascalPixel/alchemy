	.global Field_MadraCatacombsTablet
Field_MadraCatacombsTablet:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombsTablet, .LField_MadraCatacombsTablet_CELL_TILES
	map_block Field_MadraCatacombsTablet, .LField_MadraCatacombsTablet_SHAPES
	map_block Field_MadraCatacombsTablet, .LField_MadraCatacombsTablet_GRID
	map_block Field_MadraCatacombsTablet, .LField_MadraCatacombsTablet_SHAPE_GRID
	map_block Field_MadraCatacombsTablet, .LField_MadraCatacombsTablet_ANIMATION
	map_block Field_MadraCatacombsTablet, .LField_MadraCatacombsTablet_BLEND
	map_block Field_MadraCatacombsTablet, .LField_MadraCatacombsTablet_SCENE
.LField_MadraCatacombsTablet_CELL_TILES:
	.incbin "games/tla/src/map/madra_catacombs_tablet/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MadraCatacombsTablet_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_tablet/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombsTablet_GRID:
	.incbin "games/tla/src/map/madra_catacombs_tablet/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombsTablet_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_tablet/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombsTablet_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombsTablet_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombsTablet_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_tablet/scene.scene.lz"
	.global Tileset_MadraCatacombsTabletPalette
Tileset_MadraCatacombsTabletPalette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_tablet/palette.gbapal.lz"
	.byte 0x2d, 0x40, 0xff	@ PACKER: left in the buffer by Field_MadraCatacombs6+0x169
	.global Tileset_MadraCatacombsTabletTilesA
Tileset_MadraCatacombsTabletTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_tablet/tiles_a.4bpp.mtf"
	.byte 0x04	@ PACKER: left in the buffer by Field_MadraRoomsCavernHomes+0x25af
	.global Tileset_MadraCatacombsTabletTilesB
Tileset_MadraCatacombsTabletTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_tablet/tiles_b.4bpp.mtf"
	.byte 0xce, 0x80	@ PACKER: left in the buffer by Tileset_MadraCatacombsTabletTilesA+0x1cbe
	.global Tileset_MadraCatacombsTabletTilesC
Tileset_MadraCatacombsTabletTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_tablet/tiles_c.4bpp.mtf"
	.byte 0x20, 0xdb	@ PACKER: left in the buffer by Tileset_MadraCatacombsTabletTilesB+0x1aca
	.global Tileset_MadraCatacombsTabletTilesD
Tileset_MadraCatacombsTabletTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_tablet/tiles_d.4bpp.mtf"
