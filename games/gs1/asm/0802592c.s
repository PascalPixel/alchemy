@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0802592c
	.thumb_func
Func_0802592c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #300
	str	r1, [sp, #72]
	str	r2, [sp, #68]
	ldr	r5, [pc, #172]
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	movs	r1, #1
	str	r0, [sp, #64]
	negs	r1, r1
	movs	r0, #128
	str	r1, [sp, #56]
	mov	r9, r1
	bl	Func_080040b4
	str	r0, [sp, #52]
	adds	r0, r6, #0
	bl	Func_08077008
	movs	r3, #42
	str	r0, [sp, #48]
	str	r3, [sp, #0]
	movs	r1, #5
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	Func_080162d4
	movs	r2, #5
	str	r0, [sp, #44]
	str	r2, [sp, #40]
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #52]
	str	r0, [sp, #60]
	ldr	r1, [r3, #48]
	ldr	r3, [r3, #56]
	mov	sl, r1
	str	r3, [sp, #36]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r2, #21
	movs	r3, #11
	movs	r0, #9
	movs	r1, #9
	bl	Func_080162d4
	mov	r2, sp
	adds	r2, #80
	ldr	r3, [pc, #88]
	movs	r6, #128
	str	r2, [sp, #12]
	mov	fp, r0
	movs	r7, #0
	mov	ip, r3
	adds	r4, r2, #0
	lsls	r6, r6, #23
	movs	r5, #0
.L0:
	lsls	r0, r7, #1
	str	r6, [r4, #4]
	str	r5, [r4, #8]
	mov	r1, fp
	ldrh	r2, [r1, #12]
	ldr	r3, [pc, #52]
	lsls	r2, r2, #3
	ldrh	r1, [r4, #6]
	adds	r2, #8
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	mov	r2, fp
	strh	r3, [r4, #6]
	ldrh	r3, [r2, #14]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r7, #1
	strb	r0, [r4, #4]
	adds	r4, #12
	cmp	r7, #4
	ble.n	.L0
	mov	r3, sp
	adds	r3, #140
	ldr	r0, [pc, #20]
	str	r3, [sp, #8]
	ldr	r6, [sp, #12]
	str	r3, [sp, #4]
	movs	r5, #8
	b.n	.L1
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.4byte 0xfffffc00
.L1:
	mov	r8, r0
	movs	r7, #4
.L2:
	movs	r0, #128
	bl	Func_080040b4
	ldr	r2, [sp, #4]
	stmia	r2!, {r0}
	adds	r1, r2, #0
	str	r1, [sp, #4]
	movs	r1, #1
	negs	r1, r1
	bl	Func_080040d0
	ldr	r3, [pc, #52]
	ands	r0, r3
	ldrh	r3, [r5, r6]
	mov	r1, r8
	ands	r3, r1
	orrs	r3, r0
	subs	r7, #1
	strh	r3, [r5, r6]
	adds	r5, #12
	cmp	r7, #0
	bge.n	.L2
	ldr	r5, [pc, #32]
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	Func_080251d4
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl	Func_080251d4
	adds	r5, #1
	movs	r1, #132
	lsls	r1, r1, #2
	adds	r0, r5, #0
	b.n	.L3
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0000f018
	.4byte 0x00000201
.L3:
	bl	Func_080251d4
	ldr	r1, [pc, #120]
	adds	r0, r5, #0
	bl	Func_080251d4
	movs	r2, #144
	lsls	r2, r2, #1
	mov	r3, sl
	add	r2, sp
	lsls	r3, r3, #1
	str	r2, [sp, #24]
	str	r3, [sp, #20]
.L52:
	ldr	r0, [sp, #60]
	cmp	r0, r9
	bne.n	.L4
	ldr	r1, [sp, #56]
	cmp	sl, r1
	bne.n	.L4
	b.n	.L5
.L4:
	ldr	r3, [sp, #64]
	ldr	r0, [pc, #88]
	adds	r2, r3, r0
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [sp, #56]
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	movs	r3, #1
	bl	Func_08022768
	bl	Func_08016738
	ldr	r0, [sp, #68]
	cmp	r0, #0
	beq.n	.L6
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #72]
	add	r3, sl
	lsls	r3, r3, #1
	ldrh	r3, [r3, r1]
	ldr	r0, [pc, #20]
	ands	r0, r3
	ldr	r3, [pc, #28]
	add	r5, sp, #160
	adds	r0, r0, r3
	adds	r1, r5, #0
	movs	r2, #52
	bl	Func_0801965c
	b.n	.L7
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x00000211
	.4byte 0x00000ea6
	.4byte 0x0000053a
.L6:
	add	r5, sp, #160
	ldr	r0, [pc, #180]
	adds	r1, r5, #0
	movs	r2, #52
	bl	Func_0801965c
.L7:
	movs	r2, #0
	movs	r3, #4
	adds	r0, r5, #0
	ldr	r1, [sp, #44]
	bl	Func_08017aa4
	ldr	r3, [sp, #60]
	mov	r2, sl
	str	r2, [sp, #56]
	cmp	r3, r9
	bne.n	.L8
	b.n	.L9
.L8:
	mov	r0, fp
	bl	Func_08016498
	ldr	r0, [sp, #60]
	ldr	r1, [sp, #72]
	lsls	r3, r0, #1
	ldrh	r5, [r3, r1]
	movs	r7, #0
	cmp	r5, #0
	bne.n	.L10
	b.n	.L11
.L10:
	mov	r2, sp
	adds	r2, #76
	str	r2, [sp, #16]
.L18:
	adds	r0, r5, #0
	bl	Func_08077080
	mov	r8, r0
	movs	r0, #0
	str	r0, [sp, #0]
	lsls	r3, r7, #1
	mov	r0, fp
	ldr	r1, [pc, #104]
	movs	r2, #11
	mov	r9, r3
	bl	Func_08019000
	movs	r1, #0
	str	r1, [sp, #0]
	mov	r0, fp
	ldr	r1, [pc, #92]
	movs	r2, #12
	mov	r3, r9
	bl	Func_08019000
	ldr	r3, [sp, #8]
	ldr	r0, [pc, #84]
	lsls	r2, r7, #2
	adds	r2, r3, r2
	movs	r3, #1
	ands	r0, r5
	str	r3, [sp, #0]
	movs	r1, #0
	ldr	r3, [sp, #16]
	bl	Func_0801a3d0
	mov	r0, r9
	adds	r1, r0, r7
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #40]
	lsls	r1, r1, #2
	ldr	r0, [sp, #76]
	adds	r1, #8
	ands	r0, r3
	ldrh	r3, [r2, r1]
	ldr	r2, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #12]
	strh	r3, [r0, r1]
	mov	r1, r8
	ldrb	r2, [r1, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L12
	movs	r0, #4
	bl	Func_0801e71c
	b.n	.L13
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000008e7
	.4byte 0x0000f01f
	.4byte 0x0000f01e
	.4byte 0x00003fff
.L12:
	ldr	r1, [sp, #48]
	mov	r3, r8
	ldrb	r2, [r3, #9]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	ble.n	.L14
	movs	r0, #2
	bl	Func_0801e71c
	b.n	.L13
.L14:
	ldr	r2, [sp, #48]
	ldr	r0, [pc, #284]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L13
	movs	r0, #9
	bl	Func_0801e71c
.L13:
	ldr	r1, [sp, #64]
	ldr	r2, [pc, #272]
	ldr	r0, [pc, #272]
	adds	r6, r1, r2
	adds	r0, r5, r0
	movs	r3, #5
	lsls	r5, r7, #4
	strb	r3, [r6, #0]
	mov	r1, fp
	movs	r2, #16
	adds	r3, r5, #0
	bl	Func_0801e7c0
	mov	r3, r8
	ldrb	r0, [r3, #9]
	movs	r1, #2
	movs	r3, #104
	mov	r2, fp
	str	r5, [sp, #0]
	bl	Func_0801e9d4
	movs	r0, #15
	bl	Func_0801e71c
	movs	r3, #15
	mov	r0, r8
	strb	r3, [r6, #0]
	ldrb	r3, [r0, #2]
	cmp	r3, #4
	beq.n	.L15
	ldr	r2, [pc, #220]
	adds	r1, r3, #0
	movs	r3, #0
	adds	r1, r1, r2
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r2, #15
	mov	r3, r9
	bl	Func_08019000
.L15:
	mov	r0, r8
	ldrb	r3, [r0, #8]
	cmp	r3, #255
	bne.n	.L16
	movs	r3, #11
	b.n	.L17
.L16:
	subs	r3, #1
.L17:
	movs	r1, #0
	str	r1, [sp, #0]
	mov	r0, fp
	movs	r1, #16
	mov	r2, r9
	adds	r7, #1
	bl	Func_080218dc
	cmp	r7, #4
	bgt.n	.L11
	ldr	r2, [sp, #60]
	ldr	r0, [sp, #72]
	adds	r3, r2, r7
	lsls	r3, r3, #1
	ldrh	r5, [r3, r0]
	cmp	r5, #0
	beq.n	.L11
	b.n	.L18
.L11:
	ldr	r1, [sp, #60]
	str	r7, [sp, #40]
	mov	r9, r1
.L9:
	ldr	r2, [sp, #68]
	cmp	r2, #5
	ble.n	.L19
	movs	r7, #0
	adds	r2, #4
	mov	r8, r2
	b.n	.L20
.L22:
	ldr	r3, [pc, #136]
	ldr	r0, [sp, #60]
	movs	r1, #5
	adds	r6, r7, r3
	bl	Func_080022ec
	cmp	r7, r0
	bne.n	.L21
	ldr	r0, [pc, #124]
	adds	r6, r7, r0
.L21:
	mov	r1, fp
	ldrh	r2, [r1, #8]
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, fp
	adds	r1, r6, #0
	subs	r3, #1
	bl	Func_08019000
	adds	r7, #1
.L20:
	mov	r0, r8
	movs	r1, #5
	bl	Func_080022ec
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L22
.L19:
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldr	r2, [sp, #20]
	ldrh	r1, [r1, #14]
	mov	r3, fp
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	movs	r3, #1
	bl	Func_08022768
	ldr	r1, [pc, #52]
	ldr	r0, [sp, #64]
	movs	r3, #1
	adds	r2, r0, r1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #48]
	adds	r3, r0, r2
	movs	r0, #0
	strb	r0, [r3, #0]
.L5:
	ldr	r1, [sp, #68]
	cmp	r1, #5
	ble.n	.L23
	movs	r7, #0
	adds	r1, #4
	mov	r8, r1
	b.n	.L24
	.4byte 0x0000013d
	.4byte 0x00000ea7
	.4byte 0x00000333
	.4byte 0x00005001
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x00000ea3
	.4byte 0x00000ea6
.L26:
	ldr	r0, [pc, #280]
	ldr	r2, [pc, #284]
	ldr	r3, [r0, #0]
	adds	r6, r7, r2
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L25
	ldr	r0, [sp, #60]
	movs	r1, #5
	bl	Func_080022ec
	cmp	r7, r0
	bne.n	.L25
	ldr	r1, [pc, #260]
	adds	r6, r7, r1
.L25:
	mov	r2, fp
	movs	r1, #5
	mov	r0, r8
	ldrh	r5, [r2, #8]
	bl	Func_080022ec
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r3, #0
	subs	r5, #2
	str	r3, [sp, #0]
	mov	r0, fp
	adds	r1, r6, #0
	adds	r2, r5, #0
	subs	r3, #1
	bl	Func_08019000
	adds	r7, #1
.L24:
	mov	r0, r8
	movs	r1, #5
	bl	Func_080022ec
	cmp	r7, r0
	blt.n	.L26
	mov	r1, fp
	ldrh	r2, [r1, #8]
	movs	r5, #1
	negs	r5, r5
	subs	r2, r2, r0
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r0, fp
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #192]
	bl	Func_08019000
	mov	r0, fp
	ldrh	r2, [r0, #8]
	movs	r1, #0
	str	r1, [sp, #0]
	subs	r2, #2
	ldr	r1, [pc, #180]
	adds	r3, r5, #0
	bl	Func_08019000
	ldr	r2, [sp, #64]
	ldr	r3, [pc, #176]
	mov	r0, fp
	adds	r1, r2, r3
	ldrh	r3, [r0, #14]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L23:
	ldr	r1, [sp, #40]
	cmp	r1, #0
	ble.n	.L27
	ldr	r5, [sp, #12]
	adds	r7, r1, #0
.L28:
	adds	r0, r5, #0
	movs	r1, #240
	subs	r7, #1
	bl	Func_08003dec
	adds	r5, #12
	cmp	r7, #0
	bne.n	.L28
.L27:
	mov	r2, fp
	ldrh	r3, [r2, #12]
	lsls	r3, r3, #3
	subs	r3, #4
	ldr	r0, [sp, #20]
	str	r3, [sp, #28]
	ldrh	r3, [r2, #14]
	adds	r3, r0, r3
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r1, [sp, #24]
	str	r3, [sp, #32]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #96]
	bl	Func_080040d0
	ldr	r3, [pc, #48]
	ldr	r1, [sp, #24]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #48]
	ldr	r2, [r3, #0]
	movs	r0, #4
	ands	r2, r0
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #64]
	lsrs	r2, r2, #1
	adds	r2, r1, r2
	adds	r2, r2, r3
	ldr	r1, [sp, #24]
	ldr	r3, [pc, #20]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	ands	r3, r1
	b.n	.L29
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001e40
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.4byte 0x0000fffc
.L29:
	orrs	r3, r2
	ldr	r1, [pc, #556]
	ldr	r2, [sp, #24]
	strh	r3, [r2, #6]
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #32]
	ands	r3, r0
	lsrs	r3, r3, #2
	subs	r3, r2, r3
	ldr	r0, [sp, #24]
	adds	r3, #248
	strb	r3, [r0, #4]
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L30
	ldr	r0, [sp, #24]
	movs	r1, #242
	bl	Func_08003dec
.L30:
	ldr	r3, [pc, #524]
	ldr	r2, [sp, #60]
	ldr	r1, [r3, #0]
	mov	r3, sl
	str	r2, [r1, #52]
	str	r3, [r1, #48]
	ldr	r0, [sp, #36]
	str	r0, [r1, #56]
	ldr	r0, [pc, #512]
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L31
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L32
	ldr	r6, [sp, #60]
	ldr	r2, [sp, #72]
	add	r6, sl
	lsls	r3, r6, #1
	ldrh	r0, [r3, r2]
	bl	Func_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L33
	b.n	.L34
.L33:
	b.n	.L35
.L32:
	movs	r6, #1
	negs	r6, r6
	b.n	.L34
.L31:
	ldr	r3, [r1, #76]
	cmp	r3, #0
	beq.n	.L36
	ldr	r3, [r0, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L35
.L36:
	movs	r0, #113
	movs	r6, #1
	bl	Func_080f9010
	negs	r6, r6
	b.n	.L34
.L35:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L37
	b.n	.L38
.L37:
	ldr	r1, [pc, #428]
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L39
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #5
	beq.n	.L40
	ldr	r3, [sp, #60]
	ldr	r2, [sp, #68]
	add	r3, sl
	cmp	r3, r2
	bne.n	.L41
.L40:
	movs	r3, #0
	mov	sl, r3
.L41:
	mov	r1, sl
	mov	r0, sl
	lsls	r1, r1, #1
	str	r0, [sp, #36]
	str	r1, [sp, #20]
	b.n	.L38
.L39:
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L42
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L43
	ldr	r0, [sp, #68]
	movs	r1, #5
	subs	r0, #1
	bl	Func_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r0, [sp, #60]
	cmp	r0, r3
	bne.n	.L44
	ldr	r1, [sp, #68]
	subs	r3, r1, r0
	subs	r3, #1
	mov	sl, r3
	b.n	.L43
.L44:
	movs	r2, #4
	mov	sl, r2
.L43:
	mov	r0, sl
	mov	r3, sl
	lsls	r0, r0, #1
	str	r3, [sp, #36]
	str	r0, [sp, #20]
	b.n	.L38
.L42:
	ldr	r3, [r1, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L45
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #68]
	adds	r3, #5
	cmp	r3, r1
	blt.n	.L46
	ldr	r2, [sp, #60]
	cmp	r2, #0
	beq.n	.L38
	ldr	r0, [sp, #36]
	mov	sl, r0
	mov	r1, sl
	movs	r3, #0
	lsls	r1, r1, #1
	str	r3, [sp, #60]
	str	r1, [sp, #20]
	b.n	.L38
.L46:
	ldr	r0, [sp, #68]
	ldr	r2, [sp, #36]
	subs	r0, #1
	movs	r1, #5
	str	r3, [sp, #60]
	mov	sl, r2
	bl	Func_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r0, [sp, #60]
	cmp	r0, r3
	bne.n	.L47
	ldr	r1, [sp, #68]
	subs	r3, r1, r0
	subs	r3, #1
	ldr	r2, [sp, #36]
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L48
	mov	sl, r2
	b.n	.L49
.L45:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L38
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	ldr	r0, [sp, #60]
	cmp	r0, #0
	beq.n	.L50
	ldr	r1, [sp, #36]
	mov	sl, r1
	mov	r2, sl
	subs	r0, #5
	lsls	r2, r2, #1
	str	r0, [sp, #60]
	str	r2, [sp, #20]
	b.n	.L38
.L50:
	ldr	r0, [sp, #68]
	movs	r1, #5
	subs	r0, #1
	bl	Func_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	str	r3, [sp, #60]
	ldr	r0, [sp, #60]
	ldr	r3, [sp, #36]
	mov	sl, r3
	cmp	r0, #0
	beq.n	.L51
	ldr	r1, [sp, #68]
	subs	r3, r1, r0
	subs	r3, #1
	ldr	r2, [sp, #36]
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L49
	mov	sl, r2
	b.n	.L49
.L47:
	mov	r0, sl
	lsls	r0, r0, #1
	str	r0, [sp, #20]
	b.n	.L38
.L48:
	mov	r1, sl
	lsls	r1, r1, #1
	str	r1, [sp, #20]
	b.n	.L38
.L51:
	mov	r2, sl
	lsls	r2, r2, #1
	str	r2, [sp, #20]
	b.n	.L38
.L49:
	mov	r3, sl
	lsls	r3, r3, #1
	str	r3, [sp, #20]
.L38:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L52
.L34:
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	Func_08016418
	mov	r0, fp
	movs	r1, #1
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	ldr	r5, [sp, #8]
	movs	r7, #4
.L53:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	Func_08003f3c
	cmp	r7, #0
	bge.n	.L53
	ldr	r0, [sp, #52]
	bl	Func_08003f3c
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r6, #0
	add	sp, #300
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e40
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x03001b04
