@ Bank jump tables use the same 8-byte entries as overlay imports and entries.
@ Each bank lists its entries in assembly order: an export is a callable
@ symbol, and a bare overlay_veneer keeps an entry that has no exported name.
@ C interfaces live separately in include/gamepak/far_call; an entry only some
@ editions have is tested by edition. A table's head is Thumb code on a word
@ boundary and the veneer macros; it never reads the overlays' import list.
	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
