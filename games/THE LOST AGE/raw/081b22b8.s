.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_080132fc, 0x080132fc
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080144c0, 0x080144c0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_0801587c, 0x0801587c
	.set sub_08038000, 0x08038000
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038080, 0x08038080
	.set sub_080380b0, 0x080380b0
	.set sub_080ad040, 0x080ad040
	.set sub_080ad2a8, 0x080ad2a8
	.set sub_08138000, 0x08138000
	.set sub_081b203c, 0x081b203c
	.set sub_081b2150, 0x081b2150
	.set sub_081b21ec, 0x081b21ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_081b22b8
Overlay_081b22b8:
.L_081b22b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #96]
	adds	r7, r1, #0
	subs	r6, r2, r0
	movs	r1, #128
	mov	sl, r2
	sub	sp, #4
	mov	r8, r0
	subs	r5, r4, r7
	mov	r9, r1
	mov	fp, r3
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_081b22e8
	negs	r2, r6
.L_081b22e8:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_081b22f0
	negs	r3, r5
.L_081b22f0:
	cmp	r2, r3
	bge.n	.L_081b2398
	cmp	r5, #0
	bge.n	.L_081b230c
	mov	ip, r8
	mov	r8, sl
	mov	sl, ip
	mov	ip, r7
	mov	r2, sl
	adds	r7, r4, #0
	mov	r3, r8
	mov	r4, ip
	subs	r6, r2, r3
	subs	r5, r4, r7
.L_081b230c:
	lsls	r0, r6, #8
	cmp	r6, #0
	bge.n	.L_081b231a
	mov	r1, r8
	mov	r2, sl
	subs	r3, r1, r2
	lsls	r0, r3, #8
.L_081b231a:
	cmp	r5, #0
	blt.n	.L_081b232c
	adds	r1, r5, #0
	str	r4, [sp, #0]
	bl	sub_08002054
	mov	ip, r0
	ldr	r4, [sp, #0]
	b.n	.L_081b2338
.L_081b232c:
	subs	r1, r7, r4
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r4, [sp, #0]
	mov	ip, r0
.L_081b2338:
	adds	r0, r7, #0
	mov	r1, r8
	cmp	r0, r4
	beq.n	.L_081b2428
	ldr	r7, [pc, #244]
	movs	r3, #128
	lsls	r3, r3, #1
	movs	r5, #7
	mov	lr, r3
	mov	r8, r7
.L_081b234c:
	lsrs	r2, r0, #3
	lsrs	r3, r1, #3
	lsls	r2, r2, #5
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r5
	lsls	r2, r2, #3
	mov	r7, fp
	adds	r2, r2, r3
	ldrb	r3, [r7, r2]
	ldr	r7, [sp, #36]
	cmp	r3, r7
	bge.n	.L_081b2372
	mov	r3, fp
	strb	r7, [r3, r2]
.L_081b2372:
	add	r9, ip
	mov	r3, r9
	mov	r7, lr
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_081b2390
	cmp	r6, #0
	ble.n	.L_081b2386
	adds	r1, #1
	b.n	.L_081b2388
.L_081b2386:
	subs	r1, #1
.L_081b2388:
	mov	r2, r9
	mov	r3, r8
	ands	r2, r3
	mov	r9, r2
.L_081b2390:
	adds	r0, #1
	cmp	r0, r4
	bne.n	.L_081b234c
	b.n	.L_081b2428
.L_081b2398:
	cmp	r6, #0
	bge.n	.L_081b23b0
	mov	ip, r8
	mov	r8, sl
	mov	sl, ip
	mov	ip, r7
	mov	r1, sl
	adds	r7, r4, #0
	mov	r2, r8
	mov	r4, ip
	subs	r6, r1, r2
	subs	r5, r4, r7
.L_081b23b0:
	lsls	r0, r5, #8
	cmp	r5, #0
	bge.n	.L_081b23ba
	subs	r3, r7, r4
	lsls	r0, r3, #8
.L_081b23ba:
	cmp	r6, #0
	blt.n	.L_081b23c2
	adds	r1, r6, #0
	b.n	.L_081b23c8
.L_081b23c2:
	mov	r3, r8
	mov	r6, sl
	subs	r1, r3, r6
.L_081b23c8:
	bl	sub_08002054
	mov	ip, r0
	mov	r0, r8
	adds	r1, r7, #0
	cmp	r0, sl
	beq.n	.L_081b2428
	ldr	r2, [pc, #96]
	movs	r7, #128
	movs	r4, #7
	lsls	r7, r7, #1
	mov	lr, r2
.L_081b23e0:
	lsrs	r2, r1, #3
	lsrs	r3, r0, #3
	lsls	r2, r2, #5
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r4
	lsls	r2, r2, #3
	mov	r6, fp
	adds	r2, r2, r3
	ldrb	r3, [r6, r2]
	ldr	r6, [sp, #36]
	cmp	r3, r6
	bge.n	.L_081b2406
	mov	r3, fp
	strb	r6, [r3, r2]
.L_081b2406:
	add	r9, ip
	mov	r3, r9
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_081b2422
	cmp	r5, #0
	ble.n	.L_081b2418
	adds	r1, #1
	b.n	.L_081b241a
.L_081b2418:
	subs	r1, #1
.L_081b241a:
	mov	r6, r9
	mov	r2, lr
	ands	r6, r2
	mov	r9, r6
.L_081b2422:
	adds	r0, #1
	cmp	r0, sl
	bne.n	.L_081b23e0
.L_081b2428:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xfeff
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #180
	ldr	r7, [r3, #0]
	ldr	r2, [r2, #92]
	sub	sp, #40
	movs	r0, #128
	movs	r1, #168
	adds	r1, r1, r7
	str	r2, [sp, #36]
	lsls	r0, r0, #3
	movs	r2, #0
	str	r0, [sp, #32]
	str	r2, [r1, #0]
	mov	fp, r1
	mov	r8, r2
	bl	sub_08014878
	ldr	r3, [pc, #96]
	adds	r5, r7, #0
	ldr	r4, [r3, #0]
	adds	r5, #156
	ldrh	r0, [r5, #0]
	lsls	r4, r4, #16
	ldr	r1, [r3, #0]
	adds	r6, r7, #0
	lsrs	r3, r4, #16
	bics	r3, r0
	adds	r6, #160
	strh	r3, [r6, #0]
	movs	r3, #158
	adds	r3, r3, r7
	movs	r2, #240
	mov	sl, r3
	ands	r1, r2
	mov	r0, sl
	strh	r1, [r0, #0]
	ldrh	r3, [r5, #0]
	ands	r2, r3
	cmp	r2, r1
	bne.n	.L_081b24d8
	adds	r1, r7, #0
	adds	r1, #162
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #12
	bls.n	.L_081b24b0
	movs	r3, #12
	strh	r3, [r1, #0]
	ldr	r2, [pc, #32]
.L_081b24b0:
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_081b24bc
	movs	r3, #4
	strh	r3, [r1, #0]
	b.n	.L_081b24e0
.L_081b24bc:
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
	strh	r3, [r1, #0]
	mov	r2, sl
	mov	r1, r8
	strh	r1, [r2, #0]
	b.n	.L_081b24e0
	movs	r0, r0
	.4byte 0x0000000c
	.2byte 0x1150
	.2byte 0x0300
.L_081b24d8:
	adds	r2, r7, #0
	adds	r2, #162
	movs	r3, #12
	strh	r3, [r2, #0]
.L_081b24e0:
	lsrs	r3, r4, #16
	strh	r3, [r5, #0]
	ldr	r3, [pc, #208]
	ldrb	r3, [r3, #0]
	mov	r9, r3
	cmp	r3, #0
	beq.n	.L_081b24f2
	bl	.L_081b2db8
.L_081b24f2:
	adds	r3, r7, #0
	adds	r3, #140
	str	r3, [sp, #16]
	str	r3, [sp, #28]
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_081b2502
	b.n	.L_081b2644
.L_081b2502:
	movs	r0, #228
	bl	sub_080ad2a8
	mov	sl, r0
	adds	r0, r7, #0
	adds	r0, #152
	str	r0, [sp, #12]
	movs	r2, #128
	mov	r8, r0
	lsls	r2, r2, #3
	ldr	r0, [r0, #0]
	adds	r2, #204
	mov	r1, sl
	adds	r5, r7, r2
	mov	r3, r9
	subs	r0, r1, r0
	ldr	r2, [r5, #0]
	movs	r1, #2
	str	r3, [sp, #0]
	movs	r3, #64
	bl	sub_080380b0
	ldr	r1, [sp, #12]
	movs	r3, #8
	ldr	r0, [r1, #0]
	ldr	r2, [r5, #0]
	movs	r1, #2
	str	r3, [sp, #0]
	movs	r3, #64
	bl	sub_080380b0
	ldrh	r2, [r6, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b2574
	ldr	r0, [sp, #16]
	ldr	r2, [pc, #108]
	movs	r1, #160
	movs	r3, #10
	lsls	r1, r1, #1
	str	r3, [r0, #0]
	adds	r2, r2, r1
	movs	r3, #254
	strb	r3, [r2, #0]
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #148
	adds	r3, r3, r7
	mov	sl, r3
	bl	.L_081b2e0c
.L_081b2574:
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b259a
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	cmp	r3, #3
	bgt.n	.L_081b2594
	cmp	sl, r3
	ble.n	.L_081b2594
	adds	r3, #1
	str	r3, [r0, #0]
	movs	r0, #111
	bl	sub_081c0010
	b.n	.L_081b259a
.L_081b2594:
	movs	r0, #113
	bl	sub_081c0010
.L_081b259a:
	ldrh	r2, [r6, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b25c6
	mov	r1, r8
	ldr	r3, [r1, #0]
	cmp	r3, #1
	ble.n	.L_081b25c0
	subs	r3, #1
	str	r3, [r1, #0]
	movs	r0, #111
	bl	sub_081c0010
	b.n	.L_081b25c6
	.4byte 0x03001214
	.2byte 0x024c
	.2byte 0x0200
.L_081b25c0:
	movs	r0, #113
	bl	sub_081c0010
.L_081b25c6:
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #1
	ldrh	r2, [r6, #0]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081b25e6
	bl	.L_081b2dca
.L_081b25e6:
	b.n	.L_081b25f0
	.4byte 0x00003fd0
	.2byte 0x0010
	.2byte 0x0000
.L_081b25f0:
	ldr	r2, [sp, #28]
	movs	r0, #238
	str	r1, [r2, #0]
	ldr	r3, [sp, #36]
	lsls	r0, r0, #7
	adds	r0, #140
	movs	r1, #153
	adds	r2, r3, r0
	lsls	r1, r1, #3
	movs	r3, #0
	str	r3, [r2, #0]
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_081b262a
.L_081b261a:
	movs	r0, #228
	bl	sub_080ad040
	mov	r0, r8
	ldr	r3, [r0, #0]
	adds	r5, #1
	cmp	r5, r3
	bne.n	.L_081b261a
.L_081b262a:
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #204
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r0, #152
	lsls	r0, r0, #1
	bl	sub_081c0010
	b.n	.L_081b2dca
.L_081b2644:
	cmp	r5, #5
	beq.n	.L_081b264a
	b.n	.L_081b28f4
.L_081b264a:
	movs	r0, #164
	adds	r0, r0, r7
	mov	r8, r0
	movs	r3, #0
	mov	r1, r8
	mov	r9, r3
	ldr	r3, [r1, #0]
	movs	r5, #0
	adds	r3, #1
	str	r3, [r1, #0]
	ldrb	r3, [r7, #25]
	cmp	r3, #0
	beq.n	.L_081b2676
	adds	r2, r7, #0
	adds	r2, #25
.L_081b2668:
	adds	r5, #1
	cmp	r5, #5
	beq.n	.L_081b267a
	adds	r2, #28
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_081b2668
.L_081b2676:
	cmp	r5, #5
	bne.n	.L_081b267e
.L_081b267a:
	movs	r2, #1
	mov	r9, r2
.L_081b267e:
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b2738
	movs	r2, #0
	str	r2, [r1, #0]
	ldr	r0, [sp, #36]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #140
	adds	r3, r0, r1
	str	r2, [r3, #0]
	movs	r3, #148
	adds	r3, r3, r7
	mov	sl, r3
	ldr	r3, [r3, #0]
	mov	r8, sl
	cmp	r3, #4
	bne.n	.L_081b26d0
	adds	r3, r7, #0
	mov	r0, sl
	adds	r3, #144
	str	r2, [r0, #0]
	str	r2, [r3, #0]
	ldr	r1, [sp, #28]
	movs	r6, #0
	str	r2, [r1, #0]
	adds	r2, r7, #0
	movs	r0, #0
	movs	r1, #255
	adds	r2, #24
.L_081b26be:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r0, [r2, #1]
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_081b26be
	b.n	.L_081b278a
.L_081b26d0:
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bgt.n	.L_081b26f8
	movs	r0, #50
	adds	r0, #255
	bl	sub_081c0010
	ldr	r2, [r5, #0]
	movs	r1, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #24
	adds	r3, r7, r3
	ldrb	r2, [r3, #1]
	eors	r2, r1
	strb	r2, [r3, #1]
	b.n	.L_081b278a
.L_081b26f8:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_081b2730
	movs	r0, #152
	lsls	r0, r0, #1
	bl	sub_081c0010
	ldr	r0, [sp, #28]
	mov	r1, r9
	movs	r3, #1
	adds	r2, r7, #0
	str	r3, [r0, #0]
	movs	r6, #0
	str	r1, [r5, #0]
	adds	r2, #24
	movs	r1, #255
.L_081b2718:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_081b2718
	mov	r2, r8
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_081b278a
.L_081b2730:
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_081b278a
.L_081b2738:
	mov	r3, sl
	ldrh	r2, [r3, #0]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b275e
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r0, [r5, #0]
	movs	r1, #6
	adds	r0, #1
	bl	sub_08002064
	str	r0, [r5, #0]
	movs	r0, #111
	bl	sub_081c0010
	mov	r0, sl
	ldrh	r2, [r0, #0]
.L_081b275e:
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b2784
	adds	r5, r7, #0
	adds	r5, #144
	ldr	r0, [r5, #0]
	movs	r1, #6
	adds	r0, #5
	bl	sub_08002064
	str	r0, [r5, #0]
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #148
	adds	r1, r1, r7
	mov	sl, r1
	b.n	.L_081b278a
.L_081b2784:
	movs	r2, #148
	adds	r2, r2, r7
	mov	sl, r2
.L_081b278a:
	ldr	r3, [sp, #28]
	ldr	r3, [r3, #0]
	mov	r8, r3
	cmp	r3, #5
	beq.n	.L_081b2796
	b.n	.L_081b28de
.L_081b2796:
	adds	r3, r7, #0
	adds	r3, #144
	ldr	r2, [r3, #0]
	cmp	r2, #5
	bne.n	.L_081b284e
	mov	r0, r9
	cmp	r0, #0
	beq.n	.L_081b280a
	movs	r1, #195
	lsls	r1, r1, #3
	adds	r6, r7, r1
	ldr	r2, [r6, #0]
	subs	r3, r2, #1
	cmp	r3, #1
	bls.n	.L_081b27e6
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r5, r7, r2
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #19
	movs	r3, #4
	movs	r0, #11
	bl	sub_08038010
	adds	r1, r0, #0
	movs	r3, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #388]
	movs	r2, #0
	bl	sub_08038080
	movs	r3, #1
	str	r3, [r6, #0]
	b.n	.L_081b2dd2
.L_081b27e6:
	cmp	r2, #1
	beq.n	.L_081b27ec
	b.n	.L_081b2dd2
.L_081b27ec:
	movs	r1, #153
	lsls	r1, r1, #3
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r3, #8
	ldr	r0, [pc, #360]
	bl	sub_08038080
	adds	r2, r7, #0
	movs	r3, #2
	adds	r2, #152
	str	r3, [r6, #0]
	str	r2, [sp, #12]
	b.n	.L_081b2e0c
.L_081b280a:
	movs	r3, #195
	lsls	r3, r3, #3
	adds	r6, r7, r3
	ldr	r3, [r6, #0]
	cmp	r3, #3
	beq.n	.L_081b2842
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r5, r7, r0
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #14
	movs	r3, #3
	movs	r0, #16
	bl	sub_08038010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #300]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_081b2842:
	adds	r1, r7, #0
	movs	r3, #3
	adds	r1, #152
	str	r3, [r6, #0]
	str	r1, [sp, #12]
	b.n	.L_081b2e0c
.L_081b284e:
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldrb	r3, [r3, #25]
	cmp	r3, #0
	bne.n	.L_081b289a
	movs	r2, #195
	lsls	r2, r2, #3
	adds	r6, r7, r2
	ldr	r3, [r6, #0]
	cmp	r3, #4
	beq.n	.L_081b2894
	movs	r3, #153
	lsls	r3, r3, #3
	adds	r5, r7, r3
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #3
	movs	r0, #23
	bl	sub_08038010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #224]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_081b2894:
	adds	r0, r7, #0
	movs	r3, #4
	b.n	.L_081b28d6
.L_081b289a:
	movs	r1, #195
	lsls	r1, r1, #3
	adds	r6, r7, r1
	ldr	r3, [r6, #0]
	cmp	r3, #5
	beq.n	.L_081b28d2
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r5, r7, r2
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #3
	movs	r0, #23
	bl	sub_08038010
	adds	r1, r0, #0
	str	r1, [r5, #0]
	ldr	r0, [pc, #164]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_081b28d2:
	adds	r0, r7, #0
	mov	r3, r8
.L_081b28d6:
	adds	r0, #152
	str	r3, [r6, #0]
	str	r0, [sp, #12]
	b.n	.L_081b2e0c
.L_081b28de:
	movs	r1, #153
	lsls	r1, r1, #3
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	adds	r2, r7, #0
	adds	r2, #152
	str	r2, [sp, #12]
	b.n	.L_081b2e0c
.L_081b28f4:
	cmp	r5, #2
	bne.n	.L_081b2974
	movs	r3, #164
	adds	r3, r3, r7
	mov	r8, r3
	ldr	r3, [r3, #0]
	mov	r0, r8
	adds	r3, #1
	movs	r1, #0
	str	r3, [r0, #0]
	str	r1, [sp, #32]
	cmp	r3, #60
	beq.n	.L_081b2910
	b.n	.L_081b2dda
.L_081b2910:
	ldr	r2, [sp, #16]
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r0, #93
	bl	sub_081c0010
	ldr	r3, [sp, #32]
	mov	r0, r8
	str	r3, [r0, #0]
	movs	r2, #128
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [sp, #36]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r1, r2
	str	r5, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	adds	r0, r7, #0
	movs	r1, #148
	movs	r3, #75
	adds	r0, #152
	adds	r1, r1, r7
	str	r3, [r2, #0]
	mov	sl, r1
	str	r0, [sp, #12]
	b.n	.L_081b2e0c
	movs	r0, r0
	.4byte 0x00003f44
	.4byte 0x00001010
	.4byte 0x00000d75
	.4byte 0x00000d76
	.4byte 0x00000d72
	.4byte 0x00000d70
	.2byte 0x0d71
	.2byte 0x0000
.L_081b2974:
	cmp	r5, #3
	bne.n	.L_081b299e
	movs	r2, #164
	ldr	r3, [r2, r7]
	adds	r3, #1
	str	r3, [r2, r7]
	movs	r3, #0
	str	r3, [sp, #32]
	movs	r3, #1
	ldrh	r2, [r6, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081b2990
	b.n	.L_081b2df2
.L_081b2990:
	ldr	r0, [sp, #16]
	movs	r3, #10
	str	r3, [r0, #0]
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_081b29ee
.L_081b299e:
	cmp	r5, #11
	bne.n	.L_081b29fc
	movs	r3, #195
	lsls	r3, r3, #3
	adds	r5, r7, r3
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_081b29c4
	movs	r1, #153
	movs	r3, #1
	lsls	r1, r1, #3
	str	r3, [r5, #0]
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #752]
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
.L_081b29c4:
	ldrh	r2, [r6, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081b29d0
	b.n	.L_081b2de6
.L_081b29d0:
	ldr	r2, [sp, #16]
	movs	r3, #5
	str	r3, [r2, #0]
	mov	r3, r9
	str	r3, [r5, #0]
	movs	r0, #112
	bl	sub_081c0010
	movs	r0, #153
	lsls	r0, r0, #3
	adds	r3, r7, r0
	movs	r1, #1
	ldr	r0, [r3, #0]
	bl	sub_08038018
.L_081b29ee:
	adds	r1, r7, #0
	movs	r2, #148
	adds	r1, #152
	adds	r2, r2, r7
	str	r1, [sp, #12]
	mov	sl, r2
	b.n	.L_081b2e0c
.L_081b29fc:
	cmp	r5, #20
	bne.n	.L_081b2a32
	movs	r3, #164
	adds	r3, r3, r7
	mov	r8, r3
	ldr	r3, [r3, #0]
	mov	r0, r8
	adds	r3, #1
	str	r3, [r0, #0]
	cmp	r3, #45
	beq.n	.L_081b2a14
	b.n	.L_081b2df2
.L_081b2a14:
	ldr	r1, [sp, #16]
	movs	r3, #10
	str	r3, [r1, #0]
	b.n	.L_081b2df2
.L_081b2a1c:
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	adds	r0, #4
	strb	r0, [r5, #2]
	movs	r0, #52
	adds	r0, #255
	bl	sub_081c0010
	b.n	.L_081b2ac2
.L_081b2a32:
	cmp	r5, #10
	bne.n	.L_081b2a38
	b.n	.L_081b2e00
.L_081b2a38:
	movs	r0, #164
	adds	r0, r0, r7
	str	r0, [sp, #24]
	mov	r8, r0
	ldr	r3, [r0, #0]
	cmp	r3, #4
	bne.n	.L_081b2a6e
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #17
	movs	r2, #12
	movs	r3, #3
	movs	r0, #18
	bl	sub_08038010
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r1, r0, #0
	adds	r3, r7, r2
	str	r1, [r3, #0]
	ldr	r0, [pc, #588]
	movs	r3, #0
	movs	r2, #0
	bl	sub_08038080
	mov	r0, r8
	ldr	r3, [r0, #0]
.L_081b2a6e:
	cmp	r3, #16
	bne.n	.L_081b2a7e
	movs	r0, #153
	lsls	r0, r0, #1
	bl	sub_081c0010
	mov	r1, r8
	ldr	r3, [r1, #0]
.L_081b2a7e:
	cmp	r3, #56
	ble.n	.L_081b2ac2
	ldr	r2, [sp, #36]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #140
	adds	r1, r2, r3
	ldr	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_081b2a9e
	ldrh	r2, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081b2ac2
.L_081b2a9e:
	mov	r0, r9
	str	r0, [r1, #0]
	movs	r2, #1
	adds	r5, r7, #0
	movs	r6, #0
	negs	r2, r2
	adds	r5, #24
.L_081b2aac:
	ldrb	r3, [r5, #1]
	cmp	r3, #0
	bne.n	.L_081b2aba
	movs	r3, #2
	ldrsb	r3, [r5, r3]
	cmp	r3, r2
	beq.n	.L_081b2a1c
.L_081b2aba:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L_081b2aac
.L_081b2ac2:
	adds	r2, r7, #0
	movs	r6, #0
	adds	r2, #24
.L_081b2ac8:
	movs	r3, #2
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #2]
	cmp	r3, #0
	ble.n	.L_081b2ad6
	subs	r3, r1, #1
	strb	r3, [r2, #2]
.L_081b2ad6:
	adds	r6, #1
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_081b2ac8
	adds	r2, r7, #0
	movs	r1, #0
	movs	r6, #0
	movs	r4, #15
	adds	r2, #24
	movs	r0, #0
.L_081b2aea:
	ldrb	r3, [r2, #1]
	cmp	r3, #1
	beq.n	.L_081b2b00
	movs	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_081b2b02
	ldr	r3, [r0, r7]
	ands	r3, r4
	cmp	r3, #8
	bne.n	.L_081b2b02
.L_081b2b00:
	adds	r1, #1
.L_081b2b02:
	adds	r6, #1
	adds	r2, #28
	adds	r0, #28
	cmp	r6, #5
	bne.n	.L_081b2aea
	cmp	r1, #5
	beq.n	.L_081b2b12
	b.n	.L_081b2d4e
.L_081b2b12:
	adds	r2, r7, #0
	movs	r1, #0
	adds	r2, #152
	movs	r3, #172
	str	r1, [sp, #20]
	str	r2, [sp, #12]
	adds	r3, r3, r7
	movs	r4, #0
	mov	r8, r3
.L_081b2b24:
	movs	r1, #0
	mov	r2, r8
	str	r1, [r2, #0]
	ldr	r3, [sp, #12]
	movs	r0, #1
	ldr	r2, [r3, #0]
	movs	r3, #3
	negs	r0, r0
	subs	r3, r3, r2
	mov	sl, r0
	mov	r9, r1
	cmp	r4, r3
	ble.n	.L_081b2bd4
	adds	r3, r2, #3
	cmp	r4, r3
	bge.n	.L_081b2bd4
	movs	r6, #0
	movs	r5, #0
.L_081b2b48:
	cmp	r4, #0
	bne.n	.L_081b2b5c
	ldr	r0, [r5, r7]
	cmp	r0, #0
	bge.n	.L_081b2b54
	adds	r0, #15
.L_081b2b54:
	asrs	r0, r0, #4
	subs	r0, r6, r0
	adds	r0, #22
	b.n	.L_081b2b80
.L_081b2b5c:
	cmp	r4, #6
	bne.n	.L_081b2b72
	ldr	r0, [r5, r7]
	negs	r3, r6
	cmp	r0, #0
	bge.n	.L_081b2b6a
	adds	r0, #15
.L_081b2b6a:
	asrs	r0, r0, #4
	subs	r0, r3, r0
	adds	r0, #26
	b.n	.L_081b2b80
.L_081b2b72:
	ldr	r0, [r5, r7]
	cmp	r0, #0
	bge.n	.L_081b2b7a
	adds	r0, #15
.L_081b2b7a:
	asrs	r0, r0, #4
	subs	r0, r4, r0
	adds	r0, #21
.L_081b2b80:
	movs	r1, #21
	str	r4, [sp, #4]
	bl	sub_08002064
	adds	r0, r0, r5
	adds	r0, #4
	ldrb	r3, [r7, r0]
	ldr	r4, [sp, #4]
	cmp	r3, #5
	beq.n	.L_081b2ba8
	movs	r0, #1
	negs	r0, r0
	cmp	sl, r0
	bne.n	.L_081b2ba0
	mov	sl, r3
	b.n	.L_081b2ba8
.L_081b2ba0:
	cmp	sl, r3
	beq.n	.L_081b2ba8
	movs	r1, #1
	mov	r9, r1
.L_081b2ba8:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L_081b2b48
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_081b2bd4
	movs	r3, #1
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r1, [sp, #20]
	movs	r3, #160
	lsls	r3, r3, #1
	adds	r2, r1, r3
	ldr	r3, [pc, #236]
	mov	r0, sl
	ldrb	r3, [r3, r0]
	ldr	r1, [pc, #236]
	strb	r3, [r1, r2]
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
.L_081b2bd4:
	movs	r3, #4
	adds	r4, #1
	add	r8, r3
	cmp	r4, #7
	bne.n	.L_081b2b24
	ldr	r0, [sp, #24]
	movs	r5, #0
	str	r5, [r0, #0]
	ldr	r1, [sp, #20]
	cmp	r1, #0
	beq.n	.L_081b2c38
	movs	r3, #160
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r0, #1
	ldr	r1, [pc, #196]
	negs	r0, r0
	adds	r3, r0, #0
	strb	r3, [r1, r2]
	ldr	r2, [sp, #28]
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r0, #171
	bl	sub_081c0010
	ldr	r3, [sp, #36]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r1, [sp, #36]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r1, r2
	str	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r0, #153
	adds	r3, #80
	lsls	r0, r0, #3
	strh	r5, [r3, #0]
	adds	r3, r7, r0
	movs	r1, #1
	ldr	r0, [r3, #0]
	bl	sub_08038018
	movs	r1, #148
	b.n	.L_081b2d56
.L_081b2c38:
	ldr	r2, [sp, #28]
	movs	r3, #11
	str	r3, [r2, #0]
	ldr	r1, [sp, #20]
	movs	r0, #195
	movs	r2, #153
	lsls	r0, r0, #3
	lsls	r2, r2, #3
	adds	r3, r7, r0
	adds	r6, r7, r2
	str	r1, [r3, #0]
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #148
	adds	r3, r3, r7
	mov	sl, r3
	ldr	r3, [r3, #0]
	mov	r8, sl
	cmp	r3, #3
	bgt.n	.L_081b2c96
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #24
	movs	r3, #4
	movs	r0, #3
	bl	sub_08038010
	ldr	r5, [pc, #68]
	adds	r1, r0, #0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	adds	r0, r5, #0
	movs	r3, #8
	ldr	r1, [r6, #0]
	movs	r2, #0
	bl	sub_08038080
	mov	r0, sl
	ldr	r3, [r0, #0]
.L_081b2c96:
	cmp	r3, #4
	bne.n	.L_081b2d5a
	movs	r0, #228
	bl	sub_080ad2a8
	cmp	r0, #0
	ble.n	.L_081b2cc0
	ldr	r1, [sp, #28]
	movs	r3, #20
	str	r3, [r1, #0]
	b.n	.L_081b2cc6
	.4byte 0x00000d6f
	.4byte 0x00000d6d
	.4byte 0x081b489c
	.4byte 0x0200024c
	.2byte 0x0d6e
	.2byte 0x0000
.L_081b2cc0:
	ldr	r2, [sp, #28]
	movs	r3, #20
	str	r3, [r2, #0]
.L_081b2cc6:
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	cmp	r3, r0
	ble.n	.L_081b2cd0
	str	r0, [r1, #0]
.L_081b2cd0:
	movs	r2, #0
	mov	r3, r8
	str	r2, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #144
	str	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r6, #0
	movs	r0, #0
	movs	r1, #255
	adds	r2, #24
.L_081b2ce6:
	ldrb	r3, [r2, #2]
	adds	r6, #1
	orrs	r3, r1
	strb	r0, [r2, #1]
	strb	r3, [r2, #2]
	adds	r2, #28
	cmp	r6, #5
	bne.n	.L_081b2ce6
	ldr	r0, [sp, #36]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	adds	r3, r0, r2
	movs	r2, #0
	str	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #12
	movs	r3, #4
	movs	r0, #18
	bl	sub_08038010
	movs	r3, #128
	ldr	r5, [pc, #804]
	lsls	r3, r3, #3
	adds	r3, #204
	adds	r1, r0, #0
	adds	r6, r7, r3
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #8
	subs	r5, #1
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	b.n	.L_081b2d5a
.L_081b2d4e:
	adds	r0, r7, #0
	adds	r0, #152
	movs	r1, #148
	str	r0, [sp, #12]
.L_081b2d56:
	adds	r1, r1, r7
	mov	sl, r1
.L_081b2d5a:
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bne.n	.L_081b2d9e
	movs	r6, #0
	adds	r1, r7, #0
.L_081b2d66:
	ldrb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081b2d96
	movs	r3, #26
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_081b2d78
	ldr	r3, [r1, #0]
	b.n	.L_081b2d84
.L_081b2d78:
	ldr	r2, [r1, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #8
	beq.n	.L_081b2d8a
	adds	r3, r2, #0
.L_081b2d84:
	adds	r3, #8
	str	r3, [r1, #0]
	adds	r2, r3, #0
.L_081b2d8a:
	movs	r3, #168
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_081b2d96
	movs	r3, #0
	str	r3, [r1, #0]
.L_081b2d96:
	adds	r6, #1
	adds	r1, #28
	cmp	r6, #5
	bne.n	.L_081b2d66
.L_081b2d9e:
	ldr	r0, [sp, #36]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #140
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	adds	r2, #1
	str	r2, [r3, #0]
	ldr	r2, [sp, #24]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_081b2e0c
.L_081b2db8:
	adds	r3, r7, #0
	adds	r0, r7, #0
	movs	r1, #148
	adds	r3, #140
	adds	r0, #152
	adds	r1, r1, r7
	str	r3, [sp, #16]
	str	r0, [sp, #12]
	b.n	.L_081b2e0a
.L_081b2dca:
	movs	r2, #148
	adds	r2, r2, r7
	mov	sl, r2
	b.n	.L_081b2e0c
.L_081b2dd2:
	adds	r3, r7, #0
	adds	r3, #152
	str	r3, [sp, #12]
	b.n	.L_081b2e0c
.L_081b2dda:
	adds	r0, r7, #0
	movs	r1, #148
	adds	r0, #152
	adds	r1, r1, r7
	str	r0, [sp, #12]
	b.n	.L_081b2e0a
.L_081b2de6:
	adds	r0, r7, #0
	movs	r1, #148
	adds	r0, #152
	adds	r1, r1, r7
	str	r0, [sp, #12]
	b.n	.L_081b2e0a
.L_081b2df2:
	adds	r2, r7, #0
	movs	r3, #148
	adds	r2, #152
	adds	r3, r3, r7
	str	r2, [sp, #12]
	mov	sl, r3
	b.n	.L_081b2e0c
.L_081b2e00:
	adds	r0, r7, #0
	adds	r0, #152
	movs	r1, #148
	str	r0, [sp, #12]
	adds	r1, r1, r7
.L_081b2e0a:
	mov	sl, r1
.L_081b2e0c:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	cmp	r3, #5
	bne.n	.L_081b2ec4
	adds	r3, r7, #0
	adds	r3, #144
	ldr	r1, [r3, #0]
	movs	r2, #15
	lsls	r3, r1, #3
	adds	r3, r3, r1
	lsls	r3, r3, #2
	adds	r5, r3, #0
	movs	r3, #164
	adds	r3, r3, r7
	ldr	r3, [r3, #0]
	adds	r5, #36
	ands	r3, r2
	movs	r6, #128
	movs	r0, #0
	cmp	r3, #7
	bgt.n	.L_081b2e38
	movs	r0, #1
.L_081b2e38:
	cmp	r1, #5
	bne.n	.L_081b2e40
	movs	r5, #208
	movs	r6, #32
.L_081b2e40:
	mov	r2, fp
	ldr	r1, [r2, #0]
	adds	r3, r5, #0
	ldr	r2, [sp, #32]
	subs	r3, #12
	lsls	r3, r3, #16
	orrs	r3, r2
	adds	r4, r6, #0
	ldr	r2, [pc, #512]
	adds	r4, #8
	lsls	r1, r1, #3
	orrs	r3, r4
	orrs	r3, r2
	adds	r1, #200
	str	r3, [r7, r1]
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r2, #172
	lsls	r2, r2, #2
	lsls	r3, r3, #3
	lsls	r0, r0, #4
	adds	r0, r0, r2
	adds	r3, #204
	str	r0, [r7, r3]
	adds	r3, r5, #0
	ldr	r2, [r1, #0]
	adds	r3, #12
	adds	r2, #1
	str	r2, [r1, #0]
	ldr	r1, [sp, #32]
	lsls	r3, r3, #16
	orrs	r3, r1
	ldr	r1, [pc, #468]
	lsls	r2, r2, #3
	orrs	r3, r4
	orrs	r3, r1
	adds	r2, #200
	str	r3, [r7, r2]
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #456]
	lsls	r3, r3, #3
	adds	r3, #204
	str	r0, [r7, r3]
	mov	r3, fp
	ldr	r2, [r2, #0]
	adds	r2, #1
	str	r2, [r3, #0]
	ldr	r0, [sp, #32]
	lsls	r3, r5, #16
	orrs	r3, r0
	lsls	r2, r2, #3
	orrs	r3, r6
	orrs	r3, r1
	adds	r2, #200
	str	r3, [r7, r2]
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r2, #248
	lsls	r3, r3, #3
	adds	r3, #204
	lsls	r2, r2, #1
	str	r2, [r7, r3]
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
.L_081b2ec4:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	cmp	r3, #3
	bne.n	.L_081b2f82
	ldr	r0, [pc, #400]
	movs	r3, #164
	ldr	r4, [sp, #36]
	adds	r3, r3, r7
	movs	r6, #0
	mov	r8, r3
	mov	r5, fp
	mov	ip, r0
.L_081b2edc:
	movs	r1, #2
	ldrsh	r2, [r4, r1]
	ldr	r3, [sp, #32]
	lsls	r2, r2, #16
	orrs	r2, r3
	movs	r1, #6
	ldrsh	r3, [r4, r1]
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r0, [r5, #0]
	adds	r3, r3, r1
	movs	r1, #255
	ands	r3, r1
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #24
	lsls	r0, r0, #3
	orrs	r2, r3
	adds	r0, #200
	str	r2, [r7, r0]
	mov	r2, ip
	ldrb	r3, [r2, r6]
	ldr	r1, [r5, #0]
	movs	r0, #220
	lsls	r3, r3, #4
	lsls	r0, r0, #2
	movs	r2, #240
	lsls	r2, r2, #8
	lsls	r1, r1, #3
	adds	r3, r3, r0
	orrs	r3, r2
	adds	r1, #204
	str	r3, [r7, r1]
	movs	r1, #128
	ldr	r2, [r4, #16]
	ldr	r3, [r4, #4]
	lsls	r1, r1, #7
	adds	r3, r3, r2
	adds	r2, r2, r1
	str	r2, [r4, #16]
	str	r3, [r4, #4]
	mov	r2, r8
	ldr	r3, [r2, #0]
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_081b2f3a
	adds	r2, #255
.L_081b2f3a:
	asrs	r2, r2, #8
	lsls	r2, r2, #8
	subs	r2, r3, r2
	lsls	r3, r6, #2
	adds	r3, #200
	cmp	r2, r3
	bne.n	.L_081b2f52
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r4, #16]
	movs	r3, #0
	str	r3, [r4, #24]
.L_081b2f52:
	ldr	r3, [r4, #4]
	movs	r2, #128
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_081b2f74
	ldr	r1, [r4, #24]
	str	r2, [r4, #4]
	cmp	r1, #1
	bgt.n	.L_081b2f70
	ldr	r3, [r4, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r4, #16]
.L_081b2f70:
	adds	r3, r1, #1
	str	r3, [r4, #24]
.L_081b2f74:
	ldr	r3, [r5, #0]
	adds	r6, #1
	adds	r3, #1
	str	r3, [r5, #0]
	adds	r4, #28
	cmp	r6, #8
	bne.n	.L_081b2edc
.L_081b2f82:
	ldr	r3, [pc, #208]
	ldr	r6, [pc, #220]
	ldr	r4, [pc, #224]
	movs	r5, #0
	mov	r0, fp
	mov	ip, r3
.L_081b2f8e:
	ldrb	r2, [r4, #0]
	ldr	r1, [sp, #32]
	lsls	r2, r2, #16
	ldr	r3, [r0, #0]
	orrs	r2, r1
	ldrb	r1, [r6, #0]
	lsls	r3, r3, #3
	orrs	r2, r1
	mov	r1, ip
	adds	r3, #200
	orrs	r2, r1
	adds	r4, #1
	adds	r6, #1
	str	r2, [r7, r3]
	cmp	r5, #3
	bgt.n	.L_081b2fb6
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #156
	b.n	.L_081b2fbc
.L_081b2fb6:
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r2, #157
.L_081b2fbc:
	lsls	r3, r3, #3
	adds	r3, #204
	lsls	r2, r2, #3
	str	r2, [r7, r3]
	ldr	r3, [r0, #0]
	adds	r5, #1
	adds	r3, #1
	str	r3, [r0, #0]
	cmp	r5, #14
	bne.n	.L_081b2f8e
	ldr	r5, [pc, #152]
	movs	r4, #128
	adds	r0, r7, #0
	movs	r6, #0
	mov	r1, fp
	lsls	r4, r4, #14
	adds	r0, #25
.L_081b2fde:
	ldrb	r3, [r0, #0]
	adds	r0, #28
	cmp	r3, #0
	bne.n	.L_081b2ff8
	ldr	r2, [r1, #0]
	ldr	r3, [sp, #32]
	lsls	r2, r2, #3
	orrs	r3, r4
	adds	r2, #200
	orrs	r3, r5
	str	r3, [r7, r2]
	movs	r2, #140
	b.n	.L_081b3008
.L_081b2ff8:
	ldr	r2, [r1, #0]
	ldr	r3, [sp, #32]
	lsls	r2, r2, #3
	orrs	r3, r4
	adds	r2, #200
	orrs	r3, r5
	str	r3, [r7, r2]
	movs	r2, #144
.L_081b3008:
	ldr	r3, [r1, #0]
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	adds	r3, #204
	str	r2, [r7, r3]
	ldr	r3, [r1, #0]
	movs	r2, #144
	adds	r3, #1
	lsls	r2, r2, #14
	adds	r6, #1
	str	r3, [r1, #0]
	adds	r4, r4, r2
	cmp	r6, #5
	bne.n	.L_081b2fde
	movs	r5, #128
	movs	r4, #128
	movs	r6, #0
	mov	r0, fp
	lsls	r5, r5, #3
	lsls	r4, r4, #14
.L_081b3030:
	ldr	r1, [r0, #0]
	ldr	r3, [sp, #32]
	ldr	r2, [pc, #56]
	lsls	r1, r1, #3
	orrs	r3, r4
	adds	r1, #200
	orrs	r3, r2
	str	r3, [r7, r1]
	mov	r1, sl
	ldr	r3, [r1, #0]
	cmp	r6, r3
	bne.n	.L_081b3074
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r1, #132
	b.n	.L_081b307a
	.4byte 0x00000d68
	.4byte 0x80006000
	.4byte 0x90006000
	.4byte 0x80002000
	.4byte 0x081b48a2
	.4byte 0x081b48b8
	.4byte 0x081b48aa
	.4byte 0x8000207c
	.2byte 0x6003
	.2byte 0x8000
.L_081b3074:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r1, #136
.L_081b307a:
	lsls	r3, r6, #5
	lsls	r1, r1, #2
	lsls	r2, r2, #3
	adds	r3, r3, r1
	adds	r2, #204
	orrs	r3, r5
	str	r3, [r7, r2]
	ldr	r3, [r0, #0]
	movs	r2, #128
	adds	r3, #1
	lsls	r2, r2, #13
	adds	r6, #1
	str	r3, [r0, #0]
	adds	r4, r4, r2
	cmp	r6, #5
	bne.n	.L_081b3030
	movs	r4, #0
	mov	r0, fp
	movs	r5, #5
.L_081b30a0:
	movs	r2, #1
	ands	r2, r4
	movs	r3, #129
	lsls	r2, r2, #3
	lsls	r3, r3, #2
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r3, r2
	ldr	r2, [sp, #32]
	ldr	r1, [r0, #0]
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [pc, #444]
	lsls	r1, r1, #3
	orrs	r3, r5
	orrs	r3, r2
	adds	r1, #200
	str	r3, [r7, r1]
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #0]
	movs	r3, #3
	subs	r3, r3, r2
	cmp	r4, r3
	ble.n	.L_081b30e2
	adds	r3, r2, #3
	cmp	r4, r3
	bge.n	.L_081b30e2
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r2, #186
	b.n	.L_081b30e6
.L_081b30e2:
	ldr	r3, [r0, #0]
	movs	r2, #162
.L_081b30e6:
	lsls	r3, r3, #3
	adds	r3, #204
	lsls	r2, r2, #3
	str	r2, [r7, r3]
	ldr	r3, [r0, #0]
	adds	r4, #1
	adds	r3, #1
	str	r3, [r0, #0]
	adds	r5, #16
	cmp	r4, #7
	bne.n	.L_081b30a0
	movs	r2, #160
	lsls	r2, r2, #14
	str	r2, [sp, #8]
	movs	r6, #0
	mov	sl, fp
	adds	r5, r7, #0
.L_081b3108:
	ldr	r3, [sp, #8]
	movs	r4, #0
	mov	r9, r3
.L_081b310e:
	ldr	r2, [r5, #0]
	lsls	r1, r4, #4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_081b311a
	adds	r3, #15
.L_081b311a:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r2, r3
	mov	r0, sl
	adds	r3, r1, r3
	mov	r2, r9
	ldr	r1, [r0, #0]
	adds	r3, #4
	orrs	r3, r2
	ldr	r2, [pc, #336]
	lsls	r1, r1, #3
	orrs	r3, r2
	adds	r1, #200
	str	r3, [r7, r1]
	ldr	r3, [r0, #0]
	ldr	r0, [r5, #0]
	lsls	r3, r3, #3
	adds	r3, #204
	mov	r8, r3
	cmp	r0, #0
	bge.n	.L_081b3146
	adds	r0, #15
.L_081b3146:
	asrs	r0, r0, #4
	subs	r0, r4, r0
	movs	r1, #21
	adds	r0, #21
	str	r4, [sp, #4]
	bl	sub_08002064
	adds	r0, #4
	ldrb	r3, [r5, r0]
	movs	r2, #128
	lsls	r3, r3, #4
	lsls	r2, r2, #4
	orrs	r3, r2
	mov	r0, r8
	str	r3, [r7, r0]
	mov	r1, fp
	ldr	r3, [r1, #0]
	ldr	r4, [sp, #4]
	adds	r3, #1
	adds	r4, #1
	str	r3, [r1, #0]
	cmp	r4, #7
	bne.n	.L_081b310e
	ldr	r2, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r2, r3
	adds	r6, #1
	str	r2, [sp, #8]
	adds	r5, #28
	cmp	r6, #5
	bne.n	.L_081b3108
	ldr	r0, [sp, #16]
	movs	r1, #40
	ldr	r3, [r0, #0]
	cmp	r3, #1
	bne.n	.L_081b31c0
	adds	r3, r7, #0
	adds	r3, #164
	ldr	r0, [r3, #0]
	cmp	r0, #47
	bgt.n	.L_081b31aa
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	muls	r0, r3
	bl	sub_08002096
	lsls	r0, r0, #6
	b.n	.L_081b31ba
.L_081b31aa:
	cmp	r0, #55
	bgt.n	.L_081b31c0
	ldr	r1, [pc, #212]
	lsls	r0, r0, #12
	adds	r0, r0, r1
	bl	sub_08002096
	lsls	r0, r0, #2
.L_081b31ba:
	asrs	r0, r0, #16
	adds	r1, r0, #0
	adds	r1, #40
.L_081b31c0:
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r0, [sp, #32]
	ldr	r2, [pc, #192]
	orrs	r1, r0
	lsls	r3, r3, #3
	orrs	r1, r2
	adds	r3, #200
	str	r1, [r7, r3]
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r2, #160
	lsls	r3, r3, #3
	adds	r3, #204
	lsls	r2, r2, #3
	str	r2, [r7, r3]
	movs	r5, #0
	ldr	r3, [r1, #0]
	mov	r0, fp
	adds	r3, #1
	str	r3, [r1, #0]
	movs	r4, #12
.L_081b31ec:
	ldr	r3, [r0, #0]
	ldr	r2, [sp, #32]
	ldr	r1, [pc, #152]
	lsls	r3, r3, #3
	orrs	r2, r4
	adds	r3, #200
	orrs	r2, r1
	str	r2, [r7, r3]
	cmp	r5, #0
	bne.n	.L_081b3208
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #168
	b.n	.L_081b3232
.L_081b3208:
	cmp	r5, #1
	bne.n	.L_081b3214
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r2, #170
	b.n	.L_081b3232
.L_081b3214:
	cmp	r5, #6
	bne.n	.L_081b3220
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #174
	b.n	.L_081b3232
.L_081b3220:
	cmp	r5, #7
	bne.n	.L_081b322c
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r2, #176
	b.n	.L_081b3232
.L_081b322c:
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #172
.L_081b3232:
	lsls	r3, r3, #3
	adds	r3, #204
	lsls	r2, r2, #3
	str	r2, [r7, r3]
	ldr	r3, [r0, #0]
	adds	r5, #1
	adds	r3, #1
	str	r3, [r0, #0]
	adds	r4, #16
	cmp	r5, #8
	bne.n	.L_081b31ec
	mov	r1, fp
	ldr	r3, [r1, #0]
	cmp	r3, #128
	beq.n	.L_081b326e
	ldr	r0, [pc, #60]
	movs	r2, #0
.L_081b3254:
	ldr	r3, [r1, #0]
	lsls	r3, r3, #3
	adds	r3, #200
	str	r0, [r7, r3]
	ldr	r3, [r1, #0]
	lsls	r3, r3, #3
	adds	r3, #204
	str	r2, [r7, r3]
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #128
	bne.n	.L_081b3254
.L_081b326e:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x80002000
	.4byte 0x80006000
	.4byte 0xfffd0000
	.4byte 0x80d06000
	.4byte 0x80ce6000
	.2byte 0x2000
	.2byte 0x40f0
	push	{r5, r6, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #180
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #92]
	mov	ip, r3
	adds	r3, #140
	ldr	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L_081b3310
	mov	r6, ip
	movs	r5, #0
	adds	r6, #164
	movs	r4, #200
	adds	r1, r2, #0
.L_081b32b6:
	ldr	r2, [r1, #16]
	ldr	r3, [r1, #4]
	adds	r3, r3, r2
	str	r3, [r1, #4]
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r2, r2, r3
	str	r2, [r1, #16]
	ldr	r2, [r6, #0]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_081b32d0
	adds	r3, #255
.L_081b32d0:
	asrs	r3, r3, #8
	lsls	r3, r3, #8
	subs	r3, r2, r3
	cmp	r3, r4
	bne.n	.L_081b32e4
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r1, #16]
	movs	r3, #0
	str	r3, [r1, #24]
.L_081b32e4:
	ldr	r3, [r1, #4]
	movs	r2, #128
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_081b3306
	ldr	r0, [r1, #24]
	str	r2, [r1, #4]
	cmp	r0, #1
	bgt.n	.L_081b3302
	ldr	r3, [r1, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r1, #16]
.L_081b3302:
	adds	r3, r0, #1
	str	r3, [r1, #24]
.L_081b3306:
	adds	r5, #1
	adds	r4, #4
	adds	r1, #28
	cmp	r5, #8
	bne.n	.L_081b32b6
.L_081b3310:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r0, #155
	movs	r1, #128
	lsls	r0, r0, #3
	lsls	r1, r1, #19
	ldrh	r2, [r3, #10]
	add	r0, ip
	adds	r1, #84
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, ip
	adds	r3, #168
	ldr	r2, [r3, #0]
	cmp	r2, #0
	ble.n	.L_081b3366
	movs	r4, #132
	movs	r3, #128
	lsls	r4, r4, #24
	mov	r0, ip
	lsls	r2, r2, #1
	lsls	r3, r3, #19
	movs	r1, #224
	adds	r3, #212
	adds	r0, #200
	lsls	r1, r1, #19
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_081b3366:
	pop	{r5, r6, pc}
	.2byte 0x0001
	.2byte 0xa260
.L_081b336c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #180
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #92]
	mov	r8, r3
	ldr	r3, [pc, #104]
	mov	sl, r2
	movs	r2, #128
	sub	sp, #4
	movs	r5, #0
	movs	r6, #0
	movs	r1, #0
	lsls	r2, r2, #4
.L_081b3392:
	adds	r6, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r6, r2
	bne.n	.L_081b3392
	ldr	r0, [pc, #80]
	ldr	r1, [pc, #84]
	ldr	r7, [pc, #84]
	movs	r6, #0
	movs	r4, #0
	mov	r2, sl
.L_081b33a8:
	adds	r3, r5, #0
	adds	r3, #24
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	ldrb	r3, [r1, #0]
	adds	r6, #1
	str	r0, [r2, #4]
	str	r4, [r2, #16]
	str	r4, [r2, #24]
	adds	r1, #1
	adds	r5, r5, r3
	adds	r0, r0, r7
	adds	r2, #28
	cmp	r6, #8
	bne.n	.L_081b33a8
	movs	r3, #155
	ldr	r2, [pc, #28]
	lsls	r3, r3, #3
	movs	r6, #0
	add	r3, r8
.L_081b33d0:
	adds	r6, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r6, #160
	bne.n	.L_081b33d0
	movs	r7, #160
	lsls	r7, r7, #3
	adds	r7, #6
	movs	r6, #0
	add	r7, r8
	b.n	.L_081b33fc
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x02010018
	.4byte 0xffe00000
	.4byte 0x081b48c6
	.2byte 0x0000
	.2byte 0xfff8
.L_081b33fc:
	.2byte 0x239a
	adds	r3, #255
	adds	r5, r6, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #15
	strh	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	movs	r2, #110
	subs	r2, r2, r6
	movs	r1, #155
	lsls	r3, r0, #1
	lsls	r1, r1, #3
	lsls	r2, r2, #1
	adds	r3, r3, r0
	adds	r2, r2, r1
	lsrs	r3, r3, #15
	mov	r1, r8
	adds	r6, #1
	adds	r7, #2
	strh	r3, [r1, r2]
	cmp	r6, #40
	bne.n	.L_081b33fc
	mov	r3, r8
	movs	r2, #0
	adds	r3, #148
	str	r2, [r3, #0]
	movs	r1, #239
	subs	r3, #8
	str	r2, [r3, #0]
	lsls	r1, r1, #7
	adds	r3, #4
	str	r2, [r3, #0]
	add	r1, sl
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	add	r3, sl
	str	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #12
	movs	r3, #4
	movs	r0, #18
	bl	sub_08038010
	movs	r6, #128
	ldr	r5, [pc, #44]
	lsls	r6, r6, #3
	adds	r6, #204
	adds	r1, r0, #0
	add	r6, r8
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	subs	r5, #1
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0d68
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #14
	movs	r0, #100
	sub	sp, #120
	bl	sub_08014cc0
	movs	r1, #128
	str	r0, [sp, #48]
	lsls	r1, r1, #8
	movs	r0, #96
	bl	sub_08014cc0
	movs	r1, #246
	lsls	r1, r1, #7
	str	r0, [sp, #44]
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014d00
	movs	r1, #192
	lsls	r1, r1, #3
	str	r0, [sp, #40]
	adds	r1, #28
	movs	r0, #180
	bl	sub_08014d00
	str	r0, [sp, #36]
	ldr	r0, [pc, #76]
	ldr	r5, [pc, #76]
	bl	sub_080132fc
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r5, r5, r0
	movs	r3, #255
	strb	r3, [r5, #0]
	ldr	r2, [sp, #36]
	movs	r3, #0
	adds	r2, #162
	strh	r3, [r2, #0]
	ldr	r1, [sp, #36]
	movs	r3, #1
	adds	r1, #152
	str	r1, [sp, #32]
	str	r3, [r1, #0]
	bl	sub_080144c0
	ldr	r5, [pc, #24]
	ldr	r3, [pc, #40]
	ldr	r1, [pc, #44]
	strb	r5, [r3, #0]
	ldr	r7, [pc, #20]
	ldr	r4, [pc, #20]
	movs	r0, #0
	mov	r8, r0
	movs	r6, #0
.L_081b3528:
	movs	r2, #0
	mov	sl, r2
	b.n	.L_081b354c
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0000a1a6
	.4byte 0x0000a1a8
	.4byte 0x0000000c
	.4byte 0x0200024c
	.4byte 0x0300120c
	.2byte 0x2800
	.2byte 0x0600
.L_081b354c:
	mov	r3, sl
	subs	r3, #5
	cmp	r3, #19
	bhi.n	.L_081b3564
	cmp	r6, #2
	ble.n	.L_081b3564
	cmp	r6, #13
	bgt.n	.L_081b3564
	mov	r2, r8
	adds	r3, r2, r1
	strh	r7, [r3, #0]
	b.n	.L_081b357c
.L_081b3564:
	mov	r3, sl
	cmp	r3, #29
	ble.n	.L_081b3572
	mov	r2, r8
	adds	r3, r2, r1
	strh	r5, [r3, #0]
	b.n	.L_081b357c
.L_081b3572:
	mov	r3, r8
	adds	r2, r3, r1
	adds	r3, r0, r4
	strh	r3, [r2, #0]
	adds	r0, #1
.L_081b357c:
	movs	r2, #1
	add	sl, r2
	movs	r3, #2
	mov	r2, sl
	add	r8, r3
	cmp	r2, #32
	bne.n	.L_081b354c
	adds	r6, #1
	cmp	r6, #20
	bne.n	.L_081b3528
	ldr	r0, [pc, #188]
	bl	sub_08013300
	ldr	r1, [sp, #48]
	bl	sub_0801587c
	ldr	r0, [pc, #180]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r4, r0, #0
	adds	r3, #212
	ldr	r1, [pc, #168]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #32
	adds	r0, r4, #0
	ldr	r1, [pc, #160]
	bl	sub_0801587c
	ldr	r0, [pc, #152]
	ldr	r7, [pc, #156]
	movs	r3, #0
	mov	r8, r3
	movs	r6, #0
	movs	r5, #0
	mov	ip, r0
.L_081b35ce:
	movs	r1, #0
	lsls	r3, r5, #6
	mov	r2, ip
	mov	sl, r1
	adds	r4, r3, r2
.L_081b35d8:
	mov	r3, sl
	subs	r3, #5
	cmp	r3, #19
	bhi.n	.L_081b35e8
	cmp	r6, #2
	ble.n	.L_081b35e8
	cmp	r6, #13
	ble.n	.L_081b3602
.L_081b35e8:
	mov	r2, r8
	adds	r1, r2, r7
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #32
	add	r8, r3
.L_081b3602:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r4, #32
	cmp	r1, #30
	bne.n	.L_081b35d8
	adds	r6, #1
	adds	r5, #15
	cmp	r6, #20
	bne.n	.L_081b35ce
	movs	r1, #192
	ldr	r3, [pc, #72]
	ldr	r0, [pc, #76]
	lsls	r1, r1, #2
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0a
	ldr	r5, [pc, #68]
	movs	r2, #0
	movs	r4, #2
	mov	r8, r2
	movs	r6, #0
	mov	ip, r3
	negs	r4, r4
	movs	r0, #0
.L_081b3634:
	movs	r7, #0
	adds	r2, r0, #0
	mov	sl, r7
	adds	r1, r4, #0
	adds	r2, #148
.L_081b363e:
	cmp	r1, #14
	bls.n	.L_081b3670
	mov	r7, r8
	adds	r3, r7, r5
	mov	r7, ip
	strh	r7, [r3, #0]
	b.n	.L_081b3676
	.4byte 0x000000bf
	.4byte 0x00000137
	.4byte 0x00000081
	.4byte 0x05000140
	.2byte 0x0000
	.2byte 0x0201
	push	{lr}
	lsls	r0, r0, #24
	lsls	r0, r3, #9
	lsls	r0, r0, #12
	cmp	r5, #0
	lsls	r0, r0, #24
	adds	r0, #0
	lsls	r0, r0, #24
.L_081b3670:
	mov	r7, r8
	adds	r3, r7, r5
	strh	r2, [r3, #0]
.L_081b3676:
	movs	r3, #1
	add	sl, r3
	movs	r7, #2
	mov	r3, sl
	adds	r2, #1
	add	r8, r7
	cmp	r3, #32
	bne.n	.L_081b363e
	adds	r6, #1
	adds	r4, #1
	adds	r0, #32
	cmp	r6, #20
	bne.n	.L_081b3634
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r1, #0
	strh	r1, [r3, #0]
	strh	r1, [r3, #2]
	strh	r1, [r3, #4]
	strh	r1, [r3, #6]
	strh	r1, [r3, #8]
	strh	r1, [r3, #10]
	ldr	r3, [pc, #24]
	adds	r2, #60
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #6
	b.n	.L_081b36dc
	movs	r0, r0
	.4byte 0x00000509
	.4byte 0x00000680
	.4byte 0x00003737
	.4byte 0x00002727
	.4byte 0x00003f44
	.2byte 0x1120
	.2byte 0x0300
.L_081b36dc:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	ldr	r2, [pc, #44]
	adds	r3, #20
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r1, [r3, #0]
	subs	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r0, #128
	ldr	r3, [pc, #28]
	lsls	r0, r0, #19
	adds	r0, #64
	strh	r3, [r0, #0]
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #28]
	movs	r4, #128
	lsls	r4, r4, #19
	adds	r4, #68
	strh	r3, [r4, #0]
	strh	r2, [r0, #0]
	b.n	.L_081b3728
	.4byte 0x00001010
	.4byte 0x0000ff60
	.4byte 0x000028c8
	.4byte 0x000000f0
	.2byte 0x1878
	.2byte 0x0000
.L_081b3728:
	movs	r3, #128
	ldr	r0, [pc, #60]
	lsls	r3, r3, #19
	adds	r3, #66
	strh	r0, [r4, #0]
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r0, [r3, #0]
	ldr	r7, [sp, #40]
	ldr	r3, [sp, #36]
	movs	r0, #224
	movs	r2, #144
	lsls	r0, r0, #3
	lsls	r2, r2, #4
	adds	r0, r7, r0
	adds	r2, r7, r2
	adds	r3, #140
	str	r0, [sp, #28]
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	str	r1, [r3, #0]
	ldr	r3, [sp, #36]
	movs	r0, #238
	adds	r3, #144
	str	r1, [r3, #0]
	ldr	r3, [sp, #36]
	lsls	r0, r0, #7
	adds	r3, #148
	adds	r0, #140
	str	r1, [r3, #0]
	adds	r3, r7, r0
	b.n	.L_081b376c
	.2byte 0x00a0
	.2byte 0x0000
.L_081b376c:
	str	r1, [r3, #0]
	ldr	r2, [sp, #36]
	ldr	r0, [pc, #92]
	adds	r2, #164
	str	r2, [sp, #16]
	str	r1, [r2, #0]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #52]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #128
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #48]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r4, r0, #0
	adds	r3, #212
	ldr	r1, [pc, #36]
	adds	r2, #120
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r4, r4, r3
	adds	r0, r4, #0
	ldr	r1, [pc, #24]
	b.n	.L_081b37e0
	movs	r0, r0
	.4byte 0x00002f8b
	.4byte 0x00005bf6
	.4byte 0x00000152
	.4byte 0x00000082
	.4byte 0x05000200
	.2byte 0x0000
	.2byte 0x0201
.L_081b37e0:
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #180]
	ldr	r1, [pc, #180]
	ldr	r2, [pc, #184]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #180]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r4, r0, #0
	adds	r3, #212
	ldr	r1, [pc, #168]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #32
	adds	r0, r4, #0
	ldr	r1, [pc, #140]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #128]
	ldr	r1, [pc, #144]
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08038000
	bl	.L_081b336c
	movs	r7, #0
	mov	sl, r7
	ldr	r7, [sp, #36]
.L_081b3836:
	movs	r3, #8
	str	r3, [r7, #0]
	movs	r3, #0
	strb	r3, [r7, #25]
	movs	r3, #255
	strb	r3, [r7, #26]
	movs	r6, #0
	adds	r5, r7, #4
.L_081b3846:
	bl	sub_08014878
	movs	r1, #5
	bl	sub_0800206c
	adds	r6, #1
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r6, #21
	bne.n	.L_081b3846
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r7, #28
	cmp	r1, #5
	bne.n	.L_081b3836
	ldr	r3, [sp, #36]
	movs	r2, #0
	add	r7, sp, #88
	mov	sl, r2
	adds	r4, r7, #0
	mov	r9, r3
.L_081b3872:
	movs	r0, #0
	mov	r8, r0
	movs	r5, #0
.L_081b3878:
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r1, #21
	bl	sub_0800206c
	mov	r1, r8
	str	r0, [r5, r7]
	movs	r6, #0
	ldr	r4, [sp, #8]
	cmp	r1, #0
	beq.n	.L_081b38d4
	ldr	r3, [r7, #0]
	cmp	r0, r3
	bne.n	.L_081b38bc
	movs	r2, #1
	negs	r2, r2
	subs	r5, #4
	add	r8, r2
	b.n	.L_081b38d4
	.4byte 0x02010000
	.4byte 0x06010000
	.4byte 0x84001b30
	.4byte 0x00000083
	.4byte 0x050003e0
	.4byte 0x06016e00
	.2byte 0x0480
	.2byte 0x8400
.L_081b38bc:
	adds	r6, #1
	cmp	r6, r8
	beq.n	.L_081b38d4
	lsls	r3, r6, #2
	ldr	r2, [r5, r4]
	ldr	r3, [r4, r3]
	cmp	r2, r3
	bne.n	.L_081b38bc
	movs	r3, #1
	negs	r3, r3
	subs	r5, #4
	add	r8, r3
.L_081b38d4:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #4
	cmp	r1, #8
	bne.n	.L_081b3878
	movs	r6, #0
	adds	r1, r7, #0
.L_081b38e4:
	adds	r2, r6, #0
	cmp	r6, #5
	ble.n	.L_081b38ec
	movs	r2, #5
.L_081b38ec:
	ldmia	r1!, {r3}
	mov	r0, r9
	adds	r3, #4
	adds	r6, #1
	strb	r2, [r0, r3]
	cmp	r6, #8
	bne.n	.L_081b38e4
	movs	r2, #1
	add	sl, r2
	movs	r1, #28
	mov	r3, sl
	add	r9, r1
	cmp	r3, #5
	bne.n	.L_081b3872
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #7
	movs	r3, #3
	movs	r0, #104
	bl	sub_08138000
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #8
	str	r3, [sp, #52]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #188
	bl	sub_08138000
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r7, sp
	adds	r7, #52
	str	r7, [sp, #12]
	movs	r1, #128
	str	r3, [r7, #4]
	ldr	r0, [sp, #44]
	ldr	r3, [pc, #144]
	lsls	r1, r1, #8
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #132]
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #160
	lsls	r2, r2, #24
	lsls	r0, r0, #19
	ldr	r1, [sp, #28]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #112]
	ldr	r1, [sp, #24]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r5, #128
	lsls	r5, r5, #1
	ldr	r1, [pc, #96]
	ldr	r0, [sp, #24]
	movs	r2, #0
	adds	r3, r5, #0
	bl	sub_081b203c
	movs	r1, #160
	movs	r2, #0
	adds	r3, r5, #0
	lsls	r1, r1, #19
	ldr	r0, [sp, #28]
	bl	sub_081b203c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #228
	bl	sub_080ad2a8
	cmp	r0, #1
	bne.n	.L_081b39ea
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #26
	movs	r3, #4
	movs	r0, #2
	bl	sub_08038010
	adds	r1, r0, #0
	ldr	r0, [sp, #36]
	movs	r2, #153
	lsls	r2, r2, #3
	adds	r3, r0, r2
	str	r1, [r3, #0]
	ldr	r0, [pc, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	b.n	.L_081b39e8
	.4byte 0x00003740
	.4byte 0x03000260
	.4byte 0x06003500
	.4byte 0x84002000
	.4byte 0x05000200
	.2byte 0x0d6c
	.2byte 0x0000
.L_081b39e8:
	b.n	.L_081b3a20
.L_081b39ea:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #26
	movs	r3, #4
	movs	r0, #2
	bl	sub_08038010
	ldr	r3, [sp, #36]
	ldr	r5, [pc, #616]
	movs	r7, #153
	lsls	r7, r7, #3
	adds	r1, r0, #0
	adds	r6, r3, r7
	adds	r0, r5, #0
	str	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
.L_081b3a20:
	ldr	r0, [sp, #40]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #0
	movs	r1, #144
	movs	r5, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	lsls	r5, r5, #4
	ldr	r0, [pc, #564]
	bl	sub_080145a8
	adds	r1, r5, #0
	ldr	r0, [pc, #560]
	bl	sub_080145a8
	ldr	r0, [pc, #556]
	adds	r1, r5, #0
	bl	sub_080145a8
	ldr	r7, [sp, #20]
	movs	r2, #0
	ldr	r3, [r7, #0]
	mov	fp, r2
	cmp	r3, #10
	bne.n	.L_081b3a5a
	b.n	.L_081b3da2
.L_081b3a5a:
	mov	r0, fp
	cmp	r0, #16
	bgt.n	.L_081b3a80
	movs	r6, #128
	lsls	r5, r0, #12
	lsls	r6, r6, #1
	ldr	r1, [pc, #528]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r0, [sp, #24]
	bl	sub_081b203c
	movs	r1, #160
	ldr	r0, [sp, #28]
	lsls	r1, r1, #19
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_081b203c
.L_081b3a80:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	cmp	r3, #3
	beq.n	.L_081b3a8a
	b.n	.L_081b3c12
.L_081b3a8a:
	mov	r0, fp
	movs	r1, #80
	bl	sub_08002064
	cmp	r0, #15
	bgt.n	.L_081b3a9e
	ldr	r0, [pc, #484]
	bl	sub_081b21ec
	b.n	.L_081b3ac8
.L_081b3a9e:
	cmp	r0, #31
	bgt.n	.L_081b3aaa
	ldr	r0, [pc, #476]
	bl	sub_081b21ec
	b.n	.L_081b3ac8
.L_081b3aaa:
	cmp	r0, #47
	bgt.n	.L_081b3ab6
	ldr	r0, [pc, #468]
	bl	sub_081b21ec
	b.n	.L_081b3ac8
.L_081b3ab6:
	cmp	r0, #63
	bgt.n	.L_081b3ac2
	ldr	r0, [pc, #460]
	bl	sub_081b21ec
	b.n	.L_081b3ac8
.L_081b3ac2:
	ldr	r0, [pc, #456]
	bl	sub_081b21ec
.L_081b3ac8:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	cmp	r3, #15
	bgt.n	.L_081b3ad8
	bl	sub_081b2150
	ldr	r7, [sp, #16]
	ldr	r3, [r7, #0]
.L_081b3ad8:
	cmp	r3, #16
	ble.n	.L_081b3b7a
	movs	r3, #7
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_081b3b7a
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	adds	r5, r3, #0
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r1, r3, #0
	mov	r2, fp
	adds	r5, #56
	adds	r1, #48
	cmp	r2, #0
	bge.n	.L_081b3b06
	adds	r2, #7
.L_081b3b06:
	movs	r3, #3
	asrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #380]
	lsls	r3, r3, #10
	adds	r7, r3, r2
	lsls	r5, r5, #16
	movs	r3, #0
	lsls	r1, r1, #16
	mov	r8, r3
	mov	r9, r5
	mov	sl, r1
.L_081b3b22:
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	mov	r1, sl
	mov	r0, r9
	str	r1, [r7, #4]
	str	r0, [r7, #0]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #64
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_081b3b22
.L_081b3b7a:
	movs	r7, #0
	mov	r8, r7
	ldr	r7, [pc, #272]
.L_081b3b80:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	ble.n	.L_081b3c04
	ldr	r3, [r7, #0]
	ldr	r1, [pc, #264]
	subs	r0, #1
	str	r0, [r7, #24]
	cmp	r3, r1
	bhi.n	.L_081b3bd6
	ldr	r6, [r7, #4]
	ldr	r2, [pc, #256]
	cmp	r6, r2
	bgt.n	.L_081b3bd6
	cmp	r6, #0
	blt.n	.L_081b3bd6
	movs	r1, #12
	asrs	r5, r3, #16
	bl	sub_08002054
	adds	r0, #1
	ldr	r1, [pc, #240]
	lsls	r4, r0, #1
	mov	r3, r8
	movs	r2, #1
	ands	r2, r3
	asrs	r6, r6, #16
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	subs	r5, r5, r0
	ldr	r3, [sp, #48]
	subs	r6, r6, r0
	str	r4, [sp, #0]
	ldr	r0, [sp, #12]
	str	r4, [sp, #4]
	lsls	r2, r2, #2
	ldr	r4, [r2, r0]
	adds	r1, r3, r1
	ldr	r0, [sp, #44]
	adds	r3, r6, #0
	adds	r2, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x683b
.L_081b3bd6:
	ldr	r2, [r7, #12]
	ldr	r1, [r7, #16]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, r3, r1
	str	r3, [r7, #4]
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_081b3bf0
	adds	r3, #63
.L_081b3bf0:
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_081b3c00
	adds	r3, #63
.L_081b3c00:
	asrs	r3, r3, #6
	str	r3, [r7, #16]
.L_081b3c04:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_081b3b80
.L_081b3c12:
	ldr	r7, [sp, #20]
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_081b3c20
	cmp	r3, #2
	beq.n	.L_081b3c20
	b.n	.L_081b3d80
.L_081b3c20:
	movs	r0, #0
	add	r5, sp, #60
	mov	r8, r0
	movs	r2, #0
	adds	r3, r5, #0
.L_081b3c2a:
	movs	r1, #1
	add	r8, r1
	mov	r7, r8
	stmia	r3!, {r2}
	cmp	r7, #7
	bne.n	.L_081b3c2a
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_081b3ca0
	movs	r2, #1
	str	r2, [r5, #12]
	ldr	r1, [sp, #32]
	ldr	r3, [r1, #0]
	cmp	r3, #1
	ble.n	.L_081b3c50
	str	r2, [r5, #16]
	str	r2, [r5, #8]
	ldr	r3, [r1, #0]
.L_081b3c50:
	cmp	r3, #2
	ble.n	.L_081b3c5c
	str	r2, [r5, #20]
	str	r2, [r5, #4]
	ldr	r7, [sp, #32]
	ldr	r3, [r7, #0]
.L_081b3c5c:
	cmp	r3, #3
	ble.n	.L_081b3cc4
	str	r2, [r5, #24]
	str	r2, [r5, #0]
	b.n	.L_081b3cc4
	movs	r0, r0
	.4byte 0x00000d6b
	.4byte 0x081b243d
	.4byte 0x081b3295
	.4byte 0x081b20a1
	.4byte 0x05000200
	.4byte 0x00000154
	.4byte 0x00000156
	.4byte 0x00000178
	.4byte 0x00000163
	.4byte 0x00000152
	.4byte 0x02010000
	.4byte 0x00ffffff
	.4byte 0x007fffff
	.2byte 0x4888
	.2byte 0x081b
.L_081b3ca0:
	mov	r3, fp
	mov	r0, r8
	ands	r3, r0
	cmp	r3, #3
	bgt.n	.L_081b3cc4
	ldr	r2, [sp, #36]
	movs	r1, #0
	mov	r8, r1
	adds	r0, r5, #0
	adds	r2, #172
.L_081b3cb4:
	ldmia	r2!, {r3}
	str	r3, [r1, r0]
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r1, #4
	cmp	r7, #7
	bne.n	.L_081b3cb4
.L_081b3cc4:
	movs	r0, #0
	mov	r8, r0
.L_081b3cc8:
	movs	r2, #1
	mov	r1, r8
	eors	r2, r1
	negs	r3, r2
	orrs	r3, r2
	lsrs	r6, r3, #31
	movs	r3, #65
	subs	r6, r3, r6
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_081b3cee
	mov	r3, r8
	adds	r3, #19
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_081b22b8
.L_081b3cee:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq.n	.L_081b3d04
	mov	r3, r8
	adds	r3, #35
	movs	r0, #28
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_081b22b8
.L_081b3d04:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L_081b3d1a
	mov	r3, r8
	adds	r3, #51
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_081b22b8
.L_081b3d1a:
	ldr	r3, [r5, #16]
	cmp	r3, #0
	beq.n	.L_081b3d30
	mov	r3, r8
	adds	r3, #67
	movs	r0, #28
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_081b22b8
.L_081b3d30:
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_081b3d46
	mov	r3, r8
	adds	r3, #83
	movs	r0, #20
	adds	r1, r3, #0
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_081b22b8
.L_081b3d46:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_081b3d5e
	mov	r1, r8
	mov	r3, r8
	adds	r1, #5
	adds	r3, #91
	movs	r0, #28
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_081b22b8
.L_081b3d5e:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_081b3d76
	mov	r1, r8
	mov	r3, r8
	adds	r1, #97
	adds	r3, #11
	movs	r0, #28
	movs	r2, #200
	str	r6, [sp, #0]
	bl	.L_081b22b8
.L_081b3d76:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_081b3cc8
.L_081b3d80:
	ldr	r7, [sp, #40]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r3, r7, r0
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #20]
	movs	r1, #1
	ldr	r3, [r2, #0]
	add	fp, r1
	cmp	r3, #10
	beq.n	.L_081b3da2
	b.n	.L_081b3a5a
.L_081b3da2:
	movs	r3, #0
	movs	r6, #128
	mov	r8, r3
	lsls	r6, r6, #1
.L_081b3daa:
	mov	r7, r8
	movs	r5, #128
	lsls	r3, r7, #12
	lsls	r5, r5, #9
	subs	r5, r5, r3
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #108]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_081b203c
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r0, [sp, #28]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_081b203c
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #17
	bne.n	.L_081b3daa
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #60]
	bl	sub_08014644
	ldr	r0, [pc, #56]
	bl	sub_08014644
	ldr	r0, [pc, #56]
	bl	sub_08014644
	movs	r0, #180
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	movs	r0, #100
	bl	sub_0801314c
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x05000200
	.4byte 0x081b20a1
	.4byte 0x081b243d
	.4byte 0x081b3295
