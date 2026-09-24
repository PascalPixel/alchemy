.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022f4, 0x080022f4
	.set sub_08004458, 0x08004458
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080770c8, 0x080770c8
	.set sub_08077198, 0x08077198
	.set sub_080771a0, 0x080771a0
	.set sub_080c2470, 0x080c2470
	.global BattleEnemy_RecordDefeat
	.global Func_080c24f0
	.thumb_func
BattleEnemy_RecordDefeat:
Func_080c24f0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r1
	adds	r5, r0, #0
	bl	sub_08077008
	ldr	r3, [pc, #524]
	mov	r8, r0
	movs	r1, #166
	ldr	r0, [r3, #0]
	lsls	r1, r1, #3
	adds	r7, r0, r1
	cmp	r5, #7
	bhi.n	.L_080c251c
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080c2704
.L_080c251c:
	ldr	r3, [pc, #504]
	add	r3, r8
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080c252c
	movs	r0, #2
	negs	r0, r0
	b.n	.L_080c2704
.L_080c252c:
	movs	r1, #148
	lsls	r1, r1, #1
	add	r1, r8
	ldrh	r2, [r0, #16]
	ldrb	r3, [r1, #0]
	movs	r6, #0
	movs	r5, #0
	cmp	r2, r3
	beq.n	.L_080c2554
	adds	r4, r1, #0
	adds	r1, r0, #0
	adds	r1, #16
.L_080c2544:
	adds	r5, #1
	cmp	r5, #5
	bgt.n	.L_080c2554
	adds	r1, #2
	ldrh	r2, [r1, #0]
	ldrb	r3, [r4, #0]
	cmp	r2, r3
	bne.n	.L_080c2544
.L_080c2554:
	cmp	r5, #6
	beq.n	.L_080c255a
	adds	r6, r5, #0
.L_080c255a:
	ldrh	r3, [r0, #62]
	cmp	r3, #2
	beq.n	.L_080c2572
	ldrh	r3, [r0, #60]
	cmp	r6, r3
	bge.n	.L_080c2568
	strh	r6, [r0, #60]
.L_080c2568:
	ldr	r3, [r7, #8]
	cmp	r3, #0
	beq.n	.L_080c2574
	movs	r3, #1
	strh	r3, [r0, #62]
.L_080c2572:
	ldr	r3, [r7, #8]
.L_080c2574:
	adds	r3, #1
	str	r3, [r7, #8]
	ldr	r0, [pc, #416]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080c2584
	b.n	.L_080c2702
.L_080c2584:
	movs	r5, #148
	lsls	r5, r5, #1
	add	r5, r8
	ldrb	r0, [r5, #0]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r0, r0, r2
	bl	sub_080770c8
	ldrb	r0, [r5, #0]
	bl	sub_08077198
	mov	r3, fp
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080c2646
	movs	r6, #76
	add	r6, sl
	ldrh	r3, [r6, #0]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_080c25f8
	movs	r6, #0
	movs	r5, #0
	b.n	.L_080c25c8
.L_080c25b6:
	bl	sub_08004458
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r3, r6, r3
	adds	r6, r3, #1
	adds	r5, #1
.L_080c25c8:
	mov	r1, r8
	ldrb	r0, [r1, #15]
	movs	r1, #10
	bl	sub_080022f4
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, #1
	cmp	r5, r0
	blt.n	.L_080c25b6
	mov	r2, r9
	ldrh	r5, [r2, #0]
	lsls	r0, r5, #1
	adds	r0, r0, r5
	movs	r1, #10
	bl	sub_080022ec
	cmp	r6, r0
	bge.n	.L_080c25f0
	adds	r6, r0, #0
.L_080c25f0:
	ldr	r3, [r7, #0]
	adds	r2, r6, r5
	adds	r3, r3, r2
	str	r3, [r7, #0]
.L_080c25f8:
	movs	r3, #82
	add	r3, sl
	mov	r9, r3
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080c265e
	movs	r6, #0
	movs	r5, #0
	b.n	.L_080c2618
.L_080c260a:
	bl	sub_08004458
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r6, r0
	adds	r6, r0, #1
	adds	r5, #1
.L_080c2618:
	mov	r1, r8
	ldrb	r0, [r1, #15]
	movs	r1, #10
	bl	sub_080022f4
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, #1
	cmp	r5, r0
	blt.n	.L_080c260a
	mov	r2, r9
	ldrh	r5, [r2, #0]
	lsls	r0, r5, #1
	adds	r0, r0, r5
	movs	r1, #10
	bl	sub_080022ec
	cmp	r6, r0
	bge.n	.L_080c2640
	adds	r6, r0, #0
.L_080c2640:
	ldr	r3, [r7, #4]
	adds	r2, r6, r5
	b.n	.L_080c265a
.L_080c2646:
	mov	r3, sl
	adds	r3, #76
	ldrh	r2, [r3, #0]
	ldr	r3, [r7, #0]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	mov	r3, sl
	adds	r3, #82
	ldrh	r2, [r3, #0]
	ldr	r3, [r7, #4]
.L_080c265a:
	adds	r3, r3, r2
	str	r3, [r7, #4]
.L_080c265e:
	movs	r3, #78
	add	r3, sl
	movs	r6, #0
	ldrsh	r2, [r3, r6]
	mov	r9, r3
	cmp	r2, #0
	beq.n	.L_080c2702
	mov	r0, sl
	adds	r0, #80
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_080c2702
	ldrh	r3, [r7, #12]
	movs	r5, #0
	cmp	r3, r2
	beq.n	.L_080c2698
	adds	r1, r7, #0
	mov	r4, r9
	adds	r1, #12
.L_080c2686:
	adds	r5, #1
	cmp	r5, #3
	bgt.n	.L_080c2698
	adds	r1, #2
	ldrh	r2, [r1, #0]
	movs	r6, #0
	ldrsh	r3, [r4, r6]
	cmp	r2, r3
	bne.n	.L_080c2686
.L_080c2698:
	cmp	r5, #4
	bne.n	.L_080c2702
	mov	r2, fp
	movs	r1, #0
	ldrsh	r0, [r0, r1]
	cmp	r2, #0
	beq.n	.L_080c26a8
	subs	r0, #2
.L_080c26a8:
	cmp	r0, #0
	bge.n	.L_080c26ae
	movs	r0, #0
.L_080c26ae:
	movs	r5, #128
	lsls	r5, r5, #10
	asrs	r5, r0
	bl	sub_080771a0
	ldr	r3, [pc, #100]
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L_080c2702
	movs	r6, #128
	lsls	r6, r6, #23
	movs	r3, #1
	negs	r3, r3
	mov	r8, r6
	adds	r6, r7, #0
	mov	sl, r3
	movs	r5, #0
	adds	r6, #12
.L_080c26d2:
	ldrh	r0, [r6, #0]
	adds	r6, #2
	bl	sub_080c2470
	cmp	r0, r8
	bge.n	.L_080c26e2
	mov	r8, r0
	mov	sl, r5
.L_080c26e2:
	adds	r5, #1
	cmp	r5, #3
	ble.n	.L_080c26d2
	mov	r2, r9
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	bl	sub_080c2470
	cmp	r0, r8
	ble.n	.L_080c2702
	mov	r6, sl
	mov	r1, r9
	lsls	r3, r6, #1
	ldrh	r2, [r1, #0]
	adds	r3, #12
	strh	r2, [r7, r3]
.L_080c2702:
	movs	r0, #0
.L_080c2704:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
	.4byte 0x00000129
	.4byte 0x00000173
	.4byte 0x0000ffff
