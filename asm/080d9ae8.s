@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d9ae8
	.thumb_func
Func_080d9ae8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r1, [sp, #52]
	ldr	r3, [pc, #52]
	ldmia	r3!, {r1}
	str	r1, [sp, #48]
	ldr	r2, [pc, #48]
	ldr	r3, [r3, #0]
	str	r3, [sp, #44]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L0
	ldr	r0, [pc, #28]
	ldr	r1, [sp, #48]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x0000009c
.L0:
	ldr	r0, [pc, #680]
	ldr	r1, [sp, #48]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
.L1:
	ldr	r5, [sp, #52]
	cmp	r5, #0
	beq.n	.L2
	ldr	r0, [sp, #52]
	cmp	r0, #1
	bne.n	.L2
	ldr	r0, [pc, #660]
	b.n	.L3
.L2:
	ldr	r0, [pc, #660]
.L3:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #656]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r3, #150
	ldr	r2, [sp, #48]
	lsls	r3, r3, #6
	adds	r1, r2, r3
	ldr	r0, [pc, #640]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	movs	r0, #174
	movs	r5, #1
	lsls	r0, r0, #2
	mov	r9, r5
	mov	lr, r0
	movs	r4, #57
.L7:
	ldr	r3, [sp, #48]
	movs	r2, #150
	movs	r1, #0
	lsls	r2, r2, #6
	str	r1, [sp, #36]
	adds	r1, r3, r2
	adds	r3, r0, r3
	mov	ip, r4
	adds	r3, r3, r2
.L6:
	ldrb	r2, [r1, #0]
	adds	r1, #1
	cmp	r2, ip
	ble.n	.L4
	mov	r2, ip
.L4:
	cmp	r2, #0
	bge.n	.L5
	movs	r2, #0
.L5:
	strb	r2, [r3, #0]
	ldr	r5, [sp, #36]
	adds	r5, #1
	adds	r3, #1
	str	r5, [sp, #36]
	cmp	r5, lr
	bne.n	.L6
	movs	r2, #1
	movs	r1, #174
	add	r9, r2
	lsls	r1, r1, #2
	mov	r3, r9
	adds	r0, r0, r1
	subs	r4, #7
	cmp	r3, #8
	bne.n	.L7
	ldr	r5, [sp, #48]
	ldr	r0, [pc, #556]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L8
	ldr	r2, [pc, #548]
	ldr	r3, [pc, #548]
	movs	r1, #112
	negs	r1, r1
	str	r3, [r2, #0]
	str	r1, [sp, #32]
	b.n	.L9
.L8:
	ldr	r2, [pc, #532]
	movs	r3, #0
	str	r3, [r2, #0]
	str	r3, [sp, #32]
.L9:
	movs	r2, #0
	ldr	r5, [pc, #532]
	mov	r9, r2
	movs	r7, #192
.L18:
	bl	Func_08004458
	ldr	r3, [pc, #524]
	adds	r6, r0, #0
	ands	r6, r3
	movs	r3, #0
	str	r3, [r5, #0]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L10
	movs	r2, #31
	mov	r0, r9
	ands	r2, r0
	cmp	r2, #0
	bge.n	.L11
	adds	r2, #3
.L11:
	asrs	r2, r2, #2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #496]
	lsls	r3, r3, #17
	adds	r3, r3, r1
	str	r3, [r5, #4]
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L12
	adds	r3, #3
.L12:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	r2, r9
	subs	r3, r2, r3
	ldr	r0, [pc, #476]
	lsls	r3, r3, #17
	adds	r3, r3, r0
	b.n	.L13
.L10:
	movs	r2, #31
	mov	r1, r9
	ands	r2, r1
	cmp	r2, #0
	bge.n	.L14
	adds	r2, #3
.L14:
	asrs	r2, r2, #2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #444]
	lsls	r3, r3, #17
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L15
	adds	r3, #3
.L15:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	r0, r9
	subs	r3, r0, r3
	ldr	r1, [pc, #428]
	lsls	r3, r3, #19
	adds	r3, r3, r1
.L13:
	str	r3, [r5, #8]
	ldr	r2, [sp, #48]
	ldr	r0, [pc, #392]
	adds	r3, r2, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L16
	movs	r3, #128
	lsls	r3, r3, #10
	b.n	.L17
.L16:
	ldr	r3, [pc, #396]
.L17:
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r7, #0
	muls	r3, r0
	movs	r1, #128
	lsls	r1, r1, #9
	asrs	r3, r3, #6
	adds	r3, r3, r1
	str	r3, [r5, #16]
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r3, r7, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r5, #20]
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	str	r3, [r5, #24]
	movs	r2, #1
	movs	r3, #128
	add	r9, r2
	lsls	r3, r3, #2
	adds	r5, #28
	cmp	r9, r3
	bne.n	.L18
	ldr	r0, [sp, #48]
	ldr	r1, [pc, #308]
	adds	r5, r0, r1
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #56
	ldr	r0, [r3, #4]
	adds	r1, r2, #0
	str	r2, [sp, #28]
	bl	Func_080cef64
	movs	r0, #239
	ldr	r3, [sp, #48]
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #48]
	ldr	r3, [pc, #308]
	adds	r2, r1, r3
	movs	r3, #50
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #300]
	bl	Func_080041d8
	movs	r0, #0
	str	r0, [sp, #40]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	movs	r1, #64
	lsls	r3, r3, #2
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L19
	b.n	.L20
.L19:
	ldr	r3, [pc, #276]
	ldr	r2, [sp, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #24]
	cmp	r2, #72
	bne.n	.L21
	movs	r0, #0
	bl	Func_080b50e8
.L21:
	movs	r3, #0
	str	r3, [sp, #36]
	ldr	r2, [pc, #216]
	ldr	r5, [sp, #48]
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L22
	b.n	.L23
.L22:
	ldr	r0, [sp, #40]
	ldr	r3, [sp, #24]
	subs	r0, #24
	ldr	r1, [sp, #40]
	adds	r3, #12
	movs	r5, #0
	str	r0, [sp, #12]
	str	r3, [sp, #20]
	str	r5, [sp, #8]
	mov	r8, r1
.L36:
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #36]
	ldr	r2, [r0, r2]
	lsls	r3, r1, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	ldr	r6, [r0, #0]
	mov	r0, r8
	cmp	r0, #0
	bgt.n	.L24
	b.n	.L25
.L24:
	bl	Func_080049ac
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	bl	Func_080051d8
	ldr	r3, [r6, #8]
	add	r5, sp, #64
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	Func_080049ac
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	bl	Func_080051d8
	adds	r0, r5, #0
	bl	Func_08004cb4
	movs	r3, #0
	add	r0, sp, #88
	add	r5, sp, #76
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	str	r3, [r0, #8]
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #32]
	ldr	r2, [r5, #4]
	adds	r6, r3, r1
	ldr	r3, [sp, #52]
	mov	sl, r2
	cmp	r3, #0
	bne.n	.L26
	mov	r0, r8
	cmp	r0, #26
	bgt.n	.L27
	cmp	r0, #0
	bge.n	.L28
	adds	r0, #3
.L28:
	movs	r1, #7
	asrs	r0, r0, #2
	bl	Func_080022fc
	lsls	r1, r0, #4
	subs	r1, r1, r0
	ldr	r2, [sp, #48]
	movs	r0, #24
	lsls	r1, r1, #6
	adds	r1, r2, r1
	mov	r3, sl
	adds	r2, r6, #0
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #20
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	bl	Func_080072f4
	b.n	.L27
	movs	r0, r0
	.4byte 0x0000009b
	.4byte 0x000000b7
	.4byte 0x000000bb
	.4byte 0x03001388
	.4byte 0x0000009d
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xffff9000
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0xfff60000
	.4byte 0xfffe0000
	.4byte 0xfff00000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
.L26:
	mov	r3, r8
	cmp	r3, #23
	bgt.n	.L27
	mov	r0, r8
	cmp	r3, #0
	bge.n	.L29
	adds	r0, #3
.L29:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	Func_080022fc
	lsls	r1, r0, #1
	adds	r1, r1, r0
	lsls	r1, r1, #3
	adds	r1, r1, r0
	ldr	r0, [sp, #48]
	lsls	r1, r1, #6
	adds	r1, r0, r1
	movs	r0, #40
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #28]
	adds	r2, r6, #0
	mov	r3, sl
	ldr	r4, [r0, #4]
	subs	r2, #20
	subs	r3, #20
	ldr	r0, [sp, #44]
	bl	Func_080072f4
.L27:
	mov	r1, r8
	cmp	r1, #24
	bne.n	.L30
	movs	r0, #143
	bl	Func_080f9010
.L30:
	ldr	r2, [sp, #12]
	cmp	r2, #36
	bhi.n	.L25
	mov	r3, r8
	movs	r1, #0
	cmp	r3, #28
	ble.n	.L31
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L32
	mov	r3, r8
	subs	r3, #21
.L32:
	asrs	r1, r3, #2
	cmp	r1, #7
	ble.n	.L31
	movs	r1, #7
.L31:
	movs	r3, #174
	lsls	r3, r3, #2
	adds	r2, r1, #0
	muls	r2, r3
	mov	fp, r5
	ldr	r3, [sp, #8]
	ldr	r5, [pc, #264]
	movs	r0, #0
	str	r2, [sp, #16]
	mov	r9, r0
	adds	r7, r3, r5
.L35:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L33
	adds	r3, #3
.L33:
	asrs	r3, r3, #2
	mov	r0, r9
	lsls	r3, r3, #2
	subs	r3, r0, r3
	lsls	r2, r3, #1
	adds	r6, r2, r3
	ldr	r3, [r7, #24]
	mov	r1, r8
	adds	r0, r3, r1
	cmp	r0, #0
	bge.n	.L34
	adds	r0, #7
.L34:
	movs	r1, #3
	asrs	r0, r0, #3
	bl	Func_080022fc
	mov	r1, fp
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080e3944
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r0, [sp, #32]
	ldr	r1, [r2, #4]
	adds	r5, r6, r5
	ldr	r2, [pc, #200]
	adds	r6, r3, r0
	lsls	r3, r5, #1
	mov	sl, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #48]
	adds	r1, r2, r1
	adds	r1, r3, r1
	ldr	r3, [pc, #184]
	ldrb	r3, [r3, r5]
	str	r3, [sp, #0]
	ldr	r3, [pc, #184]
	movs	r0, #150
	ldrb	r3, [r3, r5]
	lsls	r0, r0, #6
	adds	r1, r1, r0
	str	r3, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #44]
	adds	r2, r6, #0
	mov	r3, sl
	bl	Func_080072f4
	adds	r0, r7, #0
	movs	r1, #60
	movs	r2, #0
	bl	Func_080e38b8
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r7, #28
	cmp	r2, #24
	bne.n	.L35
.L25:
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #8]
	ldr	r2, [sp, #36]
	movs	r1, #224
	movs	r5, #4
	lsls	r1, r1, #2
	subs	r3, #4
	negs	r5, r5
	adds	r0, r0, r1
	adds	r2, #1
	str	r2, [sp, #36]
	str	r0, [sp, #8]
	str	r3, [sp, #12]
	add	r8, r5
	ldr	r2, [pc, #112]
	ldr	r5, [sp, #48]
	ldr	r3, [r5, r2]
	ldr	r0, [sp, #36]
	ldr	r3, [r3, #20]
	cmp	r0, r3
	beq.n	.L23
	b.n	.L36
.L23:
	ldr	r1, [sp, #48]
	ldr	r3, [pc, #100]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r5, [sp, #40]
	ldr	r0, [sp, #48]
	adds	r5, #1
	ldr	r1, [pc, #76]
	str	r5, [sp, #40]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #2
	adds	r3, #64
	cmp	r5, r3
	beq.n	.L20
	b.n	.L19
.L20:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x080eea08
	.4byte 0x080eea20
	.4byte 0x080eea2c
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
