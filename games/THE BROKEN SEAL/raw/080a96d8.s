@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a96d8
	.thumb_func
Func_080a96d8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #684]
	ldr	r0, [pc, #684]
	ldr	r7, [r3, #0]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	sub	sp, #84
	movs	r2, #0
	str	r2, [sp, #20]
	subs	r0, #18
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r0, [r7, r3]
	bl	Func_08077008
	movs	r3, #10
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #10
	movs	r3, #15
	adds	r0, #48
	bl	Func_080a10d0
	bl	Func_080a9cbc
	adds	r2, r7, #0
	movs	r1, #240
	adds	r2, #72
	movs	r4, #31
.L1:
	ldmia	r2!, {r3}
	cmp	r3, #0
	beq.n	.L0
	strb	r1, [r3, #15]
.L0:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L1
	ldr	r0, [pc, #616]
	bl	Func_08004278
	movs	r6, #24
	ldr	r5, [pc, #612]
	negs	r6, r6
	adds	r0, r5, #0
	ldr	r1, [r7, #36]
	movs	r2, #64
	adds	r3, r6, #0
	adds	r5, #3
	bl	Func_08015080
	ldr	r1, [r7, #36]
	movs	r2, #0
	adds	r3, r6, #0
	adds	r0, r5, #0
	bl	Func_08015080
	bl	Func_080a9cbc
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Func_080a33d4
	movs	r0, #228
	lsls	r0, r0, #1
	movs	r3, #0
	adds	r0, r7, r0
	str	r3, [sp, #16]
	str	r0, [sp, #12]
	b.n	.L2
.L11:
	movs	r0, #112
	bl	Func_080f9010
	movs	r2, #1
	str	r2, [sp, #20]
	str	r2, [sp, #16]
	b.n	.L2
.L12:
	movs	r0, #113
	bl	Func_080f9010
	movs	r3, #1
	negs	r3, r3
	movs	r0, #1
	str	r3, [sp, #20]
	str	r0, [sp, #16]
	b.n	.L2
.L3:
	lsls	r2, r4, #1
	adds	r3, r2, #0
	adds	r3, #216
	mov	r0, r9
	ldrh	r3, [r0, r3]
	add	r1, sp, #52
	strh	r3, [r1, r2]
	adds	r4, #1
.L19:
	cmp	r4, #14
	ble.n	.L3
	ldr	r1, [sp, #12]
	movs	r2, #0
	mov	r0, r9
	bl	Func_080a3ddc
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	ldr	r0, [sp, #12]
	movs	r1, #0
	bl	Func_080a1e38
	add	r3, sp, #24
	mov	r8, r3
	movs	r1, #0
	mov	r0, r8
	bl	Func_080a8b8c
	ldr	r0, [r7, #48]
	bl	Func_08015270
	ldr	r2, [pc, #440]
	adds	r3, r7, r2
	ldrb	r1, [r3, #0]
	movs	r2, #1
	ldr	r0, [r7, #48]
	bl	Func_080a9a5c
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #0
	ldr	r0, [sp, #12]
	bl	Func_080a3e28
	movs	r5, #1
	mov	sl, r5
	mov	fp, r8
	b.n	.L4
.L17:
	cmp	r5, #0
	beq.n	.L5
	ldr	r0, [r7, #44]
	bl	Func_08015278
	mov	r3, sl
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L6
	ldr	r0, [r7, #36]
	movs	r1, #0
	mov	r2, r8
	mov	sl, r5
	bl	Func_080a9598
.L6:
	ldr	r0, [r7, #36]
	movs	r1, #0
	mov	r2, r8
	bl	Func_080a93a4
.L5:
	mov	r0, fp
	ldr	r1, [r0, #16]
	ldr	r3, [r7, #20]
	movs	r6, #1
	lsls	r1, r1, #4
	strb	r6, [r3, #5]
	adds	r1, #52
	movs	r0, #96
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	mov	r2, fp
	add	r3, sp, #32
	ldr	r1, [r2, #20]
	movs	r0, #0
	str	r3, [sp, #0]
	movs	r2, #5
	add	r3, sp, #40
	bl	Func_080a1fd4
	cmp	r0, #1
	bne.n	.L7
	movs	r3, #1
	mov	sl, r3
	movs	r5, #1
.L7:
	cmp	r0, #0
	bne.n	.L8
	movs	r5, #1
.L8:
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L9
	movs	r5, #0
.L9:
	ldr	r2, [pc, #304]
	ldr	r3, [r2, #0]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L10
	b.n	.L11
.L10:
	ldr	r2, [r2, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L12
	ldr	r6, [pc, #288]
	movs	r1, #128
	ldr	r3, [r6, #0]
	lsls	r1, r1, #1
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L13
	ldr	r2, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L4
.L13:
	movs	r0, #111
	str	r1, [sp, #8]
	bl	Func_080f9010
	movs	r5, #28
	ldrsb	r5, [r7, r5]
	movs	r0, #130
	lsls	r0, r0, #2
	lsls	r3, r5, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	movs	r2, #152
	lsls	r2, r2, #2
	mov	r0, r8
	adds	r3, r3, r2
	ldr	r2, [r0, #24]
	strb	r2, [r7, r3]
	ldr	r1, [sp, #8]
	ldr	r3, [r6, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L14
	adds	r5, #1
	b.n	.L15
.L14:
	subs	r5, #1
.L15:
	mov	r0, r9
	add	r1, sp, #52
	adds	r0, #216
	movs	r2, #0
	movs	r4, #14
.L16:
	ldrh	r3, [r2, r1]
	subs	r4, #1
	strh	r3, [r0, #0]
	adds	r2, #2
	adds	r0, #2
	cmp	r4, #0
	bge.n	.L16
	ldr	r2, [pc, #192]
	adds	r3, r7, r2
	ldrb	r1, [r3, #0]
	adds	r0, r5, r1
	bl	Func_080022fc
	movs	r3, #130
	adds	r5, r0, #0
	lsls	r3, r3, #2
	lsls	r2, r5, #1
	adds	r2, r2, r3
	ldrh	r3, [r7, r2]
	ldr	r0, [pc, #148]
	str	r3, [r7, #8]
	ldrh	r1, [r7, r2]
	adds	r3, r7, r0
	strb	r1, [r3, #0]
	strb	r5, [r7, #28]
	adds	r0, r7, #0
	ldrh	r1, [r7, r2]
	bl	Func_080a1804
	b.n	.L2
.L4:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L2
	b.n	.L17
.L2:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L18
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L18
	bl	Func_080a9cbc
	ldr	r0, [r7, #36]
	bl	Func_08015270
	ldr	r0, [pc, #84]
	adds	r3, r7, r0
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	movs	r4, #0
	mov	r9, r0
	b.n	.L19
.L18:
	ldr	r0, [r7, #44]
	bl	Func_08015278
	ldr	r0, [r7, #44]
	bl	Func_08015270
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	Func_08015278
	bl	Func_080a345c
	ldr	r0, [r7, #36]
	bl	Func_08015270
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	Func_080041d8
	ldr	r0, [sp, #20]
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x0000021a
	.4byte 0x080a19a1
	.4byte 0x00000b06
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x00000219
