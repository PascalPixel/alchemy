@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080dd77c
	.thumb_func
Func_080dd77c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #112]
	mov	r8, r1
	mov	r3, r8
	ldmia	r3!, {r2}
	ldr	r5, [pc, #108]
	ldr	r3, [r3, #0]
	sub	sp, #48
	mov	sl, r2
	add	r5, sl
	mov	fp, r3
	add	r3, sp, #20
	ldr	r2, [r0, #4]
	str	r0, [r5, #0]
	str	r3, [sp, #0]
	add	r3, sp, #16
	str	r3, [sp, #4]
	movs	r1, #4
	movs	r3, #4
	bl	Func_080de2f8
	movs	r0, #1
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #48
	strh	r3, [r2, #0]
	mov	r1, sl
	movs	r2, #1
	ldr	r0, [pc, #60]
	movs	r3, #1
	bl	Func_080e0524
	ldr	r3, [r5, #0]
	add	r6, sp, #36
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r6, #0
	bl	Func_080e396c
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #20]
	lsls	r3, r3, #1
	add	r5, sp, #24
	adds	r3, #34
	ldrsh	r0, [r2, r3]
	adds	r1, r5, #0
	bl	Func_080e396c
	ldr	r1, [r6, #0]
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000a6
.L0:
	ldr	r3, [r5, #0]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	movs	r3, #64
	ldr	r2, [pc, #392]
	subs	r3, r3, r1
	lsls	r3, r3, #8
	str	r1, [r6, #0]
	movs	r5, #1
	str	r3, [r2, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	mov	r2, r8
	ldr	r3, [r2, #28]
	movs	r1, #7
	str	r3, [sp, #8]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	mov	r0, r8
	ldr	r3, [r0, #32]
	add	r1, sp, #8
	movs	r2, #225
	mov	r9, r1
	str	r3, [r1, #4]
	lsls	r2, r2, #7
	ldr	r1, [pc, #336]
	movs	r6, #0
	add	r2, sl
.L1:
	ldrb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #64
	adds	r6, #1
	str	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #28
	cmp	r6, #16
	bne.n	.L1
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #304]
	movs	r3, #0
	add	r2, sl
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #296]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r2, #0
	mov	r8, r2
.L10:
	mov	r3, r8
	cmp	r3, #32
	bne.n	.L2
	movs	r0, #143
	bl	Func_080f9010
	ldr	r3, [pc, #276]
	mov	r0, sl
	ldr	r3, [r0, r3]
	ldr	r3, [r3, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L2
	ldr	r5, [pc, #264]
	movs	r7, #36
	add	r5, sl
.L3:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r6, #0
	movs	r2, #5
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r6, #1
	adds	r7, #2
	cmp	r6, r3
	bne.n	.L3
.L2:
	movs	r7, #225
	lsls	r7, r7, #7
	movs	r6, #0
	add	r7, sl
.L9:
	lsls	r3, r6, #2
	adds	r3, #5
	cmp	r8, r3
	bne.n	.L4
	ldr	r2, [pc, #216]
	movs	r3, #2
	add	r2, sl
	str	r3, [r2, #0]
.L4:
	lsls	r5, r6, #1
	adds	r3, r5, #4
	cmp	r8, r3
	ble.n	.L5
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L6
	adds	r0, #3
.L6:
	asrs	r0, r0, #2
	adds	r0, r0, r6
	movs	r1, #5
	bl	Func_080022fc
	adds	r3, r5, #0
	adds	r3, #32
	cmp	r8, r3
	bge.n	.L7
	mov	r2, r8
	subs	r3, r2, r5
	lsls	r3, r3, #2
	adds	r5, r3, #0
	subs	r5, #16
	cmp	r5, #32
	ble.n	.L8
	movs	r5, #32
	b.n	.L8
.L7:
	mov	r1, r8
	subs	r3, r1, r5
	lsls	r3, r3, #2
	movs	r2, #160
	subs	r5, r2, r3
.L8:
	cmp	r5, #0
	ble.n	.L5
	lsls	r1, r0, #10
	movs	r4, #1
	movs	r0, #32
	movs	r3, #7
	ldr	r2, [r7, #0]
	ands	r4, r6
	str	r0, [sp, #0]
	ands	r3, r6
	str	r5, [sp, #4]
	mov	r0, r9
	lsls	r4, r4, #2
	subs	r3, r3, r5
	ldr	r4, [r4, r0]
	add	r1, sl
	subs	r2, #16
	adds	r3, #104
	mov	r0, fp
	bl	Func_080072f4
.L5:
	adds	r6, #1
	adds	r7, #28
	cmp	r6, #16
	bne.n	.L9
	movs	r1, #4
	movs	r0, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #88]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #70
	bne.n	.L10
	ldr	r0, [pc, #52]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #48
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
	.4byte 0x080eeb96
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
