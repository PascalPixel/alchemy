@ The nine words before IWRAM_RUNTIME_MIX_RENDER.S (run 0x03000d1c) that
@ TLA's mixer shares with its renderer: a counter that
@ IWRAM_RUNTIME_MIX_LANES.S increments (ldr r6, [pc, #1160];
@ str r6, [pc, #1152]) and the renderer reads and rewrites
@ (ldr r1, [pc, #-52]; str r1, [pc, #-64]); two words that the Thumb tail
@ of IWRAM_RUNTIME_MIX_CHANNEL.S stores (add r5, pc, #20;
@ stmia r5!, {r4, r6}) and the renderer reads; a word the renderer rewrites
@ (strgt r9, [pc, #-324]); a zero word; and the renderer's
@ parameters 0x02003a90, 0xb0, 0xfff80035 and 0x0034fff8, which it reads
@ with negative offsets from pc. Run-time variables and the parameters laid
@ out among them carry no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080012d4
	.set AlchemyRunAddress, 0x03000d1c
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02003a90, 0x000000b0, 0x00000000, 0xfff80035
	.4byte	0x0034fff8
