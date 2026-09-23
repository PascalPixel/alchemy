@ Battle-effect model decoder, the first member of the 560-byte block that
@ the routine at 0x080f02e8 copies into heap slot 49 with DMA3 (size 0x230
@ and source 0x080f0024 in its literal pool at 0x080f0360), calls and frees.
@ At entry it rebuilds the dispatcher's jump table for the address it runs
@ at from its own position (ldr r2, [pc, #192]; add r3, pc, #236;
@ add r4, pc, #196), then calls the dispatcher of
@ SENTOU_KOUKA_TENKAI_DISPATCH.S with bl, keeping the decoder state in r2,
@ r3, r5 and fp across those calls. Neither approved compiler relocates
@ code at run time or passes state that way. These 204 bytes equal TLA
@ BATTLE/EFFECT/SENTOU_KOUKA_TENKAI.S after its Thumb entry (0x081a0050).
@ The block's data words (SENTOU_KOUKA_TENKAI_LINK.S and
@ SENTOU_KOUKA_TENKAI_TABLES.S) carry no credit, as in TLA.
@ credit: library — TBS/TLA common ARM model/bit-stream decoder bank, battle-effect link
.syntax unified
.text
	.arm
	.set AlchemyLoadAddress, 0x080f0024
	.set	SentouKouka_TenkaiYomi, 0x080f00f8
	.global SentouKouka_Tenkai
SentouKouka_Tenkai:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r2
	ldr	r2, [pc, #192]
	add	r3, pc, #236
	sub	r2, r3, r2
	add	r4, pc, #196
	mov	r5, #8
.L_080f0040:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne .L_080f0040
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
.L_080f0068:
	mov	r8, #8
.L_080f006c:
	mov	r7, #32
.L_080f0070:
	bl SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne .L_080f0070
	sub	r1, r1, #2048
	add	r1, r1, #8
	subs	r8, r8, #1
	bne .L_080f006c
	sub	r1, r1, #64
	add	r1, r1, #2048
	subs	r9, r9, #1
	bne .L_080f0068
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
