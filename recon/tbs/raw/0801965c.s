.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080048b0, 0x080048b0
	.set sub_08007304, 0x08007304
	.set sub_08018038, 0x08018038
	.set sub_08019bac, 0x08019bac
	.global UiText_CopyMessageString
	.global Func_0801965c
	.thumb_func
UiText_CopyMessageString:
Func_0801965c:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #92]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #92]
	adds	r5, r2, #0
	adds	r2, r6, r3
	movs	r3, #0
	adds	r7, r1, #0
	strh	r3, [r2, #0]
	movs	r1, #1
	bl	sub_08018038
	subs	r5, #1
	movs	r0, #0
	cmp	r0, r5
	bcs.n	.L_080196a8
	movs	r2, #235
	lsls	r2, r2, #4
	ldrh	r3, [r6, r2]
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_080196a8
	mov	ip, r5
	adds	r2, r6, r2
	movs	r4, #0
.L_08019690:
	adds	r0, #1
	adds	r4, #2
	cmp	r0, ip
	bcs.n	.L_080196ac
	adds	r2, #2
	ldrh	r3, [r2, #0]
	adds	r1, r4, #0
	strh	r3, [r1, r7]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_08019690
	b.n	.L_080196ae
.L_080196a8:
	movs	r1, #0
	b.n	.L_080196ae
.L_080196ac:
	lsls	r1, r0, #1
.L_080196ae:
	ldr	r3, [pc, #8]
	strh	r3, [r1, r7]
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000000
	.4byte 0x03001e8c
	.2byte 0x12b2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #208]
	adds	r3, #200
	adds	r7, r2, #0
	ldr	r2, [r3, #0]
	mov	sl, r2
	mov	r8, r3
	mov	r3, sl
	sub	sp, #12
	mov	r9, r0
	adds	r6, r1, #0
	cmp	r3, #0
	bne.n	.L_08019706
	ldr	r5, [pc, #188]
	movs	r0, #50
	adds	r1, r5, #0
	bl	sub_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #172]
	ldr	r0, [pc, #176]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	ldr	r3, [r2, #0]
.L_08019706:
	mov	r5, sp
	mov	r1, r9
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_08019bac
	ldr	r3, [pc, #156]
	mov	r9, r3
	b.n	.L_08019770
.L_08019718:
	cmp	r0, #14
	beq.n	.L_08019730
	cmp	r0, #14
	bhi.n	.L_0801972a
	cmp	r0, #12
	bhi.n	.L_08019766
	cmp	r0, #8
	bcc.n	.L_08019766
	b.n	.L_08019750
.L_0801972a:
	cmp	r0, #15
	beq.n	.L_08019750
	b.n	.L_08019766
.L_08019730:
	subs	r7, #3
	cmp	r7, #0
	ble.n	.L_0801977a
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	bl	sub_08007304
	adds	r6, #2
	add	r0, r9
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	bl	sub_08007304
	adds	r6, #2
	add	r0, r9
	b.n	.L_0801976c
.L_08019750:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L_0801977a
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	bl	sub_08007304
	ldr	r2, [pc, #80]
	adds	r6, #2
	adds	r0, r0, r2
	b.n	.L_0801976c
.L_08019766:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L_0801977a
.L_0801976c:
	strh	r0, [r6, #0]
	adds	r6, #2
.L_08019770:
	adds	r0, r5, #0
	bl	sub_08007304
	cmp	r0, #0
	bne.n	.L_08019718
.L_0801977a:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_08019786
	movs	r0, #50
	bl	sub_08002dd8
.L_08019786:
	ldr	r3, [pc, #20]
	add	sp, #12
	strh	r3, [r6, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e50
	.4byte 0x00000140
	.4byte 0x040000d4
	.4byte 0x08015430
	.4byte 0x0000ffff
