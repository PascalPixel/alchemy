@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080aad10
	.thumb_func
Func_080aad10:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #180]
	movs	r2, #194
	ldr	r0, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	sub	sp, #8
	mov	r8, r3
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r3, #30
	movs	r2, #5
	adds	r0, #48
	bl	Func_080a10d0
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, r8
	movs	r2, #128
	ldr	r6, [pc, #136]
	ldr	r1, [pc, #140]
	lsls	r2, r2, #6
	adds	r0, #168
	bl	Func_080072fc
	ldr	r0, [pc, #132]
	ldr	r1, [pc, #136]
	add	r0, r8
	movs	r2, #128
	bl	Func_080072fc
	movs	r1, #128
	ldr	r5, [pc, #128]
	lsls	r1, r1, #6
	ldr	r2, [pc, #128]
	ldr	r0, [pc, #108]
	bl	Func_080072f8
	movs	r1, #128
	ldr	r2, [pc, #120]
	ldr	r0, [pc, #108]
	bl	Func_080072f8
	ldr	r0, [pc, #116]
	bl	Func_080153d8
	ldr	r1, [pc, #116]
	movs	r2, #32
	ldr	r0, [pc, #116]
	bl	Func_080072fc
	bl	Func_080045e8
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #112]
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #112]
	ldr	r2, [pc, #112]
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #112]
	strh	r3, [r2, #0]
	adds	r1, #64
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #92]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #8
	bl	Func_080aac84
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #96]
	strh	r3, [r2, #0]
	ldrh	r3, [r5, #0]
	subs	r2, #32
	strh	r3, [r2, #0]
	mov	r0, r8
	bl	Func_080aafb8
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x000020a8
	.4byte 0x05000080
	.4byte 0x03000168
	.4byte 0x33333333
	.4byte 0x55555555
	.4byte 0x06005000
	.4byte 0x080af26c
	.4byte 0x060052c0
	.4byte 0x040000d4
	.4byte 0x050000a0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x050000bc
	.4byte 0x050001e0
	.4byte 0x050000e8
