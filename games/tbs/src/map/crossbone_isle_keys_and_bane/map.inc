	.global Field_CrossboneIsleKeysAndBane
Field_CrossboneIsleKeysAndBane:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_CELL_TILES
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_SHAPES
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_GRID
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_ANIMATION
	map_offset Field_CrossboneIsleKeysAndBane
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_MARKERS
.LField_CrossboneIsleKeysAndBane_CELL_TILES:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/map_ja.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_ANIMATION:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_MARKERS:
	treasure_spot 58, 36, 107
	treasure_spot 13, 38, 100
	treasure_spot 17, 38, 101
	treasure_spot 21, 38, 102
	treasure_spot 25, 38, 103
	treasure_spot 40, 47, 104
	treasure_spot 49, 47, 106
	treasure_spot 45, 53, 105
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_CELL_TILES
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_SHAPES
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_GRID
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_ANIMATION
	map_offset Field_CrossboneIsleKeysAndBane
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_MARKERS
.LField_CrossboneIsleKeysAndBane_CELL_TILES:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/map_ja.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_ANIMATION:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_MARKERS:
	treasure_spot 58, 36, 107
	treasure_spot 13, 38, 100
	treasure_spot 17, 38, 101
	treasure_spot 21, 38, 102
	treasure_spot 25, 38, 103
	treasure_spot 40, 47, 104
	treasure_spot 49, 47, 106
	treasure_spot 45, 53, 105
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_CELL_TILES
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_SHAPES
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_GRID
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_ANIMATION
	map_offset Field_CrossboneIsleKeysAndBane
	map_offset Field_CrossboneIsleKeysAndBane, .LField_CrossboneIsleKeysAndBane_MARKERS
.LField_CrossboneIsleKeysAndBane_CELL_TILES:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_ANIMATION:
	.incbin "games/tbs/src/map/crossbone_isle_keys_and_bane/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleKeysAndBane_MARKERS:
	treasure_spot 58, 36, 107
	treasure_spot 13, 38, 100
	treasure_spot 17, 38, 101
	treasure_spot 21, 38, 102
	treasure_spot 25, 38, 103
	treasure_spot 40, 47, 104
	treasure_spot 49, 47, 106
	treasure_spot 45, 53, 105
	treasure_spots_end
	.endif
	.endif
	.balign 4, 0
	.global Tileset_PaletteCrossboneIsleKeys
Tileset_PaletteCrossboneIsleKeys:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle_keys.gbapal.lz"
	.byte 0x10, 0x2f, 0x79	@ PACKER: left in the buffer by Field_CrossboneIsleKeysAndBane+0x181
	.balign 4, 0
