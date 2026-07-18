@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003650
	.thumb_func
Func_08003650:
	push	{r5, lr}
	ldr	r2, [pc, #184]
	ldr	r3, [pc, #184]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldr	r3, [pc, #180]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	ldr	r3, [pc, #176]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [pc, #172]
	ldr	r1, [pc, #172]
	ldr	r5, [pc, #176]
	bl	Func_08006088
	strh	r0, [r5, #0]
	bl	Func_0800655c
.L0:
	bl	Func_080f9018
	bl	Func_08003adc
	ldr	r5, [pc, #160]
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L1
	ldr	r3, [pc, #156]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L2
	ldr	r2, [pc, #152]
	movs	r1, #224
	adds	r2, #208
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #148]
	lsls	r1, r1, #19
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L2:
	ldr	r3, [pc, #136]
	ldr	r0, [pc, #144]
	ldr	r1, [pc, #144]
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	Func_08003a7c
	movs	r3, #0
	strb	r3, [r5, #0]
.L1:
	ldr	r1, [pc, #136]
	ldr	r2, [r1, #0]
	cmp	r2, #0
	beq.n	.L3
	movs	r3, #0
	str	r3, [r1, #0]
	bl	Func_080072ec
.L3:
	movs	r0, #144
	lsls	r0, r0, #3
	bl	Func_08004420
	ldr	r3, [pc, #116]
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	ldr	r4, [pc, #116]
	adds	r0, r3, #0
	eors	r0, r2
	ldr	r3, [r4, #0]
	adds	r2, r0, #0
	bics	r2, r3
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	str	r2, [r3, #0]
	ldr	r3, [r1, #0]
	orrs	r3, r2
	str	r3, [r1, #0]
	str	r0, [r4, #0]
	cmp	r0, #0
	bne.n	.L4
	ldr	r3, [pc, #96]
	movs	r2, #19
	str	r2, [r3, #0]
	ldr	r3, [pc, #96]
	ldr	r1, [pc, #96]
	str	r0, [r3, #0]
	b.n	.L5
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x03001cb0
	.4byte 0x02002220
	.4byte 0x02002020
	.4byte 0x03001f64
	.4byte 0x03001e44
	.4byte 0x03001d18
	.4byte 0x03001e50
	.4byte 0x040000d4
	.4byte 0x84000100
	.4byte 0x03001ad0
	.4byte 0x04000010
	.4byte 0x84000004
	.4byte 0x03001cfc
	.4byte 0x04000130
	.4byte 0x03001ae8
	.4byte 0x03001c94
	.4byte 0x03001af8
	.4byte 0x03001b00
	.4byte 0x03001b04
	.4byte 0x03001d0c
.L4:
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #80]
	ldr	r3, [r1, #0]
	eors	r3, r2
	ldr	r2, [r4, #0]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L6
	ldr	r3, [pc, #68]
	movs	r2, #1
	negs	r2, r2
	str	r2, [r3, #0]
	ldr	r3, [pc, #64]
	str	r0, [r3, #0]
	b.n	.L5
.L6:
	ldr	r2, [pc, #56]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L5
	subs	r3, #1
	str	r3, [r2, #0]
.L5:
	str	r0, [r1, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	bl	Func_080006fc
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x00000001
	.4byte 0x03001d0c
	.4byte 0x0000ffff
	.4byte 0x03001b00
	.4byte 0x03001b04
	.4byte 0x03001800
	.4byte 0x03001ccc
	.4byte 0x03001d28
