@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f24a0
	.thumb_func
Func_080f24a0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #60]
	ldr	r6, [pc, #52]
	ldr	r7, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r6, [r3, #0]
	ldr	r0, [pc, #52]
	bl	Func_08002f40
	ldr	r3, [pc, #48]
	adds	r4, r0, #0
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #40]
	strh	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	mov	r8, r3
	ldr	r5, [pc, #40]
	add	r4, r8
	adds	r1, r5, #0
	adds	r0, r4, #0
	bl	Func_080053e8
	ldr	r3, [pc, #16]
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001efc
	.4byte 0x00000015
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x84000080
	.4byte 0x02010000
.L0:
	adds	r0, r5, #0
	ldr	r1, [pc, #308]
	ldr	r2, [pc, #308]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #308]
	bl	Func_08002f40
	movs	r1, #160
	adds	r4, r0, #0
	ldr	r3, [pc, #300]
	lsls	r1, r1, #19
	ldr	r2, [pc, #300]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #160
	lsls	r3, r3, #19
	add	r4, r8
	strh	r6, [r3, #0]
	adds	r1, r5, #0
	adds	r0, r4, #0
	bl	Func_080053e8
	movs	r1, #192
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #280]
	lsls	r1, r1, #19
	ldr	r2, [pc, #280]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #276]
	ldr	r1, [pc, #280]
	ldr	r2, [pc, #280]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #280]
	ldr	r1, [pc, #280]
	ldr	r3, [pc, #284]
	movs	r4, #0
.L2:
	movs	r0, #29
.L1:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L1
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #10
	ble.n	.L2
	ldr	r3, [pc, #244]
	movs	r4, #11
.L4:
	movs	r0, #29
.L3:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L3
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #31
	ble.n	.L4
	movs	r3, #150
	ldr	r1, [pc, #204]
	lsls	r3, r3, #1
	movs	r4, #0
.L6:
	movs	r0, #29
.L5:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L5
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #10
	ble.n	.L6
	movs	r3, #0
	movs	r4, #11
.L8:
	movs	r0, #29
.L7:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L7
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #31
	ble.n	.L8
	ldr	r2, [pc, #124]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #40]
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #40]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #96]
	movs	r2, #0
	movs	r4, #3
	b.n	.L9
	.4byte 0x00001f43
	.4byte 0x00001e81
	.4byte 0x000000f0
	.4byte 0x0000009f
	.4byte 0x00001616
	.4byte 0x06010000
	.4byte 0x80000f00
	.4byte 0x00000017
	.4byte 0x040000d4
	.4byte 0x84000080
	.4byte 0x02012940
	.4byte 0x80002760
	.4byte 0x0201a140
	.4byte 0x06004ec0
	.4byte 0x80004ec0
	.4byte 0x000001ff
	.4byte 0x0600f000
	.4byte 0x00000267
	.4byte 0x0000013b
	.4byte 0x0600f800
	.4byte 0x0400000a
	.4byte 0x04000040
	.4byte 0x04000048
	.4byte 0x03001ad0
.L9:
	subs	r4, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r4, #0
	bge.n	.L9
	ldr	r0, [pc, #60]
	movs	r3, #0
	movs	r2, #96
	strh	r2, [r0, #6]
	strh	r2, [r0, #10]
	str	r3, [r7, #8]
	str	r3, [r7, #0]
	str	r3, [r7, #4]
	str	r3, [r7, #12]
	str	r3, [r7, #20]
	str	r3, [r7, #16]
	ldr	r1, [pc, #44]
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #16]
	adds	r1, #64
	strh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	strh	r3, [r1, #0]
	b.n	.L10
	movs	r0, r0
	.4byte 0x00003fbf
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x03001ad0
	.4byte 0x04000010
	.4byte 0x040000d4
	.4byte 0x84000004
	.4byte 0x04000052
.L10:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
