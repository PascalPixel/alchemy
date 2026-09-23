@ The three mask words of EXPAND_OPAQUE.S, 0x00ff00ff, 0x0000ffff and
@ 0x11111111, which it loads with add sl, pc, #100; ldm sl, {r4, r6, r7}
@ and which its loader copies to RAM with it. A data table carries no
@ credit, as TBS GRAPHICS/TILE/EXPAND_OPAQUE_MASKS.S.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08038ea4
	.4byte	0x00ff00ff
	.4byte	0x0000ffff
	.4byte	0x11111111
