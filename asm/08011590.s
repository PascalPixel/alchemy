@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011590
	.thumb_func
Func_08011590:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #96]
	ldmia	r3!, {r5}
	ldr	r7, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #252
	movs	r6, #1
	strb	r6, [r3, #0]
	ldr	r0, [pc, #84]
	bl	Func_080042c8
	ldr	r3, [pc, #84]
	ldr	r0, [pc, #84]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #88]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #80]
	ldr	r2, [r3, #0]
	ands	r2, r6
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #10
	adds	r5, r5, r3
	movs	r3, #200
	lsls	r3, r3, #4
	adds	r5, r5, r3
	ldr	r1, [pc, #64]
	adds	r0, r5, #0
	bl	Func_08012388
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #200
	strh	r3, [r2, #0]
	adds	r3, #58
	adds	r2, r7, r3
	movs	r3, #255
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #44]
	str	r3, [r2, #0]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e6c
	.4byte 0x0801179d
	.4byte 0x040000d4
	.4byte 0x06004000
	.4byte 0x0201c000
	.4byte 0x84000800
	.4byte 0x03001e40
	.4byte 0x02010000
	.4byte 0x03001cfc
	.4byte 0x08011569
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #20]
	strh	r2, [r3, #0]
	adds	r3, #202
	ldr	r0, [pc, #16]
	ldr	r1, [pc, #20]
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x00000501
	.4byte 0x0400000a
	.4byte 0x02038000
	.4byte 0x06008000
	.4byte 0x84002000
