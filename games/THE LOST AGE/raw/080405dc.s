.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.set sub_08016ce4, 0x08016ce4
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08041c54, 0x08041c54
	.set sub_08041fa4, 0x08041fa4
	.set sub_08042314, 0x08042314
	.set sub_08044460, 0x08044460
	.set sub_08044f88, 0x08044f88
	.set sub_08045018, 0x08045018
	.set sub_08045048, 0x08045048
	.set sub_080450ac, 0x080450ac
	.set sub_080f8028, 0x080f8028
	.set sub_08108030, 0x08108030
	.set sub_08108040, 0x08108040
	.set sub_08108048, 0x08108048
	.set sub_081c0010, 0x081c0010
	.global Overlay_080405dc
Overlay_080405dc:
.L_080405dc:
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #208
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	.4byte 0x8500018a
	.2byte 0x05ad
	.2byte 0x0804
.L_08040614:
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #208
	bl	sub_0801314c
	pop	{pc}
	.2byte 0x05ad
	.2byte 0x0804
.L_08040628:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	movs	r0, #191
	movs	r2, #3
	lsls	r0, r0, #1
	sub	sp, #12
	mov	fp, r3
	mov	sl, r2
	bl	sub_08016ce4
	movs	r3, #0
	mov	r9, r0
	str	r3, [sp, #8]
	cmp	r0, #0
	beq.n	.L_08040660
	movs	r3, #2
	str	r3, [sp, #8]
	movs	r2, #1
	mov	sl, r2
.L_08040660:
	ldr	r3, [pc, #288]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804066c
	movs	r2, #3
	add	sl, r2
.L_0804066c:
	movs	r3, #8
	mov	r2, sl
	subs	r1, r3, r2
	lsls	r3, r2, #1
	add	r3, sl
	adds	r4, r3, #1
	adds	r3, r1, r4
	cmp	r3, #19
	ble.n	.L_08040682
	movs	r1, #1
	movs	r4, #19
.L_08040682:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #5
	adds	r3, r4, #0
	movs	r2, #20
	bl	sub_08039260
	mov	r3, sl
	mov	r8, r0
	cmp	r3, #1
	ble.n	.L_080406b4
	mov	r5, sl
	movs	r6, #3
	subs	r5, #1
.L_0804069e:
	adds	r2, r6, #0
	mov	r0, r8
	movs	r1, #0
	movs	r3, #19
	subs	r5, #1
	str	r6, [sp, #0]
	bl	sub_08041c54
	adds	r6, #3
	cmp	r5, #0
	bne.n	.L_0804069e
.L_080406b4:
	mov	r2, r9
	movs	r7, #4
	cmp	r2, #0
	bne.n	.L_080406da
	ldr	r5, [pc, #200]
	mov	r1, r8
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #4
	adds	r5, #1
	bl	sub_08041fa4
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #48
	movs	r3, #28
	bl	sub_08041fa4
	movs	r7, #52
.L_080406da:
	adds	r3, r7, #0
	ldr	r0, [pc, #172]
	mov	r1, r8
	movs	r2, #48
	bl	sub_08041fa4
	ldr	r3, [pc, #156]
	adds	r7, #24
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804071c
	ldr	r5, [pc, #156]
	adds	r3, r7, #0
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #48
	adds	r7, #24
	bl	sub_08041fa4
	adds	r0, r5, #1
	adds	r3, r7, #0
	mov	r1, r8
	movs	r2, #48
	adds	r7, #24
	adds	r5, #2
	bl	sub_08041fa4
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #48
	adds	r3, r7, #0
	bl	sub_08041fa4
.L_0804071c:
	bl	sub_08044460
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	mov	r2, r8
	str	r3, [sp, #0]
	bl	sub_08042314
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	add	r3, fp
	str	r0, [r3, #0]
	movs	r7, #4
	mov	r3, sl
	negs	r7, r7
	cmp	r3, #0
	ble.n	.L_08040772
	ldr	r3, [pc, #80]
	ldr	r2, [sp, #8]
	movs	r4, #194
	lsls	r4, r4, #3
	add	r4, fp
	mov	r5, sl
	adds	r6, r2, r3
.L_08040750:
	ldrb	r0, [r6, #0]
	movs	r1, #0
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	mov	r2, r8
	movs	r3, #12
	str	r7, [sp, #0]
	str	r4, [sp, #4]
	bl	sub_080450ac
	ldr	r4, [sp, #4]
	subs	r5, #1
	adds	r6, #1
	stmia	r4!, {r0}
	adds	r7, #24
	cmp	r5, #0
	bne.n	.L_08040750
.L_08040772:
	mov	r0, r8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001238
	.4byte 0x00001154
	.4byte 0x00001156
	.4byte 0x00001158
	.2byte 0xea8f
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #1
	mov	r8, r0
	movs	r0, #191
	sub	sp, #20
	negs	r1, r1
	movs	r2, #3
	lsls	r0, r0, #1
	str	r1, [sp, #12]
	mov	fp, r2
	bl	sub_08016ce4
	movs	r3, #0
	str	r3, [sp, #4]
	adds	r5, r0, #0
	bl	.L_080405dc
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	movs	r0, #1
	mov	sl, r3
	bl	sub_08013560
	bl	.L_08040628
	movs	r3, #16
	str	r0, [sp, #16]
	negs	r3, r3
	movs	r0, #7
	ldr	r1, [sp, #16]
	movs	r2, #40
	bl	sub_08044f88
	str	r0, [sp, #8]
	cmp	r5, #0
	beq.n	.L_080407f8
	movs	r1, #2
	str	r1, [sp, #4]
	movs	r0, #1
	mov	fp, r0
.L_080407f8:
	ldr	r3, [pc, #388]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08040804
	movs	r2, #3
	add	fp, r2
.L_08040804:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_080408b2
	mov	r0, r8
	add	r0, fp
	mov	r1, fp
	bl	sub_08002064
	movs	r3, #160
	lsls	r3, r3, #3
	mov	r8, r0
	adds	r3, #116
	add	r3, sl
	mov	r0, r8
	movs	r7, #0
	strh	r0, [r3, #0]
	cmp	r7, fp
	bge.n	.L_0804086c
	ldr	r1, [pc, #344]
	movs	r4, #194
	ldr	r6, [sp, #4]
	lsls	r4, r4, #3
	mov	r9, r1
	add	r4, sl
.L_08040834:
	ldmia	r4!, {r5}
	movs	r3, #251
	strb	r3, [r5, #15]
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	sub_080f8028
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #116
	add	r3, sl
	ldrh	r3, [r3, #0]
	ldrb	r1, [r5, #14]
	movs	r2, #0
	ldr	r4, [sp, #0]
	cmp	r7, r3
	beq.n	.L_08040858
	movs	r2, #1
.L_08040858:
	mov	r3, r9
	ldrsb	r0, [r6, r3]
	str	r4, [sp, #0]
	bl	sub_08045048
	adds	r7, #1
	adds	r6, #1
	ldr	r4, [sp, #0]
	cmp	r7, fp
	blt.n	.L_08040834
.L_0804086c:
	ldr	r1, [sp, #16]
	movs	r0, #12
	ldrsh	r3, [r1, r0]
	ldr	r0, [sp, #16]
	lsls	r1, r3, #3
	movs	r3, #14
	ldrsh	r2, [r0, r3]
	mov	r0, r8
	lsls	r3, r0, #1
	add	r3, r8
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r2, r3, #0
	ldr	r3, [sp, #12]
	movs	r0, #1
	negs	r0, r0
	adds	r2, #16
	cmp	r3, r0
	bne.n	.L_080408a0
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, sl
	bl	sub_08108048
	b.n	.L_080408ae
.L_080408a0:
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, sl
	movs	r3, #3
	bl	sub_08108040
.L_080408ae:
	movs	r1, #0
	str	r1, [sp, #12]
.L_080408b2:
	ldr	r0, [sp, #8]
	bl	sub_08045018
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #200]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080408d4
	movs	r0, #112
	mov	r5, r8
	bl	sub_081c0010
	b.n	.L_08040954
.L_080408d4:
	ldr	r2, [r1, #4]
	movs	r3, #10
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080408ea
	movs	r5, #1
	movs	r0, #113
	negs	r5, r5
	bl	sub_081c0010
	b.n	.L_08040954
.L_080408ea:
	ldr	r2, [r1, #12]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08040906
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #1
	negs	r2, r2
	movs	r3, #1
	add	r8, r2
	str	r3, [sp, #12]
	b.n	.L_08040804
.L_08040906:
	ldr	r2, [r1, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0804091e
	movs	r0, #111
	bl	sub_081c0010
	movs	r0, #1
	add	r8, r0
	str	r0, [sp, #12]
	b.n	.L_08040804
.L_0804091e:
	ldr	r3, [pc, #96]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08040928
	b.n	.L_08040804
.L_08040928:
	ldr	r3, [r1, #4]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804093e
	movs	r0, #112
	movs	r5, #20
	bl	sub_081c0010
	b.n	.L_08040954
.L_0804093e:
	ldr	r3, [r1, #4]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804094c
	b.n	.L_08040804
.L_0804094c:
	movs	r0, #112
	movs	r5, #21
	bl	sub_081c0010
.L_08040954:
	ldr	r0, [sp, #16]
	movs	r1, #2
	bl	sub_0803939c
	bl	.L_08040614
	movs	r0, #1
	bl	sub_08013560
	cmp	r5, #0
	blt.n	.L_0804096e
	ldr	r1, [sp, #4]
	adds	r5, r5, r1
.L_0804096e:
	adds	r0, r5, #0
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001238
	.4byte 0x0805ea8f
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r0, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	adds	r0, r0, r3
	bl	sub_08108030
	pop	{pc}
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #208
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	.4byte 0x8500018a
	.2byte 0x098d
	.2byte 0x0804
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #208
	bl	sub_0801314c
	pop	{pc}
	.2byte 0x098d
	.2byte 0x0804
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	sub	sp, #4
	ldr	r6, [r3, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r2, #15
	movs	r3, #9
	movs	r0, #7
	bl	sub_08039260
	adds	r5, r0, #0
	bl	sub_08044460
	movs	r1, #128
	movs	r3, #0
	adds	r2, r5, #0
	lsls	r1, r1, #23
	str	r3, [sp, #0]
	bl	sub_08042314
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	adds	r6, r6, r3
	str	r0, [r6, #0]
	adds	r0, r6, #0
	movs	r3, #12
	ldrsh	r1, [r5, r3]
	movs	r3, #14
	ldrsh	r2, [r5, r3]
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #4
	adds	r2, #12
	bl	sub_08108048
	adds	r0, r5, #0
	add	sp, #4
	pop	{r5, r6, pc}
