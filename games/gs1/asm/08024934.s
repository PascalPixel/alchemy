@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08024934
	.thumb_func
Func_08024934:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #372
	str	r2, [sp, #84]
	ldr	r5, [pc, #184]
	ldr	r0, [r5, #0]
	movs	r1, #1
	str	r0, [sp, #72]
	negs	r1, r1
	movs	r0, #128
	str	r1, [sp, #68]
	mov	r9, r1
	bl	Func_080040b4
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #42
	str	r0, [sp, #64]
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	Func_080162d4
	movs	r6, #6
	str	r0, [sp, #60]
	movs	r1, #8
	movs	r2, #10
	movs	r3, #3
	movs	r0, #20
	str	r6, [sp, #0]
	bl	Func_080162d4
	movs	r2, #0
	str	r0, [sp, #56]
	str	r2, [sp, #52]
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #52]
	ldr	r1, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r0
	mov	sl, r1
	str	r3, [sp, #48]
	str	r6, [sp, #0]
	movs	r2, #17
	movs	r3, #9
	movs	r0, #13
	movs	r1, #11
	bl	Func_080162d4
	movs	r2, #156
	lsls	r2, r2, #1
	add	r2, sp
	ldr	r3, [pc, #84]
	movs	r7, #128
	str	r0, [sp, #76]
	str	r2, [sp, #28]
	movs	r4, #0
	mov	ip, r3
	adds	r5, r2, #0
	lsls	r7, r7, #23
	movs	r6, #0
.L0:
	lsls	r0, r4, #1
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	ldr	r1, [sp, #76]
	ldrh	r2, [r1, #12]
	ldr	r3, [pc, #48]
	lsls	r2, r2, #3
	ldrh	r1, [r5, #6]
	adds	r2, #8
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #14]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r4, #1
	strb	r0, [r5, #4]
	adds	r5, #12
	cmp	r4, #3
	ble.n	.L0
	ldr	r3, [pc, #24]
	ldr	r7, [sp, #28]
	movs	r5, #8
	add	r6, sp, #96
	mov	r8, r3
	movs	r4, #3
	b.n	.L1
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.4byte 0xfffffc00
.L1:
	movs	r0, #128
	str	r4, [sp, #4]
	bl	Func_080040b4
	movs	r1, #1
	negs	r1, r1
	stmia	r6!, {r0}
	bl	Func_080040d0
	ldr	r3, [pc, #24]
	ands	r0, r3
	ldrh	r3, [r5, r7]
	mov	r1, r8
	ldr	r4, [sp, #4]
	ands	r3, r1
	orrs	r3, r0
	subs	r4, #1
	strh	r3, [r5, r7]
	adds	r5, #12
	cmp	r4, #0
	bge.n	.L1
	b.n	.L2
	.4byte 0x000003ff
.L2:
	movs	r2, #138
	lsls	r2, r2, #1
	add	r2, sp
	mov	r8, r2
	mov	r0, r8
	bl	Func_080771d8
	str	r0, [sp, #80]
	movs	r7, #0
	adds	r3, r0, #0
	subs	r3, #1
	str	r3, [sp, #20]
	cmp	r3, #0
	blt.n	.L3
	mov	r0, sp
	adds	r0, #240
	adds	r5, r3, #0
	str	r0, [sp, #32]
	add	r5, r8
.L7:
	ldrb	r6, [r5, #0]
	adds	r0, r6, #0
	bl	Func_080771e0
	ldr	r1, [sp, #84]
	adds	r0, #4
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	movs	r4, #0
	cmp	r2, r3
	bhi.n	.L4
.L5:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L4
	adds	r0, #1
	adds	r1, #1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	cmp	r2, r3
	bls.n	.L5
.L4:
	cmp	r4, #4
	bne.n	.L6
	ldr	r2, [sp, #32]
	movs	r3, #32
	strb	r6, [r2, r7]
	strb	r3, [r5, #0]
	adds	r7, #1
.L6:
	subs	r5, #1
	cmp	r5, r8
	bge.n	.L7
	b.n	.L8
.L3:
	mov	r3, sp
	adds	r3, #240
	str	r3, [sp, #32]
.L8:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	ble.n	.L9
	ldr	r2, [sp, #32]
	adds	r1, r7, r2
	ldr	r2, [sp, #80]
	mov	r0, r8
.L11:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, #32
	beq.n	.L10
	strb	r3, [r1, #0]
	adds	r7, #1
	adds	r1, #1
.L10:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L11
.L9:
	ldr	r0, [sp, #32]
	movs	r3, #32
	strb	r3, [r0, r7]
	movs	r1, #180
	ldr	r3, [sp, #64]
	lsls	r1, r1, #1
	mov	r2, sl
	add	r1, sp
	lsls	r2, r2, #1
	lsls	r3, r3, #16
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
.L73:
	cmp	fp, r9
	bne.n	.L12
	ldr	r0, [sp, #68]
	cmp	sl, r0
	bne.n	.L12
	b.n	.L13
.L12:
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #316]
	movs	r0, #1
	adds	r3, r1, r2
	strb	r0, [r3, #0]
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #68]
	ldrh	r0, [r1, #12]
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	subs	r2, #2
	adds	r1, #1
	movs	r3, #1
	adds	r0, #1
	bl	Func_08022768
	bl	Func_08016738
	mov	r3, fp
	ldr	r1, [sp, #32]
	add	r3, sl
	ldrb	r0, [r1, r3]
	bl	Func_080771e0
	adds	r6, r0, #0
	ldrh	r0, [r6, #0]
	ldr	r3, [pc, #260]
	add	r5, sp, #112
	adds	r0, r0, r3
	adds	r1, r5, #0
	movs	r2, #52
	bl	Func_0801965c
	movs	r2, #0
	ldr	r1, [sp, #60]
	movs	r3, #4
	adds	r0, r5, #0
	bl	Func_08017aa4
	movs	r3, #0
	mov	r2, sl
	str	r3, [sp, #52]
	str	r2, [sp, #68]
	movs	r1, #1
	movs	r2, #0
	adds	r6, #4
.L15:
	ldrb	r3, [r6, #0]
	adds	r6, #1
	cmp	r3, #0
	beq.n	.L14
	ldr	r0, [sp, #52]
	adds	r3, r1, #0
	lsls	r3, r2
	orrs	r0, r3
	str	r0, [sp, #52]
.L14:
	adds	r2, #1
	cmp	r2, #3
	ble.n	.L15
	cmp	fp, r9
	bne.n	.L16
	b.n	.L17
.L16:
	ldr	r0, [sp, #76]
	bl	Func_08016498
	movs	r5, #0
	movs	r7, #0
	movs	r6, #1
.L18:
	ldr	r2, [pc, #184]
	ldr	r0, [sp, #56]
	adds	r1, r5, r2
	movs	r3, #0
	lsls	r2, r5, #1
	str	r7, [sp, #0]
	bl	Func_08019000
	ldr	r3, [sp, #84]
	ldrb	r1, [r3, r5]
	adds	r2, r6, #0
	adds	r1, #48
	ldr	r0, [sp, #56]
	movs	r3, #0
	adds	r5, #1
	str	r7, [sp, #0]
	adds	r6, #2
	bl	Func_08018efc
	cmp	r5, #3
	ble.n	.L18
	ldr	r0, [sp, #32]
	mov	r1, fp
	ldrb	r6, [r0, r1]
	movs	r4, #0
	cmp	r6, #32
	bne.n	.L19
	b.n	.L20
.L19:
	mov	r2, sp
	adds	r2, #88
	str	r2, [sp, #8]
.L29:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	Func_080771e0
	str	r0, [sp, #36]
	adds	r1, r0, #0
	ldr	r0, [sp, #84]
	adds	r1, #4
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	movs	r7, #0
	ldr	r4, [sp, #4]
	cmp	r2, r3
	bhi.n	.L21
.L22:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L21
	adds	r1, #1
	adds	r0, #1
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	cmp	r2, r3
	bls.n	.L22
.L21:
	movs	r3, #4
	eors	r3, r7
	ldr	r2, [sp, #36]
	negs	r5, r3
	orrs	r5, r3
	ldr	r0, [pc, #48]
	ldrh	r3, [r2, #0]
	movs	r1, #1
	ands	r0, r3
	add	r2, sp, #96
	lsls	r3, r4, #2
	lsrs	r5, r5, #31
	adds	r2, r2, r3
	str	r1, [sp, #0]
	ldr	r3, [sp, #8]
	subs	r5, r1, r5
	movs	r1, #0
	str	r4, [sp, #4]
	bl	Func_0801a3d0
	ldr	r4, [sp, #4]
	lsls	r3, r4, #1
	adds	r1, r3, r4
	ldr	r2, [sp, #28]
	mov	r8, r3
	lsls	r1, r1, #2
	ldr	r3, [pc, #12]
	ldr	r0, [sp, #88]
	adds	r1, #8
	ands	r0, r3
	b.n	.L23
	.4byte 0x00003fff
	.4byte 0x000003ff
	.4byte 0x00000ea6
	.4byte 0x0000053a
	.4byte 0x00005001
.L23:
	ldrh	r3, [r2, r1]
	ldr	r2, [pc, #56]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #28]
	strh	r3, [r0, r1]
	cmp	r5, #0
	bne.n	.L24
	movs	r0, #2
	bl	Func_0801e71c
	ldr	r4, [sp, #4]
.L24:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	Func_080771e0
	ldr	r3, [pc, #28]
	ldr	r4, [sp, #4]
	ldrh	r0, [r0, #0]
	ldr	r1, [sp, #76]
	adds	r0, r0, r3
	movs	r2, #16
	lsls	r3, r4, #4
	bl	Func_0801e7c0
	movs	r1, #0
	ldr	r6, [sp, #36]
	lsls	r3, r1, #1
	b.n	.L25
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x00000333
.L25:
	adds	r5, r3, #0
	ldr	r4, [sp, #4]
	movs	r7, #0
	mov	r9, r1
	adds	r6, #4
	adds	r5, #13
.L27:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L26
	ldr	r2, [pc, #128]
	mov	r3, r9
	adds	r1, r7, r2
	str	r3, [sp, #0]
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	mov	r3, r8
	str	r4, [sp, #4]
	bl	Func_08019000
	ldrb	r1, [r6, #0]
	mov	r0, r9
	adds	r2, r5, #1
	str	r0, [sp, #0]
	adds	r1, #48
	ldr	r0, [sp, #76]
	mov	r3, r8
	bl	Func_08018efc
	ldr	r4, [sp, #4]
	adds	r5, #2
.L26:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, #3
	ble.n	.L27
	movs	r0, #15
	str	r4, [sp, #4]
	bl	Func_0801e71c
	ldr	r4, [sp, #4]
	add	r3, sp, #92
	movs	r1, #1
	strb	r1, [r3, r4]
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L28
	mov	r2, fp
	ldr	r0, [sp, #32]
	adds	r3, r2, r4
	ldrb	r6, [r0, r3]
	cmp	r6, #32
	beq.n	.L20
	b.n	.L29
.L20:
	cmp	r4, #3
	bgt.n	.L28
	add	r2, sp, #372
	ldr	r0, [pc, #40]
	adds	r3, r4, r2
	adds	r2, r3, r0
	movs	r3, #4
	movs	r1, #0
	subs	r4, r3, r4
.L30:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L30
.L28:
	mov	r9, fp
.L17:
	ldr	r1, [sp, #80]
	cmp	r1, #4
	ble.n	.L31
	movs	r4, #0
	adds	r5, r1, #0
	adds	r5, #3
	b.n	.L32
	.4byte 0x00005001
	.4byte 0xfffffee8
.L36:
	ldr	r2, [pc, #256]
	mov	r3, fp
	adds	r1, r4, r2
	cmp	r3, #0
	bge.n	.L33
	adds	r3, #3
.L33:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L34
	ldr	r3, [pc, #244]
	adds	r1, r4, r3
.L34:
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	subs	r2, r2, r0
	movs	r0, #0
	adds	r2, r2, r4
	str	r0, [sp, #0]
	adds	r0, r3, #0
	movs	r3, #1
	subs	r2, #2
	negs	r3, r3
	str	r4, [sp, #4]
	bl	Func_08019000
	ldr	r4, [sp, #4]
	adds	r4, #1
.L32:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L35
	ldr	r3, [sp, #80]
	adds	r3, #6
.L35:
	asrs	r0, r3, #2
	cmp	r4, r0
	blt.n	.L36
.L31:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #16]
	ldrh	r0, [r1, #12]
	ldr	r3, [sp, #76]
	ldrh	r1, [r1, #14]
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	movs	r3, #1
	bl	Func_08022768
	ldr	r1, [pc, #168]
	ldr	r0, [sp, #72]
	movs	r2, #1
	adds	r3, r0, r1
	adds	r1, #3
	strb	r2, [r3, #0]
	adds	r3, r0, r1
	movs	r2, #0
	strb	r2, [r3, #0]
.L13:
	ldr	r6, [sp, #28]
	movs	r4, #0
	add	r5, sp, #92
.L38:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	cmp	r3, #0
	beq.n	.L37
	adds	r0, r6, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	Func_08003dec
	ldr	r4, [sp, #4]
.L37:
	adds	r4, #1
	adds	r6, #12
	cmp	r4, #3
	ble.n	.L38
	ldr	r0, [sp, #76]
	ldrh	r3, [r0, #12]
	lsls	r3, r3, #3
	subs	r3, #2
	ldr	r1, [sp, #16]
	str	r3, [sp, #40]
	ldrh	r3, [r0, #14]
	adds	r3, r1, r3
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r2, [sp, #24]
	str	r3, [sp, #44]
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	ldr	r1, [sp, #12]
	lsrs	r0, r1, #16
	ldr	r1, [pc, #80]
	bl	Func_080040d0
	ldr	r3, [pc, #48]
	ldr	r2, [sp, #24]
	ands	r0, r3
	ldrh	r3, [r2, #8]
	ldr	r2, [pc, #44]
	ldr	r1, [pc, #68]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #24]
	ldr	r2, [r1, #0]
	strh	r3, [r0, #8]
	movs	r0, #4
	ldr	r3, [sp, #40]
	ands	r2, r0
	ldr	r1, [pc, #56]
	lsrs	r2, r2, #1
	adds	r2, r3, r2
	adds	r2, r2, r1
	ldr	r3, [pc, #20]
	ldr	r1, [sp, #24]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	ands	r3, r1
	b.n	.L39
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.4byte 0x03001e40
	.4byte 0x0000fffc
.L39:
	orrs	r3, r2
	ldr	r1, [pc, #800]
	ldr	r2, [sp, #24]
	strh	r3, [r2, #6]
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #44]
	ands	r3, r0
	lsrs	r3, r3, #2
	ldr	r0, [sp, #24]
	subs	r3, r2, r3
	adds	r3, #248
	strb	r3, [r0, #4]
	movs	r1, #242
	ldr	r0, [sp, #24]
	bl	Func_08003dec
	ldr	r1, [pc, #768]
	ldr	r6, [r1, #0]
	movs	r3, #8
	ands	r6, r3
	movs	r5, #0
.L41:
	negs	r3, r6
	orrs	r3, r6
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	movs	r3, #15
	subs	r2, r3, r2
	ldr	r0, [sp, #52]
	movs	r3, #1
	lsls	r3, r5
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L40
	movs	r2, #15
.L40:
	ldr	r1, [sp, #56]
	ldrh	r0, [r1, #12]
	lsls	r3, r5, #1
	ldrh	r1, [r1, #14]
	adds	r0, r0, r3
	str	r2, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #2
	movs	r3, #1
	adds	r5, #1
	bl	Func_08022768
	cmp	r5, #3
	ble.n	.L41
	ldr	r2, [sp, #80]
	cmp	r2, #4
	ble.n	.L42
	movs	r4, #0
	adds	r5, r2, #0
	adds	r5, #3
	b.n	.L43
.L48:
	ldr	r3, [pc, #692]
	ldr	r0, [pc, #688]
	adds	r1, r4, r3
	ldr	r3, [r0, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L44
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L45
	adds	r3, #3
.L45:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L44
	ldr	r2, [pc, #668]
	adds	r1, r4, r2
.L44:
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	ldrh	r3, [r0, #8]
	cmp	r5, #0
	bge.n	.L46
	ldr	r2, [sp, #80]
	adds	r2, #6
.L46:
	asrs	r2, r2, #2
	subs	r2, r3, r2
	adds	r2, r2, r4
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	ldr	r0, [sp, #76]
	subs	r3, #1
	str	r4, [sp, #4]
	bl	Func_08019000
	ldr	r4, [sp, #4]
	adds	r4, #1
.L43:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L47
	ldr	r3, [sp, #80]
	adds	r3, #6
.L47:
	asrs	r2, r3, #2
	cmp	r4, r2
	blt.n	.L48
	ldr	r0, [sp, #76]
	ldrh	r3, [r0, #8]
	movs	r5, #1
	negs	r5, r5
	subs	r2, r3, r2
	movs	r1, #0
	str	r1, [sp, #0]
	ldr	r0, [sp, #76]
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #592]
	bl	Func_08019000
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	movs	r0, #0
	str	r0, [sp, #0]
	subs	r2, #2
	adds	r0, r3, #0
	ldr	r1, [pc, #576]
	adds	r3, r5, #0
	bl	Func_08019000
	ldr	r2, [sp, #72]
	ldr	r3, [pc, #572]
	ldr	r0, [sp, #76]
	adds	r1, r2, r3
	ldrh	r3, [r0, #14]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L42:
	ldr	r3, [pc, #552]
	ldr	r2, [r3, #0]
	mov	r1, fp
	mov	r3, sl
	str	r1, [r2, #52]
	str	r3, [r2, #48]
	ldr	r0, [sp, #48]
	str	r0, [r2, #56]
	ldr	r3, [pc, #540]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #540]
	ldr	r0, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L49
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L50
	movs	r3, #120
	str	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	b.n	.L49
.L50:
	subs	r3, #1
	str	r3, [r2, #0]
.L49:
	adds	r3, r1, #0
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L51
	mov	r3, fp
	ldr	r0, [sp, #32]
	add	r3, sl
	ldrb	r6, [r0, r3]
	b.n	.L52
.L51:
	ldr	r3, [pc, #472]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L53
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L54
.L53:
	movs	r0, #113
	movs	r6, #1
	bl	Func_080f9010
	negs	r6, r6
	b.n	.L52
.L54:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L55
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #4
	beq.n	.L56
	mov	r3, fp
	ldr	r0, [sp, #80]
	add	r3, sl
	cmp	r3, r0
	bne.n	.L57
.L56:
	movs	r1, #0
	mov	sl, r1
.L57:
	mov	r3, sl
	mov	r2, sl
	lsls	r3, r3, #1
	str	r2, [sp, #48]
	b.n	.L58
.L55:
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L59
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r1, sl
	cmp	r1, #0
	bge.n	.L60
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L61
	ldr	r3, [sp, #80]
	adds	r3, #2
.L61:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L62
	ldr	r2, [sp, #80]
	mov	r0, fp
	subs	r3, r2, r0
	subs	r3, #1
	mov	sl, r3
	b.n	.L60
.L62:
	movs	r1, #3
	mov	sl, r1
.L60:
	mov	r3, sl
	mov	r2, sl
	lsls	r3, r3, #1
	str	r2, [sp, #48]
	b.n	.L58
.L59:
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L63
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	mov	r3, fp
	ldr	r0, [sp, #80]
	adds	r3, #4
	cmp	r3, r0
	blt.n	.L64
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L65
	ldr	r3, [sp, #48]
	mov	sl, r3
	mov	r0, sl
	movs	r2, #0
	lsls	r0, r0, #1
	mov	fp, r2
	str	r0, [sp, #16]
	b.n	.L65
.L64:
	mov	fp, r3
	ldr	r1, [sp, #48]
	ldr	r3, [sp, #20]
	mov	sl, r1
	cmp	r3, #0
	bge.n	.L66
	ldr	r3, [sp, #80]
	adds	r3, #2
.L66:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L67
	ldr	r2, [sp, #80]
	mov	r0, fp
	subs	r3, r2, r0
	subs	r3, #1
	ldr	r1, [sp, #48]
	mov	sl, r3
	cmp	sl, r1
	ble.n	.L68
	mov	sl, r1
	b.n	.L69
.L63:
	movs	r3, #32
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L65
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L70
	ldr	r1, [sp, #48]
	mov	sl, r1
	movs	r0, #4
	mov	r2, sl
	negs	r0, r0
	lsls	r2, r2, #1
	add	fp, r0
	str	r2, [sp, #16]
	b.n	.L65
.L70:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L71
	ldr	r3, [sp, #80]
	adds	r3, #2
.L71:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	fp, r3
	ldr	r3, [sp, #48]
	mov	r0, fp
	mov	sl, r3
	cmp	r0, #0
	beq.n	.L69
	ldr	r1, [sp, #80]
	subs	r3, r1, r0
	subs	r3, #1
	ldr	r2, [sp, #48]
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L72
	mov	sl, r2
	b.n	.L72
.L67:
	mov	r0, sl
	lsls	r0, r0, #1
	str	r0, [sp, #16]
	b.n	.L65
.L68:
	mov	r1, sl
	lsls	r1, r1, #1
	str	r1, [sp, #16]
	b.n	.L65
.L69:
	mov	r2, sl
	lsls	r2, r2, #1
	str	r2, [sp, #16]
	b.n	.L65
.L72:
	mov	r3, sl
	lsls	r3, r3, #1
.L58:
	str	r3, [sp, #16]
.L65:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L73
.L52:
	movs	r0, #1
	bl	Func_080030f8
	movs	r4, #3
	add	r5, sp, #96
.L74:
	ldmia	r5!, {r0}
	str	r4, [sp, #4]
	bl	Func_08003f3c
	ldr	r4, [sp, #4]
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L74
	ldr	r1, [sp, #12]
	lsrs	r0, r1, #16
	bl	Func_08003f3c
	movs	r1, #1
	ldr	r0, [sp, #56]
	bl	Func_08016418
	movs	r1, #1
	ldr	r0, [sp, #60]
	bl	Func_08016418
	movs	r1, #1
	ldr	r0, [sp, #76]
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r6, #0
	add	sp, #372
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e40
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x00000ea3
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x03001b04
