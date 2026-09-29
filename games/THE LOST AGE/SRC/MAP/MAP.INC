@ A field map resource: a header, the offsets of its blocks from the map's
@ first byte, and the blocks, each word-aligned with zeros. The cell block
@ follows the header; an offset of 0 marks a block the map lacks.

@ The header's first fourteen bytes: two words of settings, the number of
@ layers and their drawing order, and two layer settings.
	.macro map_header a:req, b:req, c:req, layers:req, o0:req, o1:req, o2:req, d:req, e:req, f:req, g:req
	.2byte \a
	.byte \b, \c, \layers, \o0, \o1, \o2, \d, \e
	.2byte \f, \g
	.endm

@ One layer's record: its cell size in pixels, its origin and its scroll rate.
	.macro map_layer cell_w:req, cell_h:req, x:req, y:req, rate:req
	.byte \cell_w, \cell_h
	.2byte \x, \y, \rate
	.endm

@ The offset of a block from the map's first byte, or 0 without one.
	.macro map_block map:req, block
	.ifnb \block
	.4byte \block - \map
	.else
	.4byte 0
	.endif
	.endm
