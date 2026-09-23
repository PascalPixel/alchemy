@ Second battle-effect model decoder, the first member of another 0x234-byte
@ unit that battle-effect code copies into a heap block, calls and frees
@ (size and source 0x081a029c in its literal pool at 0x081a17a8). Behind a
@ 4-byte Thumb entry (bx pc) it rebuilds the dispatcher's jump table for the
@ address it runs at from its own position (add r3, pc; sub) and calls the
@ dispatcher with bl while its state stays in registers. Neither approved
@ compiler relocates code at run time or passes state that way. It is a
@ TLA-only variant of SENTOU_KOUKA_TENKAI.S: three instruction words differ,
@ an inner loop count of 64 (mov r7, #64) and a 0x1000-byte column step where
@ that decoder has 32 and 0x800, and no TBS edition has it. Its data words
@ (0x081a036c and 0x081a0380) are SENTOU_KOUKA_TENKAI_2_LINK.S and
@ SENTOU_KOUKA_TENKAI_2_TABLES.S and carry no credit; neither does its
@ entry.
@ credit: handwritten — TLA variant model/bit-stream decoder of the battle-effect bank
.syntax unified
	.set SentouKouka_TenkaiYomi2, 0x081a0374
	.thumb
	.set AlchemyLoadAddress, 0x081a029c
	.global SentouKouka_Tenkai2
	.thumb_func
SentouKouka_Tenkai2:
AlchemyUncredited_081a029c:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_081a029c:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r2
	ldr	r2, [pc, #192]
	add	r3, pc, #236
	sub	r2, r3, r2
	add	r4, pc, #196
	mov	r5, #8
.L_081a02bc:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne	.L_081a02bc
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
.L_081a02e4:
	mov	r8, #8
.L_081a02e8:
	mov	r7, #64
.L_081a02ec:
	bl	SentouKouka_TenkaiYomi2
	lsr	r6, r5, #25
	bl	SentouKouka_TenkaiYomi2
	orr	r6, r6, r5, lsr #17
	bl	SentouKouka_TenkaiYomi2
	orr	r6, r6, r5, lsr #9
	bl	SentouKouka_TenkaiYomi2
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl	SentouKouka_TenkaiYomi2
	lsr	r6, r5, #25
	bl	SentouKouka_TenkaiYomi2
	orr	r6, r6, r5, lsr #17
	bl	SentouKouka_TenkaiYomi2
	orr	r6, r6, r5, lsr #9
	bl	SentouKouka_TenkaiYomi2
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne	.L_081a02ec
	sub	r1, r1, #0x1000
	add	r1, r1, #8
	subs	r8, r8, #1
	bne	.L_081a02e8
	sub	r1, r1, #64
	add	r1, r1, #0x1000
	subs	r9, r9, #1
	bne	.L_081a02e4
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
