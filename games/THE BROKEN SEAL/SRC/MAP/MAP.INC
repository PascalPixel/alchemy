@ A field map resource, as Map_LoadLayeredScene reads it: a header, three
@ layer records, the offsets of its blocks from the map's first byte, and the
@ blocks, each word-aligned with zeros. An offset of 0 marks a block the map
@ lacks. The blocks are, in offset order:
@   METATILES  each metatile's four tile entries, delta-coded and packed,
@              decoded into gMapBlocks
@   SHAPES     the terrain shapes: a height function (low nibble) and its
@              height bytes, indexed by a cell's shape
@   GRID       the 128x128 cell grid, packed as three planes: each cell's
@              collision code, its shape, and its metatile (0xfff numbers a
@              cell of its own); Tilemap_ConvertBuffer interleaves them into
@              four-byte cells
@   ANIMATION  the tile animation script MapAnimation_StartChannels runs
@   BLEND      the blend script DisplayBlend_StartScript runs
@   MARKERS    x, z and event of each hidden-item marker, ending 0xff, 0xff,
@              read in place by FieldEffect_SpawnNearbyMarkers

@ The camera bounds in 8-pixel tiles, then the three layers' BG priorities and
@ character blocks, and two bytes the loader does not read.
	.macro map_header left:req, top:req, right:req, bottom:req, priority0:req, priority1:req, priority2:req, chars0:req, chars1:req, chars2:req, pad0:req, pad1:req
	.byte \left, \top, \right, \bottom
	.byte \priority0, \priority1, \priority2, \chars0, \chars1, \chars2
	.byte \pad0, \pad1
	.endm

@ One layer: its origin in the cell grid in 8-pixel tiles, its scroll rate
@ against the camera and its drift per frame in 1/16 pixels, and two bytes
@ the loader keeps as halfwords.
	.macro map_layer x:req, y:req, rate_x:req, rate_y:req, drift_x:req, drift_y:req, a:req, b:req
	.byte \x, \y, \rate_x, \rate_y, \drift_x, \drift_y, \a, \b
	.endm

@ The offset of a block from the map's first byte, or 0 without one.
	.macro map_block map:req, block
	.ifnb \block
	.4byte \block - \map
	.else
	.4byte 0
	.endif
	.endm
