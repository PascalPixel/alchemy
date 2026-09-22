.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144628, 0x08144628
	.set sub_08144aac, 0x08144aac
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_08144044
Overlay_08144044:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #76
	str	r0, [sp, #40]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	movs	r0, #0
	str	r1, [sp, #36]
	movs	r5, #200
	ldr	r3, [r3, #96]
	lsls	r5, r5, #4
	str	r3, [sp, #32]
	bl	sub_081435e0
	ldr	r2, [sp, #36]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #720]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #716]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #708]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2000
	add	r1, sp, #44
	bl	sub_08144aac
	adds	r1, r5, #0
	ldr	r0, [pc, #692]
	bl	sub_080145a8
	ldr	r1, [sp, #36]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	ldr	r3, [pc, #672]
	adds	r1, r5, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #668]
	ldr	r5, [pc, #672]
	bl	sub_080145a8
	movs	r1, #0
	mov	sl, r1
	movs	r6, #255
.L_081440ca:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #15
	add	sl, r2
	lsls	r3, r3, #2
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_081440ca
	movs	r0, #142
	bl	sub_081c0010
	movs	r1, #0
	str	r1, [sp, #28]
	ldr	r2, [sp, #40]
	subs	r1, #64
	ldr	r3, [r2, #20]
	lsls	r3, r3, #5
	cmp	r3, r1
	bne.n	.L_08144114
	b.n	.L_0814431e
.L_08144114:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	ldr	r2, [sp, #28]
	str	r3, [sp, #24]
	cmp	r2, #92
	bne.n	.L_08144128
	movs	r0, #0
	bl	sub_081180e8
.L_08144128:
	ldr	r3, [sp, #36]
	ldr	r2, [sp, #40]
	movs	r1, #225
	lsls	r1, r1, #7
	adds	r6, r3, r1
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08144168
	ldr	r1, [sp, #28]
	movs	r3, #0
	mov	sl, r3
	lsls	r5, r1, #11
.L_08144140:
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r2, r0, #1
	adds	r2, r2, r0
	movs	r3, #192
	lsls	r2, r2, #1
	lsls	r3, r3, #11
	subs	r3, r3, r2
	asrs	r3, r3, #10
	stmia	r6!, {r3}
	movs	r3, #1
	movs	r2, #128
	add	sl, r3
	lsls	r2, r2, #4
	mov	r1, sl
	adds	r5, r5, r2
	cmp	r1, #160
	bne.n	.L_08144140
	b.n	.L_08144190
.L_08144168:
	ldr	r3, [sp, #28]
	movs	r2, #0
	mov	sl, r2
	lsls	r5, r3, #11
.L_08144170:
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	movs	r2, #1
	asrs	r3, r3, #10
	movs	r1, #128
	add	sl, r2
	stmia	r6!, {r3}
	lsls	r1, r1, #4
	mov	r3, sl
	adds	r5, r5, r1
	cmp	r3, #160
	bne.n	.L_08144170
.L_08144190:
	ldr	r2, [sp, #40]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	fp, r1
	cmp	r3, #0
	bne.n	.L_0814419e
	b.n	.L_081442f2
.L_0814419e:
	ldr	r3, [sp, #24]
	movs	r2, #36
	adds	r3, #12
	str	r3, [sp, #16]
	movs	r3, #0
	movs	r1, #52
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	add	r1, sp
	mov	r9, r1
.L_081441b2:
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #40]
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	bl	sub_08014de4
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #16]
	bl	sub_080156e8
	ldr	r3, [r5, #8]
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r1, #4]
	mov	r0, r9
	ldr	r3, [r5, #16]
	str	r3, [r1, #8]
	bl	sub_08015128
	ldr	r3, [sp, #28]
	mov	r2, fp
	lsls	r5, r2, #5
	cmp	r3, r5
	ble.n	.L_081442d4
	lsls	r0, r3, #9
	bl	sub_08015024
	ldr	r1, [sp, #28]
	adds	r3, r5, #0
	adds	r3, #32
	cmp	r1, r3
	bne.n	.L_0814420e
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #40]
	ldrsh	r0, [r2, r1]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, fp
	bl	sub_0814cd48
.L_0814420e:
	mov	r3, fp
	movs	r2, #0
	lsls	r3, r3, #3
	mov	sl, r2
	str	r3, [sp, #20]
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #324]
	add	r1, sp, #64
	mov	r8, r1
	adds	r6, r2, r3
.L_08144222:
	ldr	r3, [sp, #20]
	ldr	r1, [sp, #28]
	add	r3, sl
	lsls	r3, r3, #2
	cmp	r1, r3
	ble.n	.L_081442c8
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	adds	r0, r0, r2
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #276]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1207
	cmp	r7, #0
	beq.n	.L_081442c8
	mov	r1, r8
	adds	r0, r6, #0
	bl	sub_0815e1ec
	mov	r2, r8
	ldr	r5, [r2, #0]
	movs	r1, #3
	asrs	r5, r5, #1
	str	r5, [r2, #0]
	mov	r0, sl
	bl	sub_08002064
	ldr	r3, [sp, #36]
	lsls	r1, r0, #3
	adds	r1, r1, r0
	lsls	r1, r1, #6
	movs	r2, #224
	adds	r1, r3, r1
	lsls	r2, r2, #3
	adds	r1, r1, r2
	mov	r2, r8
	ldr	r3, [r2, #4]
	subs	r5, #12
	movs	r2, #24
	subs	r3, #12
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	adds	r2, r5, #0
	ldr	r0, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6835
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_08002054
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_08002054
	ldr	r3, [r6, #24]
	subs	r5, r5, r0
	adds	r3, #1
	str	r5, [r6, #8]
	str	r3, [r6, #24]
.L_081442c8:
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r6, #28
	cmp	r1, #8
	bne.n	.L_08144222
.L_081442d4:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r3, r3, r1
	adds	r2, #2
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	ldr	r1, [sp, #40]
	movs	r2, #1
	ldr	r3, [r1, #20]
	add	fp, r2
	cmp	fp, r3
	beq.n	.L_081442f2
	b.n	.L_081441b2
.L_081442f2:
	bl	sub_081434f8
	movs	r1, #240
	ldr	r3, [sp, #36]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r3, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #28]
	ldr	r1, [sp, #40]
	adds	r2, #1
	str	r2, [sp, #28]
	ldr	r3, [r1, #20]
	lsls	r3, r3, #5
	adds	r3, #64
	cmp	r2, r3
	beq.n	.L_0814431e
	b.n	.L_08144114
.L_0814431e:
	ldr	r0, [pc, #60]
	bl	sub_08014644
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0000016d
	.4byte 0x0000017f
	.4byte 0x03000730
	.4byte 0x08152475
	.4byte 0x04040404
	.4byte 0x08143001
	.4byte 0x02010000
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r1, [r5, #92]
	sub	sp, #68
	str	r1, [sp, #32]
	mov	r9, r0
	ldr	r2, [r5, #96]
	movs	r0, #0
	str	r2, [sp, #28]
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [sp, #32]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r3, r2
	ldr	r0, [pc, #48]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r1, #31
	movs	r0, #188
	str	r3, [sp, #36]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r1, sp
	movs	r5, #200
	adds	r1, #36
	lsls	r5, r5, #4
	b.n	.L_081443d4
	movs	r0, r0
	.4byte 0x00000100
	.2byte 0x0173
	.2byte 0x0000
.L_081443d4:
	str	r1, [sp, #16]
	ldr	r0, [pc, #528]
	str	r3, [r1, #4]
	adds	r1, r5, #0
	bl	sub_080145a8
	ldr	r3, [sp, #32]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r3, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [sp, #32]
	adds	r1, #4
	adds	r2, r3, r1
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #500]
	adds	r1, r5, #0
	bl	sub_080145a8
	mov	r2, r9
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08144418
	ldr	r3, [pc, #488]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
	movs	r3, #112
	negs	r3, r3
	str	r3, [sp, #20]
	b.n	.L_0814441c
.L_08144418:
	movs	r1, #0
	str	r1, [sp, #20]
.L_0814441c:
	mov	r1, r9
	ldr	r3, [r1, #20]
	movs	r2, #0
	mov	r8, r2
	lsls	r3, r3, #4
	subs	r2, #48
	cmp	r3, r2
	bne.n	.L_0814442e
	b.n	.L_081445be
.L_0814442e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	movs	r1, #225
	mov	sl, r3
	ldr	r3, [sp, #32]
	lsls	r1, r1, #7
	mov	r2, r9
	adds	r6, r3, r1
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08144470
	movs	r3, #128
	mov	r1, r8
	movs	r7, #0
	lsls	r3, r3, #12
	lsls	r5, r1, #10
.L_08144450:
	adds	r0, r5, #0
	str	r3, [sp, #8]
	bl	sub_08002096
	ldr	r3, [sp, #8]
	lsls	r0, r0, #3
	subs	r0, r3, r0
	movs	r2, #128
	asrs	r0, r0, #10
	lsls	r2, r2, #3
	adds	r7, #1
	stmia	r6!, {r0}
	adds	r5, r5, r2
	cmp	r7, #160
	bne.n	.L_08144450
	b.n	.L_08144492
.L_08144470:
	mov	r3, r8
	movs	r7, #0
	lsls	r5, r3, #10
.L_08144476:
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r1, [pc, #372]
	lsls	r0, r0, #3
	asrs	r0, r0, #10
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #3
	adds	r7, #1
	stmia	r6!, {r0}
	adds	r5, r5, r2
	cmp	r7, #160
	bne.n	.L_08144476
.L_08144492:
	bl	sub_08014de4
	mov	r1, sl
	adds	r1, #12
	mov	r0, sl
	bl	sub_080156e8
	movs	r3, #0
	str	r3, [sp, #24]
	mov	r1, r9
	ldr	r3, [r1, #20]
	cmp	r3, #0
	beq.n	.L_08144598
	movs	r3, #36
	movs	r2, #56
	str	r3, [sp, #12]
	add	r2, sp
	mov	sl, r2
.L_081444b6:
	ldr	r1, [sp, #12]
	mov	r3, r9
	ldrsh	r0, [r1, r3]
	bl	sub_08118098
	ldr	r1, [sp, #24]
	ldr	r5, [r0, #0]
	lsls	r2, r1, #4
	cmp	r8, r2
	ble.n	.L_08144582
	adds	r3, r2, #0
	adds	r3, #60
	cmp	r8, r3
	bge.n	.L_08144582
	subs	r3, #28
	cmp	r8, r3
	bne.n	.L_081444ec
	ldr	r2, [sp, #12]
	mov	r1, r9
	ldrsh	r0, [r2, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	bl	sub_0814cd48
.L_081444ec:
	ldr	r3, [r5, #8]
	mov	r2, sl
	str	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r2, #4]
	mov	r0, sl
	ldr	r3, [r5, #16]
	add	r5, sp, #44
	str	r3, [r2, #8]
	adds	r1, r5, #0
	bl	sub_0815e1ec
	mov	r3, r8
	mov	fp, r5
	movs	r7, #0
	lsls	r5, r3, #9
.L_0814450e:
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r1, fp
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #20]
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r0, r5, #0
	adds	r6, r3, r2
	bl	sub_08002090
	mov	r1, fp
	ldr	r3, [r1, #4]
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	adds	r0, r3, r0
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_0814453a
	adds	r3, #15
.L_0814453a:
	asrs	r2, r3, #4
	ldr	r1, [sp, #16]
	movs	r3, #1
	ands	r3, r2
	lsls	r3, r3, #2
	adds	r4, r3, r1
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L_0814454e
	adds	r1, #3
.L_0814454e:
	lsls	r3, r2, #2
	asrs	r1, r1, #2
	ldr	r2, [sp, #32]
	subs	r1, r1, r3
	lsls	r1, r1, #10
	movs	r3, #224
	adds	r1, r2, r1
	lsls	r3, r3, #3
	adds	r1, r1, r3
	adds	r3, r0, #0
	movs	r0, #32
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	adds	r2, r6, #0
	subs	r2, #16
	subs	r3, #16
	ldr	r4, [r4, #0]
	ldr	r0, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #7
	adds	r7, #1
	adds	r5, r5, r1
	cmp	r7, #4
	bne.n	.L_0814450e
.L_08144582:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #24]
	adds	r2, #2
	adds	r3, #1
	str	r2, [sp, #12]
	str	r3, [sp, #24]
	mov	r1, r9
	ldr	r3, [r1, #20]
	ldr	r2, [sp, #24]
	cmp	r2, r3
	bne.n	.L_081444b6
.L_08144598:
	ldr	r3, [sp, #32]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r3, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	mov	r1, r9
	ldr	r3, [r1, #20]
	movs	r2, #1
	lsls	r3, r3, #4
	add	r8, r2
	adds	r3, #48
	cmp	r8, r3
	beq.n	.L_081445be
	b.n	.L_0814442e
.L_081445be:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08152475
	.4byte 0x08143001
	.4byte 0xffff9800
	.2byte 0x9000
	.2byte 0xffff
	.2byte 0xb500
	movs	r1, #0
	bl	sub_08144628
	pop	{pc}
