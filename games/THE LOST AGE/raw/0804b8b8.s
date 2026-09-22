.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_080200e8, 0x080200e8
	.set sub_0804297c, 0x0804297c
	.set sub_080461c8, 0x080461c8
	.set sub_08118098, 0x08118098
	.set sub_081180c0, 0x081180c0
	.set sub_081180e0, 0x081180e0
	.set sub_08118118, 0x08118118
	.set sub_08118138, 0x08118138
	.set sub_08118140, 0x08118140
	.set sub_081c0010, 0x081c0010
	.global Overlay_0804b8b8
Overlay_0804b8b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #28]
	mov	r0, sp
	adds	r0, #36
	str	r0, [sp, #24]
	bl	sub_08118138
	movs	r1, #0
	movs	r2, #1
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	mov	fp, r0
	ldr	r0, [r3, #36]
	str	r1, [sp, #12]
	adds	r3, #228
	ldr	r3, [r3, #0]
	mov	r1, fp
	mov	r9, r0
	str	r3, [sp, #8]
	cmp	r1, #0
	bgt.n	.L_0804b8fa
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0804baa6
.L_0804b8fa:
	ldr	r2, [sp, #28]
	movs	r3, #255
	ands	r2, r3
	lsls	r3, r2, #1
	str	r3, [sp, #4]
	str	r2, [sp, #28]
	adds	r3, #88
	mov	r0, r9
	ldrsh	r0, [r0, r3]
	mov	sl, r0
	movs	r0, #112
	bl	sub_081c0010
	movs	r2, #0
	add	r3, sp, #32
	str	r2, [sp, #0]
	mov	r8, r3
	b.n	.L_0804ba46
.L_0804b91e:
	ldr	r3, [r1, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804b984
	ldr	r0, [sp, #16]
	cmp	r0, #0
	bne.n	.L_0804b984
	movs	r0, #1
	movs	r1, #0
	bl	sub_08118140
	cmp	r0, #0
	bne.n	.L_0804b942
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_0804ba46
.L_0804b942:
	movs	r0, #110
	bl	sub_081c0010
	ldr	r1, [sp, #8]
	movs	r2, #130
	lsls	r2, r2, #1
	adds	r3, r1, r2
	mov	r0, sl
	str	r0, [r3, #0]
	adds	r2, #4
	ldr	r0, [sp, #28]
	ldr	r4, [pc, #344]
	adds	r3, r1, r2
	movs	r2, #134
	lsls	r2, r2, #2
	str	r0, [r3, #0]
	adds	r1, r4, r2
	movs	r0, #0
.L_0804b966:
	ldrb	r3, [r1, #0]
	cmp	r3, sl
	bne.n	.L_0804b970
	strb	r7, [r4, r2]
	b.n	.L_0804b978
.L_0804b970:
	cmp	r3, r7
	bne.n	.L_0804b978
	mov	r3, sl
	strb	r3, [r4, r2]
.L_0804b978:
	adds	r0, #1
	adds	r1, #1
	adds	r2, #1
	cmp	r0, #7
	ble.n	.L_0804b966
	b.n	.L_0804ba78
.L_0804b984:
	ldr	r3, [r1, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804b9ae
	ldr	r0, [sp, #20]
	mov	r1, fp
	add	r0, fp
	subs	r0, #1
	bl	sub_08002064
	str	r0, [sp, #20]
	movs	r0, #112
	bl	sub_081c0010
	ldr	r1, [sp, #20]
	movs	r0, #1
	lsls	r1, r1, #1
	str	r0, [sp, #16]
	str	r1, [sp, #0]
	b.n	.L_0804b9d4
.L_0804b9ae:
	ldr	r3, [r1, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804b9d4
	ldr	r0, [sp, #20]
	mov	r1, fp
	adds	r0, #1
	bl	sub_08002064
	str	r0, [sp, #20]
	movs	r0, #112
	bl	sub_081c0010
	ldr	r3, [sp, #20]
	movs	r2, #1
	lsls	r3, r3, #1
	str	r2, [sp, #16]
	str	r3, [sp, #0]
.L_0804b9d4:
	ldr	r0, [sp, #16]
	cmp	r0, #0
	beq.n	.L_0804ba40
	movs	r1, #0
	str	r1, [sp, #16]
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #4]
	ldrh	r3, [r0, r1]
	adds	r2, #88
	mov	r0, r9
	strh	r3, [r0, r2]
	lsls	r3, r3, #16
	mov	r0, sl
	asrs	r7, r3, #16
	bl	sub_08118098
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_08118098
	ldr	r3, [r5, #12]
	ldr	r4, [r5, #0]
	str	r3, [r0, #12]
	ldr	r6, [r0, #0]
	ldr	r3, [r5, #16]
	str	r3, [r0, #16]
	adds	r0, r6, #0
	ldr	r1, [r4, #8]
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #16]
	bl	sub_080200e8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_0804297c
	bl	sub_08118118
	mov	r1, r8
	movs	r3, #255
	mov	r2, r8
	strh	r7, [r1, #0]
	mov	r0, r8
	strh	r3, [r2, #2]
	movs	r1, #2
	bl	sub_081180e0
	mov	r0, r8
	bl	sub_080461c8
.L_0804ba40:
	movs	r0, #1
	bl	sub_08013560
.L_0804ba46:
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #0]
	ldr	r0, [sp, #4]
	ldrh	r3, [r1, r2]
	adds	r0, #88
	mov	r1, r9
	strh	r3, [r1, r0]
	ldr	r1, [pc, #96]
	lsls	r3, r3, #16
	asrs	r7, r3, #16
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804ba66
	b.n	.L_0804b91e
.L_0804ba66:
	mov	r3, sl
	mov	r2, r9
	strh	r3, [r2, r0]
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #12]
	movs	r0, #113
	bl	sub_081c0010
.L_0804ba78:
	movs	r3, #255
	mov	r1, r8
	strh	r3, [r1, #0]
	mov	r0, r8
	movs	r1, #0
	bl	sub_081180e0
	mov	r0, r8
	bl	sub_080461c8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_0804297c
	bl	sub_08118118
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #12]
.L_0804baa6:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #16]
	movs	r0, #9
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	bl	sub_08014dac
	ldr	r2, [sp, #12]
	adds	r5, r0, #0
	ldr	r2, [r2, #0]
	movs	r0, #1
	adds	r1, r5, #0
	mov	fp, r2
	bl	sub_081180c0
	movs	r3, #0
	mov	sl, r3
	movs	r7, #0
	cmp	sl, r0
	bge.n	.L_0804bb88
	mov	r8, r0
.L_0804baf4:
	ldrh	r0, [r5, #0]
	bl	sub_08016ca4
	movs	r4, #67
	adds	r4, r4, r0
	ldrb	r3, [r4, #0]
	movs	r6, #0
	mov	ip, r4
	cmp	r6, r3
	bge.n	.L_0804bb7a
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r2, r2, r0
	movs	r3, #156
	mov	r9, r2
	lsls	r3, r3, #1
	ldr	r2, [sp, #16]
	adds	r3, r3, r0
	mov	r4, sl
	mov	lr, r3
	str	r5, [sp, #0]
	lsls	r3, r4, #1
	adds	r1, r3, r2
	lsls	r2, r7, #4
	add	r2, fp
.L_0804bb26:
	mov	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0804bb3a
	mov	r4, lr
	ldr	r3, [r4, #0]
	ldr	r4, [pc, #44]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0804bb64
.L_0804bb3a:
	ldr	r4, [sp, #0]
	adds	r7, #1
	ldrh	r3, [r4, #0]
	strh	r3, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #64
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #4]
	movs	r3, #8
	strh	r3, [r2, #6]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #8]
	movs	r3, #192
	lsls	r3, r3, #1
	strh	r3, [r2, #10]
	adds	r2, #16
	b.n	.L_0804bb70
	.4byte 0x00000000
	.2byte 0xff00
	.2byte 0xffff
.L_0804bb64:
	ldr	r4, [sp, #0]
	ldrh	r3, [r4, #0]
	strh	r3, [r1, #0]
	movs	r3, #1
	adds	r1, #2
	add	sl, r3
.L_0804bb70:
	mov	r4, ip
	ldrb	r3, [r4, #0]
	adds	r6, #1
	cmp	r6, r3
	blt.n	.L_0804bb26
.L_0804bb7a:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	adds	r5, #2
	cmp	r3, #0
	bne.n	.L_0804baf4
.L_0804bb88:
	ldr	r4, [sp, #8]
	lsls	r3, r7, #4
	str	r7, [r4, #0]
	add	fp, r3
	ldr	r3, [sp, #12]
	mov	r2, fp
	mov	r0, sl
	str	r2, [r3, #0]
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
