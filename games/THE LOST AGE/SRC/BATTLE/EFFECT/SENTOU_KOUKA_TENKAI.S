@ Battle-effect model decoder, the first member of a 0x234-byte unit that
@ battle-effect code copies into a heap block, calls and frees (size and
@ source 0x081a004c in its literal pool at 0x081a05ec). Behind a 4-byte
@ Thumb entry (bx pc) the ARM decoder rebuilds the dispatcher's jump table
@ for the address it runs at from its own position (ldr r2, [pc, #192];
@ add r3, pc, #236; add r4, pc, #196) and calls the dispatcher with bl,
@ keeping the decoder state in r2, r3, r5 and fp across those calls. Neither
@ approved compiler relocates code at run time or passes state that way.
@ After that entry these 204 bytes equal TBS
@ BATTLE/EFFECT/SENTOU_KOUKA_TENKAI.S (0x080f0024). The entry is TLA's own
@ and carries no credit. The unit's data words (an unused 0x80808080 word
@ and the table's link address at 0x081a011c, the runtime table and its
@ eight case addresses at 0x081a0130) are SENTOU_KOUKA_TENKAI_LINK.S and
@ SENTOU_KOUKA_TENKAI_TABLES.S and carry no credit, as in TBS.
@ credit: library — TBS/TLA common ARM model/bit-stream decoder bank, battle-effect link
.syntax unified
	.set SentouKouka_TenkaiYomi, 0x081a0124
	.thumb
	.set AlchemyLoadAddress, 0x081a004c
	.global SentouKouka_Tenkai
	.thumb_func
SentouKouka_Tenkai:
AlchemyUncredited_081a004c:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_081a004c:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r2
	ldr	r2, [pc, #192]
	add	r3, pc, #236
	sub	r2, r3, r2
	add	r4, pc, #196
	mov	r5, #8
.L_081a006c:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne	.L_081a006c
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
.L_081a0094:
	mov	r8, #8
.L_081a0098:
	mov	r7, #32
.L_081a009c:
	bl	SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl	SentouKouka_TenkaiYomi
	lsr	r6, r5, #25
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #17
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #9
	bl	SentouKouka_TenkaiYomi
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne	.L_081a009c
	sub	r1, r1, #0x800
	add	r1, r1, #8
	subs	r8, r8, #1
	bne	.L_081a0098
	sub	r1, r1, #64
	add	r1, r1, #0x800
	subs	r9, r9, #1
	bne	.L_081a0094
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
