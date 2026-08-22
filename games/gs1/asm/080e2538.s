@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e2538
	.thumb_func
Func_080e2538:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #96]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	sub	sp, #56
	str	r1, [sp, #28]
	ldr	r2, [pc, #88]
	ldr	r3, [r3, #0]
	adds	r1, r1, r2
	str	r3, [sp, #24]
	str	r0, [r1, #0]
	movs	r0, #1
	mov	r8, r1
	bl	Func_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r1, [sp, #28]
	ldr	r0, [pc, #64]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	mov	sl, r3
	movs	r0, #46
	movs	r3, #3
	bl	Func_080ed408
	ldr	r5, [r5, #28]
	str	r5, [sp, #20]
	mov	r4, r8
	ldr	r3, [r4, #0]
	add	r6, sp, #44
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r6, #0
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x0000008a
.L0:
	bl	Func_080e396c
	mov	r3, r8
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #20]
	lsls	r3, r3, #1
	add	r5, sp, #32
	adds	r3, #34
	ldrsh	r0, [r2, r3]
	adds	r1, r5, #0
	bl	Func_080e396c
	ldr	r1, [r6, #0]
	ldr	r3, [r5, #0]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	movs	r3, #64
	ldr	r2, [pc, #720]
	subs	r3, r3, r1
	lsls	r3, r3, #8
	str	r1, [r6, #0]
	str	r3, [r2, #0]
	ldr	r0, [sp, #28]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r3, r0, r1
	mov	r2, sl
	str	r2, [r3, #0]
	ldr	r3, [pc, #704]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #696]
	lsls	r1, r1, #3
	bl	Func_080041d8
	mov	r4, r8
	ldr	r3, [r4, #0]
	ldr	r2, [pc, #688]
	ldr	r3, [r3, #24]
	ldrb	r2, [r2, r3]
	movs	r0, #0
	str	r2, [sp, #16]
	mov	r9, r0
	cmp	r2, #0
	beq.n	.L1
	ldr	r4, [sp, #28]
	movs	r0, #225
	lsls	r0, r0, #7
	ldr	r1, [pc, #672]
	movs	r2, #0
	adds	r3, r4, r0
.L2:
	str	r1, [r3, #4]
	str	r2, [r3, #16]
	movs	r4, #1
	ldr	r0, [sp, #16]
	add	r9, r4
	adds	r3, #28
	cmp	r9, r0
	bne.n	.L2
	movs	r1, #0
	mov	r9, r1
	cmp	r0, #0
	beq.n	.L1
	ldr	r2, [pc, #644]
	movs	r7, #0
	mov	sl, r2
.L4:
	movs	r3, #0
	mov	r8, r3
	movs	r3, #140
	mov	r4, r9
	muls	r4, r3
	ldr	r0, [pc, #632]
	adds	r3, r4, #0
	adds	r3, r7, r3
	mov	r6, sl
	adds	r5, r3, r0
.L3:
	ldr	r2, [pc, #628]
	mov	r1, r9
	ldrsb	r2, [r2, r1]
	ldrb	r3, [r6, #0]
	adds	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldrb	r3, [r6, #1]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r1, #96
	bl	Func_08002304
	subs	r0, #48
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r3, #32
	negs	r3, r3
	lsls	r3, r3, #11
	str	r3, [r5, #16]
	movs	r2, #1
	movs	r3, #32
	str	r3, [r5, #8]
	add	r8, r2
	movs	r3, #0
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r6, #2
	adds	r5, #28
	cmp	r3, #21
	bne.n	.L3
	movs	r4, #224
	ldr	r0, [sp, #16]
	lsls	r4, r4, #1
	add	r9, r2
	adds	r7, r7, r4
	cmp	r9, r0
	bne.n	.L4
.L1:
	ldr	r2, [sp, #16]
	movs	r1, #0
	subs	r2, #1
	mov	fp, r1
	str	r2, [sp, #12]
	ldr	r1, [pc, #532]
	movs	r4, #80
	ldrb	r3, [r1, r2]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L5
	b.n	.L6
.L5:
	ldrb	r3, [r1, r2]
	adds	r3, #48
	cmp	fp, r3
	bne.n	.L7
	movs	r0, #132
	bl	Func_080b50e8
.L7:
	ldr	r1, [sp, #16]
	movs	r0, #0
	mov	r9, r0
	cmp	r1, #0
	bne.n	.L8
	b.n	.L9
.L8:
	ldr	r2, [sp, #28]
	movs	r3, #225
	lsls	r3, r3, #7
	adds	r2, r2, r3
	mov	sl, r2
.L21:
	ldr	r5, [pc, #484]
	mov	r4, r9
	ldrb	r3, [r5, r4]
	adds	r2, r3, #0
	adds	r2, #18
	cmp	fp, r2
	bne.n	.L10
	movs	r0, #134
	bl	Func_080f9010
	ldr	r1, [pc, #468]
	ldr	r0, [sp, #28]
	movs	r3, #4
	adds	r2, r0, r1
	str	r3, [r2, #0]
	mov	r2, r9
	ldrb	r3, [r5, r2]
.L10:
	adds	r3, #18
	cmp	fp, r3
	blt.n	.L11
	mov	r4, r9
	lsls	r4, r4, #2
	movs	r3, #0
	str	r4, [sp, #8]
	mov	r8, r3
.L13:
	ldr	r3, [sp, #8]
	add	r3, r9
	lsls	r3, r3, #2
	add	r3, r9
	add	r3, r8
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r0, [pc, #412]
	lsls	r2, r2, #2
	adds	r7, r2, r0
	movs	r1, #5
	mov	r0, r8
	bl	Func_080022fc
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	ldr	r0, [r7, #24]
	bl	Func_080022ec
	movs	r1, #3
	bl	Func_080022fc
	ldr	r2, [pc, #396]
	adds	r5, r5, r0
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	ldr	r3, [pc, #388]
	adds	r1, r2, r1
	adds	r1, r1, r3
	ldr	r3, [pc, #388]
	ldrb	r6, [r3, r5]
	movs	r4, #2
	ldrsh	r2, [r7, r4]
	lsrs	r3, r6, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldr	r0, [pc, #376]
	ldrb	r4, [r0, r5]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	str	r6, [sp, #0]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	bl	Func_080072f4
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r0, r7, #0
	movs	r1, #64
	bl	Func_080e3908
	ldr	r3, [r7, #24]
	ldr	r2, [r7, #8]
	adds	r3, r3, r2
	str	r3, [r7, #24]
	cmp	r2, #1
	ble.n	.L12
	movs	r3, #1
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L12
	subs	r3, r2, #1
	str	r3, [r7, #8]
.L12:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #21
	bne.n	.L13
	ldr	r5, [pc, #288]
	mov	r4, r9
	ldrb	r3, [r5, r4]
	adds	r3, #18
	cmp	fp, r3
	bge.n	.L14
	b.n	.L15
.L11:
	ldr	r5, [pc, #272]
.L15:
	mov	r0, r9
	ldrb	r3, [r5, r0]
	cmp	fp, r3
	blt.n	.L16
	ldr	r3, [pc, #260]
	mov	r4, sl
	ldrsb	r2, [r3, r0]
	movs	r1, #6
	ldrsh	r3, [r4, r1]
	movs	r1, #34
	str	r1, [sp, #0]
	movs	r1, #62
	str	r1, [sp, #4]
	adds	r2, #47
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #28]
	ldr	r4, [sp, #20]
	bl	Func_080072f4
.L16:
	mov	r0, sl
	ldr	r3, [r0, #4]
	ldr	r2, [r0, #16]
	mov	r1, r9
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldrb	r3, [r5, r1]
	cmp	fp, r3
	ble.n	.L17
	movs	r4, #128
	lsls	r4, r4, #9
	adds	r3, r2, r4
	str	r3, [r0, #16]
.L17:
	mov	r0, sl
	movs	r2, #200
	ldr	r3, [r0, #4]
	lsls	r2, r2, #14
	cmp	r3, r2
	ble.n	.L18
	str	r2, [r0, #4]
.L18:
	ldr	r5, [pc, #192]
.L14:
	mov	r1, r9
	ldrb	r3, [r5, r1]
	adds	r3, #18
	cmp	fp, r3
	bne.n	.L19
	ldr	r3, [pc, #204]
	ldr	r4, [sp, #28]
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #20]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L19
	ldr	r0, [pc, #188]
	movs	r6, #36
	adds	r5, r4, r0
.L20:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	movs	r1, #7
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L20
.L19:
	movs	r4, #1
	ldr	r0, [sp, #16]
	movs	r3, #28
	add	r9, r4
	add	sl, r3
	cmp	r9, r0
	beq.n	.L9
	b.n	.L21
.L9:
	movs	r0, #2
	movs	r1, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r3, [pc, #128]
	ldr	r1, [sp, #28]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #84]
	ldr	r2, [sp, #12]
	ldrb	r3, [r1, r2]
	movs	r4, #1
	add	fp, r4
	adds	r3, #80
	cmp	fp, r3
	beq.n	.L6
	b.n	.L5
.L6:
	ldr	r0, [pc, #40]
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
	.4byte 0x04000028
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080eecfc
	.4byte 0xffc00000
	.4byte 0x080eecb2
	.4byte 0x02010000
	.4byte 0x080eecf2
	.4byte 0x080eecf7
	.4byte 0x000077a8
	.4byte 0x080eed1e
	.4byte 0x0000083c
	.4byte 0x080eecff
	.4byte 0x080eed0e
	.4byte 0x00007828
	.4byte 0x00007824
