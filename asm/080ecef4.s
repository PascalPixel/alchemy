@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ecef4
	.thumb_func
Func_080ecef4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #56]
	ldmia	r3!, {r7}
	ldr	r2, [pc, #56]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	r8, r3
	adds	r3, r7, r2
	str	r5, [r3, #0]
	movs	r0, #0
	sub	sp, #16
	adds	r6, r1, #0
	bl	Func_080cd594
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	cmp	r6, #0
	bne.n	.L0
	ldr	r0, [pc, #32]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #24]
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000050
	.4byte 0x0000004f
	.4byte 0x00000050
.L0:
	cmp	r6, #1
	bne.n	.L2
	ldr	r0, [pc, #380]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #372]
.L1:
	ldr	r1, [pc, #376]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	b.n	.L3
.L2:
	ldr	r0, [pc, #368]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #360]
	ldr	r1, [pc, #348]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
.L3:
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r4, [pc, #348]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r1, #144
	adds	r2, r7, r4
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #336]
	lsls	r1, r1, #3
	bl	Func_080041d8
	cmp	r6, #1
	bne.n	.L4
	movs	r3, #128
	movs	r2, #36
	ldrsh	r1, [r5, r2]
	ldr	r0, [r5, #8]
	lsls	r3, r3, #12
	movs	r2, #16
	bl	Func_080b5078
	b.n	.L5
.L4:
	movs	r3, #36
	ldrsh	r1, [r5, r3]
	ldr	r0, [r5, #8]
	movs	r2, #16
	movs	r3, #0
	bl	Func_080b5078
.L5:
	movs	r0, #16
	bl	Func_080030f8
	ldr	r4, [pc, #288]
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L6
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
	b.n	.L7
.L6:
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	Func_080ed408
.L7:
	ldr	r3, [pc, #248]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r0, #212
	str	r3, [sp, #8]
	bl	Func_080f9010
	movs	r5, #0
	movs	r6, #120
.L16:
	cmp	r5, #3
	bgt.n	.L8
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	adds	r1, r7, #0
	b.n	.L9
.L8:
	cmp	r5, #7
	bgt.n	.L10
	movs	r2, #225
	lsls	r2, r2, #6
	adds	r1, r7, r2
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	b.n	.L9
.L10:
	cmp	r5, #11
	bgt.n	.L11
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	ldr	r1, [pc, #164]
.L9:
	movs	r2, #0
	movs	r3, #0
	ldr	r4, [sp, #8]
	bl	Func_080072f4
	b.n	.L12
.L11:
	cmp	r5, #15
	bgt.n	.L12
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	ldr	r1, [pc, #168]
	movs	r2, #0
	movs	r3, #0
	ldr	r4, [sp, #8]
	bl	Func_080072f4
.L12:
	adds	r3, r5, #0
	subs	r3, #16
	cmp	r3, #3
	bhi.n	.L13
	movs	r1, #128
	ldr	r3, [pc, #148]
	mov	r0, r8
	lsls	r1, r1, #7
	ldr	r2, [pc, #148]
	bl	Func_080072f0
.L13:
	cmp	r5, #18
	bne.n	.L14
	movs	r0, #134
	bl	Func_080b50e8
.L14:
	cmp	r5, #20
	bne.n	.L15
	ldr	r3, [pc, #132]
	ldr	r4, [pc, #108]
	adds	r2, r7, r3
	movs	r3, #8
	str	r3, [r2, #0]
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r1, #4
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080b5088
.L15:
	movs	r0, #16
	movs	r1, #16
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r3, [pc, #100]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #21
	bne.n	.L16
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #44]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000004d
	.4byte 0x0000004e
	.4byte 0x02010000
	.4byte 0x0000004b
	.4byte 0x0000004c
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x02013840
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.4byte 0x000077a8
	.4byte 0x00007824
