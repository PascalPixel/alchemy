	.global Field_MadraCatacombsEntrance
Field_MadraCatacombsEntrance:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombsEntrance, .LField_MadraCatacombsEntrance_CELL_TILES
	map_block Field_MadraCatacombsEntrance, .LField_MadraCatacombsEntrance_SHAPES
	map_block Field_MadraCatacombsEntrance, .LField_MadraCatacombsEntrance_GRID
	map_block Field_MadraCatacombsEntrance, .LField_MadraCatacombsEntrance_SHAPE_GRID
	map_block Field_MadraCatacombsEntrance, .LField_MadraCatacombsEntrance_ANIMATION
	map_block Field_MadraCatacombsEntrance, .LField_MadraCatacombsEntrance_BLEND
	map_block Field_MadraCatacombsEntrance, .LField_MadraCatacombsEntrance_SCENE
.LField_MadraCatacombsEntrance_CELL_TILES:
	.incbin "games/tla/src/map/madra_catacombs_entrance/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombsEntrance_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_entrance/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombsEntrance_GRID:
	.incbin "games/tla/src/map/madra_catacombs_entrance/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombsEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombsEntrance_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombsEntrance_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombsEntrance_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraCatacombsEntrancePalette
Tileset_MadraCatacombsEntrancePalette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_entrance/palette.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_MadraCatacombs1+0x10b
	.global Tileset_MadraCatacombsEntranceTilesA
Tileset_MadraCatacombsEntranceTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_entrance/tiles_a.4bpp.mtf"
	.byte 0xce	@ PACKER: left in the buffer by Field_MadraRoomsCavernHomes+0x212f
	.global Tileset_MadraCatacombsEntranceTilesB
Tileset_MadraCatacombsEntranceTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_entrance/tiles_b.4bpp.mtf"
	.byte 0x22, 0x0e, 0xf8	@ PACKER: left in the buffer by Tileset_MadraCatacombsEntranceTilesA+0x1bd5
	.global Tileset_MadraCatacombsEntranceTilesC
Tileset_MadraCatacombsEntranceTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_entrance/tiles_c.4bpp.mtf"
	.byte 0x19	@ PACKER: left in the buffer by Tileset_MadraCatacombsEntranceTilesB+0x60f
	.global Tileset_MadraCatacombsEntranceTilesD
Tileset_MadraCatacombsEntranceTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_entrance/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_MadraCatacombsEntranceTilesC+0x13f
