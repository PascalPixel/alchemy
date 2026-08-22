@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f2028
	.thumb_func
Func_080f2028:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #416]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #416]
	ldrb	r3, [r3, #0]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L0
	ldr	r2, [r6, #12]
	movs	r3, #3
	adds	r2, #1
	ands	r3, r2
	str	r2, [r6, #12]
	cmp	r3, #0
	bne.n	.L0
	ldr	r3, [r6, #20]
	adds	r3, #1
	str	r3, [r6, #20]
.L0:
	ldr	r3, [pc, #388]
	ldrh	r2, [r3, #6]
	movs	r3, #48
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	mov	sl, r3
	movs	r3, #144
	subs	r1, r3, r2
	movs	r2, #140
	ldr	r3, [r6, #8]
	lsls	r2, r2, #1
	cmp	r3, r2
	blt.n	.L1
	b.n	.L2
.L1:
	ldr	r3, [r6, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	b.n	.L4
.L3:
	ldr	r3, [pc, #352]
	mov	r2, sl
	ldrb	r3, [r3, #0]
	subs	r2, r1, r2
	adds	r7, r3, #0
	mov	r9, r2
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	Func_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L5
	ldr	r3, [pc, #320]
.L6:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L6
.L5:
	cmp	r5, #0
	bge.n	.L7
	movs	r3, #128
	lsls	r3, r3, #1
.L8:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L8
.L7:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	adds	r3, r0, #0
	ldr	r2, [pc, #296]
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #20
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #280]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	adds	r2, #16
	movs	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #268]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #264]
	movs	r4, #40
	orrs	r1, r2
	str	r0, [r6, r4]
	movs	r2, #232
	orrs	r1, r3
	movs	r0, #48
	movs	r3, #28
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	movs	r3, #36
	str	r2, [r6, r3]
	movs	r3, #44
	str	r2, [r6, r3]
	movs	r3, #52
	str	r2, [r6, r3]
	ldr	r3, [pc, #212]
	ldrb	r3, [r3, #2]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	Func_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L9
	ldr	r3, [pc, #188]
.L10:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L10
.L9:
	cmp	r5, #0
	bge.n	.L11
	movs	r3, #128
	lsls	r3, r3, #1
.L12:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L12
.L11:
	adds	r3, r7, #4
	ldr	r2, [pc, #184]
	lsls	r3, r3, #16
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #56
	str	r3, [r6, r1]
	movs	r2, #60
	movs	r3, #128
	str	r3, [r6, r2]
	movs	r3, #5
	mov	r8, r3
	ldr	r3, [pc, #136]
	ldrb	r3, [r3, #4]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	Func_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #32
	subs	r7, #32
	cmp	r5, #255
	ble.n	.L13
	ldr	r3, [pc, #112]
.L14:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L14
.L13:
	cmp	r5, #0
	bge.n	.L15
	movs	r3, #128
	lsls	r3, r3, #1
.L16:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L16
.L15:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	mov	r1, r8
	adds	r3, r0, #0
	ldr	r2, [pc, #100]
	lsls	r1, r1, #3
	orrs	r3, r5
	mov	ip, r1
	orrs	r3, r2
	adds	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #36
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #84]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	mov	r4, ip
	adds	r2, #32
	adds	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #68]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #64]
	orrs	r1, r2
	adds	r4, #8
	str	r0, [r6, r4]
	orrs	r1, r3
	mov	r0, ip
	mov	r3, ip
	movs	r2, #192
	b.n	.L17
	movs	r0, r0
	.4byte 0x03001efc
	.4byte 0x03001d20
	.4byte 0x03001ad0
	.4byte 0x080f39ab
	.4byte 0xffffff00
	.4byte 0x40002400
	.4byte 0x50002400
	.4byte 0x60002400
	.4byte 0x70002400
	.4byte 0x80002400
	.4byte 0x90002400
	.4byte 0xa0002400
	.4byte 0xb0002400
.L4:
	ldr	r3, [pc, #504]
	mov	r2, sl
	ldrb	r3, [r3, #1]
	subs	r2, r1, r2
	adds	r7, r3, #0
	mov	r9, r2
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	Func_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L18
	ldr	r3, [pc, #472]
.L19:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L19
.L18:
	cmp	r5, #0
	bge.n	.L20
	movs	r3, #128
	lsls	r3, r3, #1
.L21:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L21
.L20:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	adds	r3, r0, #0
	ldr	r2, [pc, #448]
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #20
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #436]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	adds	r2, #16
	movs	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #420]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #420]
	movs	r4, #40
	orrs	r1, r2
	str	r0, [r6, r4]
	movs	r2, #232
	orrs	r1, r3
	movs	r0, #48
	movs	r3, #28
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	movs	r3, #36
	str	r2, [r6, r3]
	movs	r3, #44
	str	r2, [r6, r3]
	movs	r3, #52
	str	r2, [r6, r3]
	ldr	r3, [pc, #368]
	ldrb	r3, [r3, #3]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	Func_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L22
	ldr	r3, [pc, #340]
.L23:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L23
.L22:
	cmp	r5, #0
	bge.n	.L24
	movs	r3, #128
	lsls	r3, r3, #1
.L25:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L25
.L24:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	adds	r3, r0, #0
	ldr	r2, [pc, #316]
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #56
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #20
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #304]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	adds	r2, #16
	movs	r4, #64
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #288]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #288]
	movs	r4, #72
	orrs	r1, r2
	str	r0, [r6, r4]
	movs	r2, #224
	orrs	r1, r3
	movs	r0, #80
	movs	r3, #60
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	movs	r3, #68
	str	r2, [r6, r3]
	movs	r3, #76
	str	r2, [r6, r3]
	movs	r3, #84
	str	r2, [r6, r3]
	movs	r3, #8
	mov	r8, r3
	ldr	r3, [pc, #232]
	ldrb	r3, [r3, #5]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	Func_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #32
	subs	r7, #32
	cmp	r5, #255
	ble.n	.L26
	ldr	r3, [pc, #204]
.L27:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L27
.L26:
	cmp	r5, #0
	bge.n	.L28
	movs	r3, #128
	lsls	r3, r3, #1
.L29:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L29
.L28:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	mov	r1, r8
	adds	r3, r0, #0
	ldr	r2, [pc, #196]
	lsls	r1, r1, #3
	orrs	r3, r5
	mov	ip, r1
	orrs	r3, r2
	adds	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #36
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #176]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	mov	r4, ip
	adds	r2, #32
	adds	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #160]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #160]
	orrs	r1, r2
	adds	r4, #8
	str	r0, [r6, r4]
	orrs	r1, r3
	mov	r0, ip
	mov	r3, ip
	movs	r2, #160
.L17:
	adds	r3, #28
	adds	r0, #48
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	adds	r3, #8
	str	r2, [r6, r3]
	adds	r3, #8
	str	r2, [r6, r3]
	adds	r3, #8
	str	r2, [r6, r3]
	movs	r2, #4
	add	r8, r2
.L2:
	mov	r3, r8
	cmp	r3, #119
	bhi.n	.L30
	lsls	r3, r3, #3
	ldr	r2, [pc, #116]
	adds	r3, #24
.L31:
	movs	r1, #1
	add	r8, r1
	mov	r1, r8
	str	r2, [r6, r3]
	adds	r3, #8
	cmp	r1, #119
	bls.n	.L31
.L30:
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	mov	r2, r8
	lsls	r5, r2, #3
	adds	r7, r6, #0
	movs	r4, #132
	lsrs	r2, r5, #2
	lsls	r4, r4, #24
	adds	r7, #24
	movs	r1, #224
	ldr	r3, [pc, #76]
	adds	r0, r7, #0
	lsls	r1, r1, #19
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #68]
	ldrh	r2, [r3, #6]
	movs	r3, #32
	subs	r2, r3, r2
	b.n	.L32
	movs	r0, r0
	.4byte 0x00003f50
	.4byte 0x00000e0e
	.4byte 0x080f39ab
	.4byte 0xffffff00
	.4byte 0x40002400
	.4byte 0x50002400
	.4byte 0x60002400
	.4byte 0x70002400
	.4byte 0x80002400
	.4byte 0x90002400
	.4byte 0xa0002400
	.4byte 0xb0002400
	.4byte 0x400020a0
	.4byte 0x04000050
	.4byte 0x040000d4
	.4byte 0x03001ad0
.L32:
	cmp	r2, #255
	ble.n	.L33
	ldr	r3, [pc, #72]
.L34:
	adds	r2, r2, r3
	cmp	r2, #255
	bgt.n	.L34
.L33:
	cmp	r2, #0
	bge.n	.L35
	movs	r3, #128
	lsls	r3, r3, #1
.L36:
	adds	r2, r2, r3
	cmp	r2, #0
	blt.n	.L36
.L35:
	ldr	r3, [pc, #56]
	movs	r4, #224
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #4
	lsls	r4, r4, #19
	adds	r0, r6, #0
	str	r2, [r6, #120]
	adds	r0, #120
	str	r3, [r6, #124]
	adds	r1, r5, r4
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r7, #0
	adds	r1, r4, #0
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xffffff00
	.4byte 0xc05c2000
	.4byte 0x040000d4
	.4byte 0x84000002
	.4byte 0x84000008
