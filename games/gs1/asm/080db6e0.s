@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080db6e0
	.thumb_func
Func_080db6e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #40]
	ldr	r3, [pc, #832]
	ldmia	r3!, {r1}
	ldr	r5, [pc, #832]
	mov	sl, r1
	ldr	r3, [r3, #0]
	add	r5, sl
	str	r3, [sp, #36]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	Func_080cd594
	movs	r2, #1
	ldr	r0, [pc, #816]
	mov	r1, sl
	movs	r3, #0
	bl	Func_080e0524
	ldr	r2, [sp, #40]
	cmp	r2, #1
	bne.n	.L0
	movs	r3, #0
	movs	r0, #160
	mov	r8, r3
	lsls	r0, r0, #19
.L1:
	mov	r4, r8
	lsrs	r3, r4, #31
	add	r3, r8
	asrs	r3, r3, #1
	lsls	r1, r3, #5
	lsls	r2, r3, #10
	orrs	r2, r1
	movs	r1, #1
	orrs	r2, r3
	add	r8, r1
	strh	r2, [r0, #0]
	mov	r2, r8
	adds	r0, #2
	cmp	r2, #64
	bne.n	.L1
	str	r1, [sp, #24]
	b.n	.L2
.L0:
	ldr	r0, [pc, #760]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #756]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	str	r3, [sp, #24]
.L2:
	movs	r5, #225
	movs	r3, #0
	lsls	r5, r5, #7
	mov	r8, r3
	movs	r7, #0
	movs	r6, #63
	add	r5, sl
.L5:
	ldr	r3, [pc, #712]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L3
	movs	r3, #200
	lsls	r3, r3, #14
	b.n	.L4
.L3:
	ldr	r3, [pc, #712]
.L4:
	str	r3, [r5, #0]
	str	r7, [r5, #4]
	str	r7, [r5, #8]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r6
	adds	r0, #16
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #32
	movs	r4, #1
	lsls	r0, r0, #13
	add	r8, r4
	str	r0, [r5, #20]
	mov	r0, r8
	str	r7, [r5, #24]
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L5
	movs	r1, #0
	ldr	r5, [pc, #656]
	mov	r8, r1
	movs	r6, #0
	movs	r7, #63
.L8:
	ldr	r3, [pc, #628]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L6
	movs	r3, #200
	lsls	r3, r3, #14
	b.n	.L7
.L6:
	ldr	r3, [pc, #624]
.L7:
	str	r3, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #8
	lsls	r3, r3, #13
	str	r3, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #32
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #13
	add	r8, r2
	lsls	r3, r3, #3
	str	r0, [r5, #20]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L8
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [pc, #556]
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #28]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #548]
	add	r3, sl
	str	r5, [r3, #0]
	add	r2, sl
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #536]
	bl	Func_080041d8
	ldr	r1, [sp, #24]
	ldr	r3, [sp, #24]
	lsls	r1, r1, #1
	str	r1, [sp, #12]
	adds	r2, r1, r3
	ldr	r0, [pc, #524]
	adds	r1, r2, #2
	ldrb	r3, [r0, r1]
	movs	r4, #0
	mov	r9, r4
	cmp	r3, #0
	bne.n	.L9
	b.n	.L10
.L9:
	adds	r4, r2, #0
	adds	r4, #1
	str	r2, [sp, #16]
	str	r4, [sp, #8]
	str	r1, [sp, #20]
	mov	fp, r0
.L31:
	ldr	r3, [pc, #500]
	ldr	r5, [r3, #0]
	bl	Func_080049ac
	adds	r1, r5, #0
	adds	r0, r5, #0
	adds	r1, #12
	bl	Func_080051d8
	mov	r0, r9
	cmp	r0, #2
	bne.n	.L11
	movs	r0, #144
	bl	Func_080f9010
.L11:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #24]
	ldr	r6, [pc, #460]
	adds	r5, r1, r2
	adds	r3, r5, #2
	ldrb	r3, [r6, r3]
	subs	r3, #48
	cmp	r9, r3
	bne.n	.L12
	movs	r0, #133
	bl	Func_080b50e8
.L12:
	movs	r3, #0
	mov	r8, r3
	ldrb	r3, [r6, r5]
	cmp	r3, #0
	beq.n	.L13
	ldr	r6, [pc, #416]
.L18:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	blt.n	.L14
	add	r5, sp, #44
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r2, [r5, #0]
	ldr	r3, [pc, #376]
	asrs	r2, r2, #1
	add	r3, sl
	str	r2, [r5, #0]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	lsls	r3, r3, #5
	adds	r2, r2, r3
	subs	r2, #16
	str	r2, [r5, #0]
	ldr	r2, [r5, #8]
	cmp	r2, #159
	bgt.n	.L15
	movs	r3, #160
	str	r3, [r5, #8]
	movs	r2, #160
.L15:
	ldr	r3, [pc, #388]
	cmp	r2, r3
	ble.n	.L16
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L16:
	adds	r3, r2, #0
	subs	r3, #160
	cmp	r3, #0
	bge.n	.L17
	adds	r3, #63
.L17:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #364]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	mov	r3, r8
	movs	r2, #1
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r1, r1, r3
	movs	r2, #200
	lsls	r2, r2, #6
	lsrs	r3, r0, #31
	add	r1, sl
	adds	r1, r1, r2
	adds	r3, r0, r3
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	bl	Func_080072f4
	adds	r0, r6, #0
	movs	r1, #64
	ldr	r2, [pc, #304]
	bl	Func_080e38b8
.L14:
	ldr	r2, [sp, #16]
	mov	r1, fp
	movs	r0, #1
	ldrb	r3, [r1, r2]
	add	r8, r0
	adds	r6, #28
	cmp	r8, r3
	bne.n	.L18
.L13:
	mov	r3, r9
	cmp	r3, #2
	ble.n	.L19
	ldr	r0, [sp, #8]
	mov	r2, fp
	ldrb	r3, [r2, r0]
	movs	r4, #0
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L19
	movs	r6, #225
	lsls	r6, r6, #7
	add	r6, sl
.L23:
	cmp	r8, r9
	bge.n	.L20
	ldr	r3, [r6, #4]
	cmp	r3, #0
	blt.n	.L20
	add	r5, sp, #44
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r2, [r5, #0]
	ldr	r3, [pc, #184]
	asrs	r2, r2, #1
	add	r3, sl
	str	r2, [r5, #0]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	lsls	r3, r3, #5
	adds	r2, r2, r3
	adds	r7, r2, #0
	subs	r7, #16
	str	r7, [r5, #0]
	ldr	r0, [r6, #24]
	cmp	r0, #20
	bhi.n	.L21
	movs	r1, #3
	bl	Func_080022ec
	ldr	r3, [pc, #204]
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	ldr	r3, [pc, #204]
	ldrh	r4, [r3, r0]
	ldr	r3, [r5, #4]
	lsrs	r0, r4, #1
	subs	r2, r7, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	add	r1, sl
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	bl	Func_080072f4
	ldr	r0, [r6, #24]
.L21:
	cmp	r0, #20
	bgt.n	.L22
	adds	r3, r0, #1
	str	r3, [r6, #24]
.L22:
	ldr	r2, [pc, #160]
	adds	r0, r6, #0
	movs	r1, #64
	bl	Func_080e38b8
	ldr	r2, [pc, #132]
.L20:
	ldr	r1, [sp, #8]
	movs	r0, #1
	ldrb	r3, [r2, r1]
	add	r8, r0
	adds	r6, #28
	cmp	r8, r3
	bne.n	.L23
.L19:
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L24
	ldr	r2, [pc, #76]
	movs	r3, #0
	add	r2, sl
	mov	r8, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L25
	adds	r5, r2, #0
	movs	r6, #36
.L27:
	mov	r3, r8
	adds	r3, #6
	cmp	r9, r3
	bne.n	.L26
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #10
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r1, #2
	bl	Func_080b5088
.L26:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L27
	b.n	.L25
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x000000c0
	.4byte 0x00000096
	.4byte 0x03001388
	.4byte 0xffce0000
	.4byte 0x02010000
	.4byte 0x03001e50
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080eeae2
	.4byte 0x03001e80
	.4byte 0x0000031f
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0x080eeaec
	.4byte 0x080eeafa
.L24:
	ldr	r2, [pc, #152]
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L25
	movs	r5, #36
.L29:
	mov	r3, r8
	adds	r3, #6
	cmp	r9, r3
	bne.n	.L28
	mov	r0, sl
	ldr	r3, [r0, r2]
	ldrsh	r0, [r3, r5]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	Func_080d6888
.L28:
	movs	r2, #1
	add	r8, r2
	ldr	r2, [pc, #104]
	mov	r4, sl
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L29
.L25:
	mov	r0, r9
	cmp	r0, #2
	bne.n	.L30
	ldr	r2, [pc, #88]
	movs	r3, #6
	add	r2, sl
	str	r3, [r2, #0]
.L30:
	movs	r1, #16
	movs	r0, #16
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r3, [pc, #72]
	movs	r2, #1
	add	r3, sl
	str	r2, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r4, [sp, #20]
	mov	r2, fp
	movs	r1, #1
	ldrb	r3, [r2, r4]
	add	r9, r1
	cmp	r9, r3
	beq.n	.L10
	b.n	.L31
.L10:
	ldr	r0, [pc, #44]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
