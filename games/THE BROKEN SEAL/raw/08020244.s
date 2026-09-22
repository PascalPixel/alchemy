.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080048b0, 0x080048b0
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016498, 0x08016498
	.set sub_0801ccc0, 0x0801ccc0
	.set sub_0801e41c, 0x0801e41c
	.set sub_0801e74c, 0x0801e74c
	.set sub_0801e858, 0x0801e858
	.set sub_0801fd84, 0x0801fd84
	.set sub_0801fd98, 0x0801fd98
	.set sub_0801fda8, 0x0801fda8
	.set sub_0801fe2c, 0x0801fe2c
	.set sub_0801ff14, 0x0801ff14
	.set sub_0801ffd8, 0x0801ffd8
	.set sub_08020088, 0x08020088
	.set sub_08020150, 0x08020150
	.set sub_08020198, 0x08020198
	.set sub_08021620, 0x08021620
	.set sub_080216b4, 0x080216b4
	.set sub_08077300, 0x08077300
	.set sub_080f9010, 0x080f9010
	.global Overlay_08020244
Overlay_08020244:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r1
	movs	r1, #167
	mov	r8, r0
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #40
	bl	sub_080048b0
	ldr	r3, [pc, #792]
	movs	r1, #0
	movs	r2, #1
	ldr	r7, [r3, #0]
	str	r1, [sp, #32]
	str	r1, [sp, #28]
	str	r1, [sp, #24]
	str	r2, [sp, #12]
	subs	r3, #144
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	str	r3, [sp, #8]
	bl	sub_08077300
	mov	r3, r8
	str	r0, [sp, #4]
	cmp	r3, #0
	bge.n	.L_0802028a
	movs	r1, #0
	mov	r8, r1
.L_0802028a:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_080202d2
	mov	r3, r8
	ldr	r1, [pc, #748]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	b.n	.L_080202bc
.L_0802029e:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_080202ac
	movs	r1, #0
	mov	r8, r1
.L_080202ac:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L_0802039a
	mov	r3, r8
	ldr	r1, [pc, #712]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L_080202bc:
	cmp	r3, #0
	beq.n	.L_0802029e
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0802029e
	b.n	.L_0802039a
.L_080202d2:
	mov	r2, r9
	cmp	r2, #4
	bne.n	.L_0802031a
	mov	r3, r8
	ldr	r1, [pc, #676]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	b.n	.L_08020304
.L_080202e6:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_080202f4
	movs	r1, #0
	mov	r8, r1
.L_080202f4:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L_0802039a
	mov	r3, r8
	ldr	r1, [pc, #640]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L_08020304:
	cmp	r3, #0
	beq.n	.L_080202e6
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080202e6
	b.n	.L_0802039a
.L_0802031a:
	mov	r2, r9
	cmp	r2, #5
	bne.n	.L_08020362
	mov	r3, r8
	ldr	r1, [pc, #604]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	b.n	.L_0802034c
.L_0802032e:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_0802033c
	movs	r1, #0
	mov	r8, r1
.L_0802033c:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L_0802039a
	mov	r3, r8
	ldr	r1, [pc, #568]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
.L_0802034c:
	cmp	r3, #0
	beq.n	.L_0802032e
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0802032e
	b.n	.L_0802039a
.L_08020362:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_080203b0
	mov	r3, r8
	ldr	r1, [pc, #532]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_0802039a
	adds	r3, r2, r7
	adds	r2, r3, r1
.L_0802037c:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r2, #64
	cmp	r3, #3
	bne.n	.L_0802038e
	movs	r3, #0
	adds	r2, r7, r1
	mov	r8, r3
.L_0802038e:
	adds	r6, #1
	cmp	r6, #2
	bgt.n	.L_0802039a
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0802037c
.L_0802039a:
	cmp	r6, #3
	bne.n	.L_080203b0
	movs	r5, #2
	negs	r5, r5
	b.n	.L_08020794
.L_080203a4:
	movs	r0, #113
	movs	r5, #1
	bl	sub_080f9010
	negs	r5, r5
	b.n	.L_0802074a
.L_080203b0:
	add	r0, sp, #36
	movs	r3, #0
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #456]
	ldr	r2, [pc, #460]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_0801fd84
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #2
	movs	r2, #28
	movs	r3, #7
	movs	r0, #1
	bl	sub_080162d4
	movs	r1, #130
	lsls	r1, r1, #5
	adds	r1, r7, r1
	ldr	r3, [pc, #420]
	movs	r2, #0
	str	r1, [sp, #20]
	mov	sl, r0
	mov	fp, r2
	movs	r6, #2
	adds	r5, r7, r3
.L_080203e8:
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080203f2
	ldr	r0, [pc, #412]
	b.n	.L_0802041a
.L_080203f2:
	ldrh	r3, [r5, #26]
	ldr	r1, [sp, #4]
	cmp	r3, r1
	bcs.n	.L_080203fe
	ldr	r0, [pc, #404]
	b.n	.L_0802041a
.L_080203fe:
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #28]
	cmp	r2, r3
	beq.n	.L_0802040a
	ldr	r0, [pc, #396]
	b.n	.L_0802041a
.L_0802040a:
	mov	r2, r9
	cmp	r2, #5
	bne.n	.L_08020426
	movs	r3, #21
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_08020426
	ldr	r0, [pc, #380]
.L_0802041a:
	mov	r1, sl
	movs	r2, #10
	mov	r3, fp
	bl	sub_0801e74c
	b.n	.L_0802044e
.L_08020426:
	ldr	r0, [sp, #20]
	mov	r1, sl
	adds	r0, #16
	movs	r2, #12
	mov	r3, fp
	bl	sub_0801e858
	ldr	r3, [pc, #356]
	ldrh	r0, [r5, #2]
	mov	r1, sl
	adds	r0, r0, r3
	movs	r2, #62
	mov	r3, fp
	bl	sub_0801e74c
	ldr	r1, [pc, #344]
	ldr	r3, [sp, #8]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
.L_0802044e:
	ldr	r3, [sp, #20]
	movs	r2, #16
	adds	r3, #64
	subs	r6, #1
	add	fp, r2
	adds	r5, #64
	str	r3, [sp, #20]
	cmp	r6, #0
	bge.n	.L_080203e8
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #2
	movs	r3, #27
	bl	sub_0801e41c
	movs	r3, #4
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #4
	movs	r3, #27
	bl	sub_0801e41c
	movs	r3, #24
	mov	r1, sl
	negs	r3, r3
	mov	r0, r9
	movs	r2, #72
	bl	sub_08021620
	movs	r1, #2
	str	r0, [sp, #16]
	mov	fp, r1
.L_08020494:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_0802049c
	b.n	.L_0802061e
.L_0802049c:
	movs	r3, #0
	mov	r1, r8
	ldr	r2, [pc, #220]
	str	r3, [sp, #12]
	lsls	r5, r1, #6
	adds	r3, r5, r2
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	bne.n	.L_080204b0
	b.n	.L_080205ac
.L_080204b0:
	ldr	r1, [pc, #240]
	adds	r3, r5, r1
	ldrb	r0, [r7, r3]
	adds	r3, r7, r3
	ldrb	r1, [r3, #1]
	bl	sub_0801ccc0
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_080204d6
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r0, #1
	movs	r1, #10
	movs	r2, #14
	movs	r3, #9
	bl	sub_080162d4
	str	r0, [sp, #32]
.L_080204d6:
	movs	r1, #130
	adds	r3, r7, r5
	lsls	r1, r1, #5
	adds	r6, r3, r1
	ldr	r0, [sp, #32]
	adds	r1, r6, #0
	bl	sub_08020198
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_08020504
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r0, #16
	movs	r1, #10
	movs	r2, #13
	movs	r3, #3
	bl	sub_080162d4
	str	r0, [sp, #28]
.L_08020504:
	bl	sub_0801ff14
	movs	r1, #0
	ldr	r0, [sp, #28]
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_0801fe2c
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #140]
	adds	r3, r5, r1
	adds	r1, #1
	ldrsb	r2, [r7, r3]
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	adds	r1, #1
	adds	r2, r2, r3
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	adds	r1, #1
	adds	r2, r2, r3
	adds	r3, r5, r1
	ldrsb	r3, [r7, r3]
	cmn	r2, r3
	beq.n	.L_0802056a
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_08020552
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r0, #16
	movs	r1, #14
	movs	r2, #13
	movs	r3, #5
	bl	sub_080162d4
	str	r0, [sp, #24]
.L_08020552:
	ldr	r0, [sp, #24]
	adds	r1, r6, #0
	bl	sub_08020150
	bl	sub_08020088
	movs	r1, #0
	ldr	r0, [sp, #24]
	movs	r2, #0
	bl	sub_0801ffd8
	b.n	.L_080205e6
.L_0802056a:
	bl	sub_08020088
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	sub_08016418
	movs	r1, #0
	str	r1, [sp, #24]
	b.n	.L_080205e6
	.4byte 0x03001f1c
	.4byte 0x0000105c
	.4byte 0x040000d4
	.4byte 0x8500029c
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x0000099b
	.4byte 0x00000ea3
	.4byte 0x00001074
	.2byte 0x1068
	.2byte 0x0000
.L_080205ac:
	ldr	r2, [pc, #504]
	ldr	r1, [pc, #508]
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #504]
	adds	r2, r2, r3
	ldrb	r1, [r2, #0]
	bl	sub_0801ccc0
	bl	sub_08020088
	bl	sub_0801ff14
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	sub_08016418
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	sub_08016418
	movs	r1, #2
	ldr	r0, [sp, #32]
	bl	sub_08016418
	movs	r1, #0
	str	r1, [sp, #24]
	str	r1, [sp, #28]
	str	r1, [sp, #32]
.L_080205e6:
	mov	r0, sl
	bl	sub_08016498
	mov	r2, fp
	str	r2, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #2
	movs	r3, #27
	bl	sub_0801e41c
	movs	r3, #4
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #4
	movs	r3, #27
	bl	sub_0801e41c
	mov	r3, r8
	lsls	r2, r3, #1
	movs	r3, #1
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r3, #26
	bl	sub_0801fda8
.L_0802061e:
	ldr	r0, [sp, #16]
	bl	sub_080216b4
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #392]
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080206aa
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	str	r1, [sp, #12]
	b.n	.L_08020696
.L_08020642:
	mov	r3, r8
	ldr	r1, [pc, #368]
	lsls	r2, r3, #6
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_08020696
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_08020666
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08020696
.L_08020666:
	mov	r3, r9
	cmp	r3, #4
	bne.n	.L_0802067c
	ldr	r1, [pc, #332]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08020696
.L_0802067c:
	mov	r3, r9
	cmp	r3, #5
	beq.n	.L_08020684
	b.n	.L_08020494
.L_08020684:
	ldr	r1, [pc, #308]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08020696
	b.n	.L_08020494
.L_08020696:
	mov	r0, r8
	adds	r0, #2
	movs	r1, #3
	bl	sub_080022fc
	mov	r2, r9
	mov	r8, r0
	cmp	r2, #0
	bne.n	.L_08020642
	b.n	.L_08020494
.L_080206aa:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08020728
	movs	r0, #111
	bl	sub_080f9010
	movs	r3, #1
	str	r3, [sp, #12]
	b.n	.L_08020714
.L_080206c0:
	mov	r1, r8
	lsls	r2, r1, #6
	ldr	r1, [pc, #240]
	adds	r3, r2, r1
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_08020714
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_080206e4
	adds	r1, #20
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08020714
.L_080206e4:
	mov	r3, r9
	cmp	r3, #4
	bne.n	.L_080206fa
	ldr	r1, [pc, #208]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #2]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08020714
.L_080206fa:
	mov	r3, r9
	cmp	r3, #5
	beq.n	.L_08020702
	b.n	.L_08020494
.L_08020702:
	ldr	r1, [pc, #184]
	adds	r3, r2, r1
	adds	r3, r7, r3
	ldrb	r3, [r3, #1]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08020714
	b.n	.L_08020494
.L_08020714:
	mov	r0, r8
	adds	r0, #4
	movs	r1, #3
	bl	sub_080022fc
	mov	r2, r9
	mov	r8, r0
	cmp	r2, #0
	bne.n	.L_080206c0
	b.n	.L_08020494
.L_08020728:
	ldr	r2, [pc, #148]
	ldr	r3, [r2, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08020736
	b.n	.L_080203a4
.L_08020736:
	ldr	r3, [r2, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08020742
	b.n	.L_08020494
.L_08020742:
	movs	r0, #112
	bl	sub_080f9010
	mov	r5, r8
.L_0802074a:
	bl	sub_08020088
	bl	sub_0801ff14
	movs	r1, #2
	ldr	r0, [sp, #24]
	bl	sub_08016418
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	sub_08016418
	movs	r1, #2
	ldr	r0, [sp, #32]
	bl	sub_08016418
	movs	r1, #2
	mov	r0, sl
	bl	sub_08016418
	bl	sub_0801fd98
	movs	r0, #55
	bl	sub_08002dd8
	ldr	r3, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, r3, r1
	ldrb	r0, [r2, #0]
	ldr	r2, [pc, #40]
	adds	r3, r3, r2
	ldrb	r1, [r3, #0]
	bl	sub_0801ccc0
	movs	r0, #1
	bl	sub_080030f8
.L_08020794:
	adds	r0, r5, #0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x00000206
	.4byte 0x03001b04
	.4byte 0x0000105c
	.4byte 0x00001070
	.4byte 0x03001c94
