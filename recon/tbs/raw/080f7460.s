.syntax unified
	.thumb
	.set sub_08002f3c, 0x08002f3c
	.set sub_08002f40, 0x08002f40
	.set sub_080040e8, 0x080040e8
	.set sub_080048b0, 0x080048b0
	.set sub_080048f4, 0x080048f4
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080f7620, 0x080f7620
	.set sub_080f7626, 0x080f7626
	.global Unnamed_080f7460
	.global Func_080f7460
	.thumb_func
Unnamed_080f7460:
Func_080f7460:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #120]
	movs	r0, #41
	sub	sp, #116
	bl	sub_080048b0
	movs	r1, #128
	str	r0, [sp, #44]
	lsls	r1, r1, #8
	movs	r0, #40
	bl	sub_080048b0
	ldr	r1, [pc, #100]
	str	r0, [sp, #40]
	movs	r0, #39
	bl	sub_080048f4
	ldr	r1, [pc, #96]
	str	r0, [sp, #36]
	movs	r0, #45
	bl	sub_080048f4
	str	r0, [sp, #32]
	ldr	r0, [pc, #88]
	ldr	r5, [pc, #88]
	bl	sub_08002f3c
	movs	r0, #144
	lsls	r0, r0, #1
	adds	r5, r5, r0
	movs	r3, #255
	strb	r3, [r5, #0]
	ldr	r2, [sp, #32]
	movs	r3, #0
	adds	r2, #162
	strh	r3, [r2, #0]
	ldr	r1, [sp, #32]
	movs	r3, #1
	adds	r1, #152
	str	r1, [sp, #28]
	str	r3, [r1, #0]
	bl	sub_080040e8
	ldr	r5, [pc, #24]
	ldr	r3, [pc, #52]
	movs	r0, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #52]
	ldr	r7, [pc, #16]
	ldr	r4, [pc, #20]
	mov	r8, r0
	movs	r6, #0
.L_080f74d4:
	movs	r2, #0
	mov	sl, r2
	b.n	.L_080f7504
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0000a1a6
	.4byte 0x0000a1a8
	.4byte 0x0000060e
	.4byte 0x0000782c
	.4byte 0x0000061c
	.4byte 0x0000000c
	.4byte 0x0200024c
	.4byte 0x03001d18
	.2byte 0x2800
	.2byte 0x0600
.L_080f7504:
	mov	r3, sl
	subs	r3, #5
	cmp	r3, #19
	bhi.n	.L_080f751c
	cmp	r6, #2
	ble.n	.L_080f751c
	cmp	r6, #13
	bgt.n	.L_080f751c
	mov	r2, r8
	adds	r3, r2, r1
	strh	r7, [r3, #0]
	b.n	.L_080f7534
.L_080f751c:
	mov	r3, sl
	cmp	r3, #29
	ble.n	.L_080f752a
	mov	r2, r8
	adds	r3, r2, r1
	strh	r5, [r3, #0]
	b.n	.L_080f7534
.L_080f752a:
	mov	r3, r8
	adds	r2, r3, r1
	adds	r3, r0, r4
	strh	r3, [r2, #0]
	adds	r0, #1
.L_080f7534:
	movs	r2, #1
	add	sl, r2
	movs	r3, #2
	mov	r2, sl
	add	r8, r3
	cmp	r2, #32
	bne.n	.L_080f7504
	adds	r6, #1
	cmp	r6, #20
	bne.n	.L_080f74d4
	ldr	r0, [pc, #172]
	bl	sub_08002f40
	ldr	r1, [sp, #44]
	bl	sub_08005340
	ldr	r0, [pc, #164]
	bl	sub_08002f40
	ldr	r3, [pc, #164]
	adds	r4, r0, #0
	ldr	r1, [pc, #164]
	ldr	r2, [pc, #164]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #32
	adds	r0, r4, #0
	ldr	r1, [pc, #160]
	bl	sub_08005340
	ldr	r0, [pc, #152]
	movs	r3, #0
	ldr	r7, [pc, #152]
	mov	r8, r3
	movs	r6, #0
	movs	r5, #0
	mov	ip, r0
.L_080f757e:
	movs	r1, #0
	lsls	r3, r5, #6
	mov	r2, ip
	mov	sl, r1
	adds	r4, r3, r2
.L_080f7588:
	mov	r3, sl
	subs	r3, #5
	cmp	r3, #19
	bhi.n	.L_080f7598
	cmp	r6, #2
	ble.n	.L_080f7598
	cmp	r6, #13
	ble.n	.L_080f75aa
.L_080f7598:
	mov	r2, r8
	adds	r1, r2, r7
	ldr	r3, [pc, #96]
	adds	r0, r4, #0
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #32
	add	r8, r3
.L_080f75aa:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r4, #32
	cmp	r1, #30
	bne.n	.L_080f7588
	adds	r6, #1
	adds	r5, #15
	cmp	r6, #20
	bne.n	.L_080f757e
	movs	r1, #192
	ldr	r3, [pc, #80]
	ldr	r0, [pc, #84]
	lsls	r1, r1, #2
	bl	sub_080072f0
	ldr	r3, [pc, #40]
	movs	r2, #0
	movs	r4, #2
	ldr	r5, [pc, #72]
	mov	r8, r2
	movs	r6, #0
	mov	ip, r3
	negs	r4, r4
	movs	r0, #0
	movs	r7, #0
	adds	r2, r0, #0
	mov	sl, r7
	adds	r1, r4, #0
	adds	r2, #148
	cmp	r1, #14
	bls.n	sub_080f7620
	mov	r7, r8
	adds	r3, r7, r5
	mov	r7, ip
	strh	r7, [r3, #0]
	b.n	sub_080f7626
	.4byte 0x000000bf
	.4byte 0x00000076
	.4byte 0x0000003f
	.4byte 0x040000d4
	.4byte 0x05000140
	.4byte 0x84000008
	.4byte 0x02010000
