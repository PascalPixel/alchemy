@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e7404
	.thumb_func
Func_080e7404:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #284
	str	r1, [sp, #76]
	ldr	r3, [pc, #96]
	mov	r2, sp
	ldr	r1, [r3, #0]
	adds	r2, #148
	str	r2, [sp, #60]
	str	r1, [sp, #72]
	subs	r3, #4
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #84]
	str	r3, [r2, #0]
	adds	r3, r3, r4
	str	r0, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L0
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #52]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	movs	r3, #160
	ldr	r2, [r0, #0]
	lsls	r3, r3, #12
	str	r3, [r2, #40]
	ldr	r3, [pc, #40]
	str	r3, [r2, #72]
	ldr	r4, [sp, #60]
	ldr	r0, [pc, #24]
	ldr	r3, [r4, #0]
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	movs	r5, #1
	ldr	r0, [r3, #8]
	negs	r5, r5
	movs	r3, #0
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000091eb
.L1:
	str	r3, [sp, #0]
	adds	r1, r5, #0
	movs	r2, #2
	adds	r3, r5, #0
	bl	Func_080d6888
	movs	r0, #145
	bl	Func_080f9010
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #20]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	ldr	r4, [sp, #76]
	ldr	r3, [r3, #4]
	str	r4, [sp, #64]
	cmp	r3, #1
	beq.n	.L2
	str	r5, [sp, #64]
	b.n	.L2
	movs	r0, r0
	.4byte 0x00007828
.L0:
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #64]
.L2:
	bl	Func_080c9048
	ldr	r2, [pc, #56]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r1, [sp, #60]
	movs	r2, #239
	ldr	r3, [r1, #0]
	ldr	r5, [pc, #40]
	lsls	r2, r2, #7
	adds	r3, r3, r2
	movs	r1, #144
	movs	r2, #0
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl	Func_080041d8
	movs	r0, #0
	movs	r1, #0
	bl	Func_080cd104
	adds	r0, r5, #0
	bl	Func_08004278
	ldr	r3, [sp, #76]
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x080cd261
.L3:
	cmp	r3, #1
	bne.n	.L4
	movs	r4, #0
	ldr	r5, [pc, #68]
	ldr	r6, [pc, #68]
	mov	r8, r4
.L6:
	adds	r0, r6, #0
	bl	Func_08009030
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #0]
	str	r0, [r3, r5]
	cmp	r0, #0
	beq.n	.L5
	adds	r2, r0, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #2
	bl	Func_08009020
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	ldr	r1, [r3, r5]
	ldrb	r3, [r1, #9]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #9]
.L5:
	movs	r4, #1
	ldr	r3, [pc, #24]
	add	r8, r4
	mov	r0, r8
	adds	r5, #4
	adds	r6, r6, r3
	cmp	r0, #2
	bne.n	.L6
	b.n	.L7
	movs	r0, r0
	.4byte 0x000077d8
	.4byte 0x000001e3
	.4byte 0x00002001
.L4:
	ldr	r1, [pc, #140]
	movs	r0, #1
	movs	r2, #3
	bl	Func_080dbb24
.L7:
	ldr	r2, [sp, #60]
	movs	r3, #1
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #128]
	movs	r2, #1
	bl	Func_080e0524
	ldr	r3, [sp, #76]
	cmp	r3, #1
	bne.n	.L8
	ldr	r0, [pc, #120]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #112]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L8:
	movs	r4, #140
	lsls	r4, r4, #1
	add	r4, sp
	ldr	r3, [pc, #100]
	mov	r9, r4
	str	r3, [r4, #0]
	mov	r0, r9
	ldr	r3, [pc, #96]
	ldr	r1, [pc, #100]
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #240
	ldr	r3, [pc, #76]
	lsls	r2, r2, #7
	ldr	r0, [pc, #92]
	bl	Func_080072f0
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #84]
	ldr	r3, [pc, #32]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #48
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #22
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	ldr	r1, [pc, #68]
	movs	r0, #0
	strh	r3, [r2, #0]
	mov	r8, r0
	movs	r7, #15
	mov	sl, r1
	b.n	.L9
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x00001f80
	.4byte 0x00002787
	.4byte 0x0000017d
	.4byte 0x000000c1
	.4byte 0x000000c4
	.4byte 0x03001388
	.4byte 0x01010101
	.4byte 0x040000d4
	.4byte 0x02010000
	.4byte 0x85002000
	.4byte 0x06008000
	.4byte 0x04000050
	.4byte 0x05000100
.L9:
	bl	Func_08004458
	adds	r6, r0, #0
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	ands	r5, r7
	ands	r0, r7
	adds	r5, #16
	adds	r0, #16
	ands	r6, r7
	lsls	r0, r0, #10
	lsls	r5, r5, #5
	adds	r6, #16
	orrs	r0, r5
	movs	r4, #1
	orrs	r0, r6
	mov	r2, sl
	add	r8, r4
	strh	r0, [r2, #0]
	movs	r3, #2
	mov	r0, r8
	add	sl, r3
	cmp	r0, #63
	bne.n	.L9
	mov	r1, r9
	movs	r3, #0
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [pc, #436]
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #436]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	movs	r3, #127
	mov	r8, r2
	mov	sl, r3
	movs	r7, #7
.L12:
	bl	Func_08004458
	mov	r4, sl
	adds	r6, r0, #0
	ands	r6, r4
	bl	Func_08004458
	adds	r5, r0, #0
	mov	r0, sl
	ands	r5, r0
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r1, r3, #0
	adds	r1, #64
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L10
	adds	r3, r5, #7
.L10:
	asrs	r3, r3, #3
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L11
	adds	r2, r6, #7
.L11:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ands	r5, r7
	lsls	r3, r3, #3
	adds	r3, r3, r5
	ands	r6, r7
	lsls	r3, r3, #3
	ldr	r2, [sp, #72]
	adds	r3, r3, r6
	strb	r1, [r2, r3]
	movs	r4, #128
	movs	r3, #1
	add	r8, r3
	lsls	r4, r4, #1
	cmp	r8, r4
	bne.n	.L12
	movs	r2, #128
	ldr	r1, [sp, #72]
	ldr	r3, [pc, #336]
	lsls	r2, r2, #7
	ldr	r0, [pc, #336]
	bl	Func_080072f0
	ldr	r2, [pc, #332]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #328]
	ldr	r3, [r0, #0]
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	Func_080d6750
	ldr	r3, [sp, #60]
	ldr	r4, [pc, #320]
	ldr	r2, [r3, #0]
	ldr	r0, [pc, #320]
	adds	r3, r2, r4
	movs	r1, #0
	str	r1, [r3, #0]
	subs	r4, #64
	adds	r3, r2, r0
	str	r1, [r3, #0]
	subs	r0, #64
	adds	r3, r2, r4
	str	r1, [r3, #0]
	adds	r1, r2, r0
	movs	r3, #2
	str	r3, [r1, #0]
	ldr	r4, [sp, #64]
	ldr	r3, [pc, #296]
	adds	r0, #8
	adds	r1, r2, r3
	lsls	r3, r4, #7
	adds	r2, r2, r0
	str	r3, [r1, #0]
	mov	r1, r8
	str	r1, [r2, #0]
	ldr	r0, [pc, #284]
	ldr	r1, [pc, #284]
	bl	Func_080041d8
	movs	r1, #144
	ldr	r0, [pc, #280]
	lsls	r1, r1, #3
	bl	Func_080041d8
	add	r2, sp, #152
	mov	sl, r2
	movs	r7, #63
	mov	r5, sl
	add	r6, sp, #280
.L13:
	bl	Func_08004458
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L13
	movs	r3, #1
	movs	r6, #0
	mov	r8, r3
	movs	r5, #0
.L20:
	mov	r4, r8
	lsrs	r3, r4, #31
	add	r3, r8
	asrs	r3, r3, #1
	movs	r0, #4
	adds	r6, r6, r3
	add	r8, r0
	cmp	r5, r6
	beq.n	.L14
	movs	r1, #127
	movs	r2, #0
	mov	r7, sl
	movs	r4, #7
	mov	lr, r1
	mov	ip, r2
.L19:
	movs	r0, #0
.L18:
	mov	r1, lr
	adds	r3, r0, #0
	ands	r3, r1
	ldrb	r3, [r7, r3]
	subs	r1, r5, r3
	cmp	r1, #0
	blt.n	.L15
	cmp	r1, #127
	bgt.n	.L15
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L16
	adds	r2, r1, #7
.L16:
	asrs	r2, r2, #3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L17
	adds	r3, r0, #7
.L17:
	asrs	r3, r3, #3
	lsls	r2, r2, #5
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	adds	r3, r0, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #156]
	mov	r1, ip
	adds	r2, r2, r3
	strb	r1, [r2, #0]
.L15:
	movs	r2, #128
	adds	r0, #1
	lsls	r2, r2, #1
	cmp	r0, r2
	bne.n	.L18
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L19
.L14:
	ldr	r4, [sp, #60]
	ldr	r0, [pc, #136]
	ldr	r3, [r4, #0]
	movs	r2, #1
	adds	r3, r3, r0
	str	r2, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	cmp	r6, #191
	ble.n	.L20
	ldr	r2, [pc, #120]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [pc, #112]
	ldrh	r1, [r2, #4]
	str	r1, [sp, #56]
	ldrh	r3, [r2, #6]
	ldr	r5, [pc, #108]
	str	r3, [sp, #52]
	ldr	r4, [r5, #0]
	movs	r3, #0
	str	r4, [sp, #48]
	strh	r3, [r2, #4]
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Func_080ed408
	ldr	r5, [r5, #8]
	ldr	r0, [sp, #60]
	b.n	.L21
	.4byte 0x00003f42
	.4byte 0x00001010
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x03001ce0
	.4byte 0x00007828
	.4byte 0x000077d0
	.4byte 0x000077d4
	.4byte 0x00007798
	.4byte 0x080c9139
	.4byte 0x000004ff
	.4byte 0x080cd359
	.4byte 0x02010000
	.4byte 0x00007824
	.4byte 0x04000050
	.4byte 0x03001ad0
	.4byte 0x03001f00
.L21:
	str	r5, [sp, #68]
	movs	r3, #239
	ldr	r2, [r0, #0]
	lsls	r3, r3, #7
	adds	r1, r2, r3
	ldr	r4, [pc, #304]
	movs	r3, #3
	str	r3, [r1, #0]
	ldr	r3, [pc, #300]
	adds	r2, r2, r4
	str	r3, [r2, #0]
	ldr	r1, [pc, #300]
	ldr	r0, [pc, #300]
	bl	Func_080041d8
	ldr	r1, [sp, #60]
	ldr	r4, [pc, #296]
	ldr	r3, [r1, #0]
	movs	r0, #0
	movs	r2, #1
	mov	r8, r0
	negs	r2, r2
	adds	r3, r3, r4
.L22:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #64
	bne.n	.L22
	ldr	r2, [sp, #48]
	movs	r3, #1
	str	r3, [r2, #16]
	ldr	r4, [sp, #60]
	ldr	r0, [pc, #264]
	ldr	r3, [r4, #0]
	movs	r2, #0
	adds	r3, r3, r0
	str	r2, [r3, #0]
	mov	fp, r2
	mov	r1, sp
	mov	r2, sp
	adds	r1, #132
	adds	r2, #88
	movs	r3, #0
	str	r1, [sp, #24]
	str	r2, [sp, #44]
	str	r4, [sp, #40]
	str	r3, [sp, #16]
.L57:
	ldr	r4, [sp, #60]
	ldr	r1, [pc, #232]
	ldr	r0, [r4, #0]
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L23
	adds	r3, #3
.L23:
	asrs	r4, r3, #2
	movs	r2, #252
	ldr	r3, [sp, #76]
	lsls	r2, r2, #5
	adds	r5, r0, r2
	cmp	r3, #1
	bne.n	.L24
	ldr	r3, [pc, #208]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L25
	mov	r0, fp
	cmp	r0, #16
	ble.n	.L25
	b.n	.L26
.L24:
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L25
	mov	r1, fp
	cmp	r1, #4
	ble.n	.L25
	b.n	.L26
.L25:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L27
	movs	r0, #141
	str	r4, [sp, #8]
	bl	Func_080f9010
	ldr	r4, [sp, #8]
.L27:
	movs	r3, #0
	mov	r8, r3
.L28:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r1, #15
	bne.n	.L28
.L34:
	mov	r1, r8
	subs	r1, #16
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L29
	mov	r3, r8
	subs	r3, #13
.L29:
	asrs	r3, r3, #2
	adds	r2, r3, r4
	adds	r3, r2, #0
	adds	r1, r2, #0
	subs	r3, #32
	subs	r1, #80
	cmp	r3, #0
	bge.n	.L30
	movs	r3, #0
.L30:
	cmp	r3, #31
	ble.n	.L31
	movs	r3, #31
.L31:
	cmp	r1, #0
	bge.n	.L32
	movs	r1, #0
.L32:
	cmp	r1, #31
	ble.n	.L33
	movs	r1, #31
.L33:
	lsls	r2, r1, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	asrs	r2, r1, #1
	orrs	r3, r2
	movs	r2, #1
	add	r8, r2
	strh	r3, [r5, #0]
	mov	r3, r8
	adds	r5, #2
	cmp	r3, #135
	bne.n	.L34
	ldr	r3, [pc, #32]
.L35:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r0, #160
	bne.n	.L35
	ldr	r1, [sp, #64]
	cmp	r1, #1
	bne.n	.L36
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L37
	adds	r3, #3
.L37:
	asrs	r7, r3, #2
	b.n	.L38
	.4byte 0x00000000
	.4byte 0x00007784
	.4byte 0x02020202
	.4byte 0x000004fe
	.4byte 0x080e72e1
	.4byte 0x00007098
	.4byte 0x0000778c
	.4byte 0x03001b04
.L36:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L39
	adds	r2, #3
.L39:
	asrs	r2, r2, #2
	movs	r3, #64
	subs	r7, r3, r2
.L38:
	movs	r2, #96
	mov	r3, fp
	subs	r3, r2, r3
	ldr	r4, [sp, #24]
	mov	sl, r3
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L40
	ldr	r1, [sp, #16]
	movs	r2, #160
	ldr	r3, [sp, #44]
	lsls	r2, r2, #8
	adds	r6, r1, r2
	str	r6, [sp, #88]
	movs	r4, #160
	str	r6, [r3, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #15
	lsls	r3, r7, #16
	adds	r3, r3, r4
	str	r3, [r0, #0]
	mov	r1, sl
	movs	r3, #64
	subs	r3, r3, r1
	lsls	r3, r3, #16
	str	r3, [r0, #8]
	ldr	r2, [sp, #40]
	ldr	r4, [pc, #484]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	Func_08009008
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #468]
	ldr	r3, [r0, #0]
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	Func_08009008
	b.n	.L41
.L40:
	ldr	r3, [sp, #16]
	movs	r4, #128
	ldr	r0, [sp, #44]
	lsls	r4, r4, #9
	adds	r6, r3, r4
	str	r6, [sp, #88]
	movs	r1, #192
	str	r6, [r0, #4]
	lsls	r1, r1, #15
	ldr	r4, [sp, #24]
	lsls	r3, r7, #16
	adds	r3, r3, r1
	mov	r0, sl
	str	r3, [r4, #0]
	subs	r3, r2, r0
	lsls	r3, r3, #16
	str	r3, [r4, #8]
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #408]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	Func_08009008
.L41:
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	movs	r3, #32
	subs	r4, r3, r4
	mov	sl, r4
	movs	r2, #0
.L46:
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #0]
	movs	r1, #225
	adds	r3, r3, r2
	lsls	r1, r1, #7
	adds	r5, r3, r1
	movs	r4, #1
	ldr	r3, [r5, #24]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L42
	bl	Func_08004458
	ldr	r3, [pc, #356]
	ands	r3, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r1, r3, r0
	movs	r3, #0
	str	r3, [r5, #24]
	adds	r0, r1, #0
	str	r1, [sp, #12]
	bl	Func_08002322
	adds	r3, r7, #0
	adds	r3, #96
	lsls	r2, r3, #16
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #1
	ldr	r1, [sp, #12]
	cmp	r3, #0
	bge.n	.L43
	ldr	r4, [pc, #320]
	adds	r3, r3, r4
.L43:
	asrs	r3, r3, #16
	muls	r3, r6
	adds	r3, r2, r3
	str	r3, [r5, #0]
	adds	r0, r1, #0
	bl	Func_0800231c
	lsls	r3, r0, #4
	subs	r3, r3, r0
	mov	r1, sl
	lsls	r3, r3, #1
	lsls	r2, r1, #16
	cmp	r3, #0
	bge.n	.L44
	ldr	r4, [pc, #288]
	adds	r3, r3, r4
.L44:
	asrs	r3, r3, #16
	muls	r3, r6
	subs	r3, r2, r3
	str	r3, [r5, #4]
	b.n	.L45
.L42:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r2, #28
	cmp	r1, #32
	bne.n	.L46
.L45:
	add	r5, sp, #96
	movs	r3, #0
	str	r3, [r5, #0]
	str	r3, [r5, #4]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r3, [r5, #8]
	bl	Func_080049ac
	adds	r0, r5, #0
	bl	Func_08004cb4
	movs	r0, #128
	lsls	r0, r0, #4
	bl	Func_08004c6c
	ldr	r0, [sp, #16]
	bl	Func_08004c1c
	movs	r2, #0
	ldr	r7, [pc, #228]
	mov	r8, r2
	add	r6, sp, #120
	add	r5, sp, #108
.L47:
	ldrh	r3, [r7, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	movs	r4, #2
	ldrsh	r3, [r7, r4]
	add	r3, fp
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	ldrh	r3, [r7, #4]
	asrs	r2, r2, #1
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	str	r2, [r6, #0]
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	lsls	r2, r2, #16
	adds	r1, r5, #0
	str	r2, [r6, #8]
	adds	r0, r6, #0
	bl	Func_080e3944
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r3, r2, #0
	adds	r3, #128
	str	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	adds	r1, r3, #0
	adds	r1, #60
	str	r1, [r5, #4]
	ldr	r4, [sp, #60]
	movs	r0, #250
	ldr	r1, [r4, #0]
	lsls	r0, r0, #5
	adds	r1, r1, r0
	movs	r0, #8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	adds	r2, #124
	adds	r3, #56
	ldr	r0, [pc, #132]
	ldr	r4, [sp, #68]
	bl	Func_080072f4
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r7, #6
	cmp	r1, #7
	bne.n	.L47
	ldr	r2, [sp, #64]
	cmp	r2, #1
	bne.n	.L48
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L49
	adds	r3, #3
.L49:
	asrs	r3, r3, #2
	adds	r7, r3, #0
	subs	r7, #16
	b.n	.L50
.L48:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L51
	adds	r2, #3
.L51:
	asrs	r2, r2, #2
	movs	r3, #16
	subs	r7, r3, r2
.L50:
	movs	r3, #96
	negs	r3, r3
	add	r3, fp
	movs	r4, #0
	ldr	r5, [pc, #72]
	mov	sl, r3
	mov	r8, r4
.L54:
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	add	r3, sl
	cmp	r3, #93
	bgt.n	.L52
	ldr	r2, [sp, #60]
	ldr	r1, [r2, #0]
	movs	r0, #0
	ldrsh	r2, [r5, r0]
	movs	r4, #228
	movs	r0, #24
	lsls	r4, r4, #5
	adds	r2, r2, r7
	adds	r1, r1, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #12
	ldr	r0, [pc, #28]
	ldr	r4, [sp, #68]
	bl	Func_080072f4
	b.n	.L53
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x00007fff
	.4byte 0x0000ffff
	.4byte 0x080eee76
	.4byte 0x02010000
	.4byte 0x080eeea0
.L52:
	cmp	r3, #95
	bgt.n	.L53
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	add	r2, sp, #284
	adds	r0, r0, r7
	mov	r9, r2
	lsls	r0, r0, #16
	lsls	r1, r3, #16
	movs	r2, #1
	bl	Func_080e7338
.L53:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #4
	cmp	r4, #7
	bne.n	.L54
	ldr	r1, [sp, #64]
	lsls	r3, r1, #2
	movs	r0, #0
	adds	r3, r3, r1
	mov	r8, r0
	lsls	r7, r3, #14
	movs	r6, #0
.L56:
	ldr	r3, [sp, #60]
	ldr	r2, [r3, #0]
	movs	r4, #225
	adds	r3, r2, r6
	lsls	r4, r4, #7
	adds	r5, r3, r4
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L55
	lsls	r1, r1, #10
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	adds	r1, r2, r1
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r0, #32
	subs	r3, #16
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [pc, #228]
	ldr	r4, [sp, #68]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	subs	r3, r3, r7
	str	r3, [r5, #0]
	ldr	r0, [pc, #216]
	ldr	r3, [r5, #4]
	adds	r3, r3, r0
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #6
	bne.n	.L55
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L55:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L56
	ldr	r4, [sp, #40]
	ldr	r0, [pc, #184]
	ldr	r3, [r4, #0]
	movs	r2, #1
	adds	r3, r3, r0
	str	r2, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #128
	ldr	r1, [sp, #16]
	lsls	r2, r2, #1
	adds	r1, r1, r2
	str	r1, [sp, #16]
	ldr	r4, [sp, #40]
	ldr	r0, [pc, #160]
	ldr	r2, [r4, #0]
	movs	r3, #1
	adds	r2, r2, r0
	add	fp, r3
	ldr	r3, [r2, #0]
	mov	r1, fp
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r1, #192
	beq.n	.L26
	b.n	.L57
.L26:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #48]
	movs	r5, #0
	str	r5, [r2, #16]
	ldr	r0, [pc, #128]
	bl	Func_08004278
	ldr	r0, [pc, #124]
	bl	Func_08004278
	ldr	r0, [pc, #124]
	bl	Func_08004278
	add	r4, sp, #56
	add	r0, sp, #52
	ldr	r3, [pc, #116]
	ldrh	r4, [r4, #0]
	ldrh	r0, [r0, #0]
	strh	r4, [r3, #4]
	strh	r0, [r3, #6]
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080d67dc
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #100]
	str	r5, [r3, #0]
	ldr	r3, [pc, #100]
	adds	r2, #12
	str	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #38
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #70
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Func_080ed408
	ldr	r3, [pc, #68]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #60]
	str	r3, [sp, #68]
	ldr	r0, [pc, #64]
	ldr	r1, [r2, #0]
	b.n	.L58
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00002784
	.4byte 0x02010000
	.4byte 0xfffb0000
	.4byte 0x00007824
	.4byte 0x0000778c
	.4byte 0x080c9139
	.4byte 0x080e72e1
	.4byte 0x080cd359
	.4byte 0x03001ad0
	.4byte 0x04000020
	.4byte 0x04000028
	.4byte 0xfffff000
	.4byte 0x03001e50
	.4byte 0x000000c0
.L58:
	movs	r3, #0
	movs	r2, #1
	bl	Func_080e0524
	movs	r3, #0
	mov	r8, r3
	movs	r7, #127
	movs	r6, #0
.L59:
	ldr	r4, [sp, #60]
	ldr	r5, [r4, #0]
	movs	r0, #225
	adds	r5, r5, r6
	lsls	r0, r0, #7
	adds	r5, r5, r0
	bl	Func_08004458
	ands	r0, r7
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r1, #1
	ands	r0, r7
	add	r8, r1
	adds	r0, #127
	mov	r2, r8
	str	r0, [r5, #4]
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L59
	movs	r3, #0
	ldr	r5, [pc, #816]
	mov	r8, r3
	movs	r6, #0
	movs	r7, #255
.L60:
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #127
	movs	r4, #1
	lsls	r0, r0, #12
	add	r8, r4
	str	r0, [r5, #20]
	mov	r0, r8
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r0, #128
	bne.n	.L60
	movs	r1, #0
	ldr	r5, [pc, #756]
	mov	r8, r1
	movs	r6, #0
	movs	r7, #255
.L61:
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #128
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #13
	add	r8, r2
	lsls	r3, r3, #2
	str	r0, [r5, #20]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L61
	ldr	r4, [sp, #60]
	movs	r0, #239
	ldr	r2, [r4, #0]
	lsls	r0, r0, #7
	adds	r1, r2, r0
	movs	r3, #1
	str	r3, [r1, #0]
	ldr	r1, [pc, #684]
	ldr	r3, [pc, #684]
	adds	r2, r2, r1
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #680]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r0, #232
	ldr	r3, [sp, #60]
	mov	r4, sp
	adds	r4, #80
	lsls	r0, r0, #9
	movs	r2, #0
	str	r3, [sp, #32]
	str	r4, [sp, #28]
	str	r0, [sp, #20]
	mov	fp, r2
.L83:
	ldr	r3, [pc, #656]
	mov	r1, fp
	subs	r1, #16
	ldr	r5, [r3, #0]
	str	r1, [sp, #36]
	cmp	r1, #19
	ble.n	.L62
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	Func_080e727c
.L62:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L63
	movs	r0, #156
	bl	Func_080f9010
.L63:
	mov	r3, fp
	cmp	r3, #40
	bne.n	.L64
	movs	r0, #145
	bl	Func_080f9010
.L64:
	mov	r4, fp
	cmp	r4, #48
	bne.n	.L65
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L66
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #600]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_08009038
	ldr	r4, [sp, #32]
	ldr	r0, [pc, #588]
	ldr	r3, [r4, #0]
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	Func_08009038
	bl	Func_080b5118
.L66:
	movs	r0, #134
	bl	Func_080b50e8
.L65:
	bl	Func_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	Func_080051d8
	ldr	r7, [pc, #528]
	movs	r1, #0
	movs	r2, #63
	mov	r8, r1
	mov	sl, r2
.L71:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	blt.n	.L67
	add	r6, sp, #96
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	Func_080e3944
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #8]
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	cmp	r2, #159
	bgt.n	.L68
	movs	r3, #160
	str	r3, [r6, #8]
	movs	r2, #160
.L68:
	ldr	r3, [pc, #512]
	cmp	r2, r3
	ble.n	.L69
	str	r3, [r6, #8]
	adds	r2, r3, #0
.L69:
	adds	r3, r2, #0
	subs	r3, #160
	cmp	r3, #0
	bge.n	.L70
	adds	r3, #63
.L70:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	ldr	r2, [pc, #492]
	lsls	r5, r0, #1
	subs	r3, r5, #2
	ldrh	r4, [r2, r3]
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	ldr	r2, [sp, #60]
	lsls	r3, r3, #1
	ldr	r1, [r2, #0]
	adds	r4, r4, r3
	movs	r3, #200
	adds	r1, r1, r4
	lsls	r3, r3, #6
	adds	r1, r1, r3
	lsrs	r3, r0, #31
	ldr	r2, [r6, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #68]
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #72]
	bl	Func_080072f4
	ldr	r2, [pc, #432]
	adds	r0, r7, #0
	movs	r1, #64
	bl	Func_080e38b8
	movs	r2, #160
	ldr	r3, [r7, #4]
	lsls	r2, r2, #13
	cmp	r3, r2
	bgt.n	.L67
	movs	r3, #0
	str	r3, [r7, #0]
	str	r3, [r7, #8]
	str	r2, [r7, #4]
	bl	Func_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #15
	str	r0, [r7, #12]
	bl	Func_08004458
	mov	r2, sl
	ands	r0, r2
	lsls	r0, r0, #13
	str	r0, [r7, #16]
	bl	Func_08004458
	mov	r3, sl
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #15
	str	r0, [r7, #20]
.L67:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r7, #28
	cmp	r0, #64
	bne.n	.L71
	movs	r1, #0
	mov	r8, r1
	mov	sl, r1
.L73:
	ldr	r2, [sp, #60]
	movs	r5, #7
	ldr	r1, [r2, #0]
	mov	r2, r8
	ands	r5, r2
	mov	r4, sl
	adds	r3, r1, r4
	movs	r0, #225
	adds	r4, r5, #3
	ldr	r2, [pc, #324]
	lsls	r6, r4, #1
	lsls	r0, r0, #7
	adds	r7, r3, r0
	subs	r3, r6, #2
	ldrh	r0, [r2, r3]
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldr	r2, [r7, #0]
	adds	r0, r0, r3
	lsrs	r3, r4, #1
	adds	r1, r1, r0
	subs	r2, r2, r3
	movs	r0, #200
	ldr	r3, [r7, #4]
	lsls	r0, r0, #6
	subs	r3, r3, r4
	adds	r1, r1, r0
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #68]
	bl	Func_080072f4
	ldr	r3, [r7, #4]
	movs	r0, #10
	subs	r3, r3, r5
	subs	r3, #8
	negs	r0, r0
	str	r3, [r7, #4]
	cmp	r3, r0
	bge.n	.L72
	movs	r3, #128
	str	r3, [r7, #4]
.L72:
	movs	r2, #1
	add	r8, r2
	movs	r1, #28
	mov	r3, r8
	add	sl, r1
	cmp	r3, #64
	bne.n	.L73
	movs	r4, #0
	movs	r0, #255
	ldr	r7, [pc, #188]
	mov	r8, r4
	mov	sl, r4
	mov	r9, r0
.L77:
	movs	r1, #3
	mov	r0, r8
	bl	Func_080022ec
	ldr	r1, [sp, #36]
	cmp	r0, r1
	bge.n	.L74
	ldr	r3, [r7, #4]
	cmp	r3, #0
	blt.n	.L74
	add	r5, sp, #96
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	asrs	r6, r3, #1
	str	r6, [r5, #0]
	ldr	r2, [r7, #24]
	cmp	r2, #13
	bhi.n	.L75
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r4, [sp, #60]
	ldr	r2, [pc, #176]
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	ldrh	r2, [r2, r3]
	ldr	r1, [r4, #0]
	adds	r1, r1, r2
	ldr	r2, [pc, #168]
	ldrh	r4, [r2, r3]
	ldr	r3, [r5, #4]
	lsrs	r0, r4, #1
	subs	r2, r6, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #68]
	bl	Func_080072f4
	ldr	r2, [r7, #24]
.L75:
	adds	r3, r2, #1
	str	r3, [r7, #24]
	cmp	r3, #14
	bne.n	.L76
	movs	r3, #160
	lsls	r3, r3, #13
	mov	r0, sl
	str	r3, [r7, #4]
	str	r0, [r7, #0]
	bl	Func_08004458
	mov	r1, r9
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #16
	mov	r2, sl
	str	r0, [r7, #8]
	str	r2, [r7, #12]
	bl	Func_08004458
	mov	r3, r9
	ands	r0, r3
	mov	r4, sl
	lsls	r0, r0, #11
	str	r0, [r7, #16]
	str	r4, [r7, #20]
	str	r4, [r7, #24]
	b.n	.L74
.L76:
	adds	r0, r7, #0
	movs	r1, #64
	movs	r2, #1
	bl	Func_080e38b8
.L74:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r7, #28
	cmp	r1, #64
	bne.n	.L77
	ldr	r2, [sp, #64]
	cmp	r2, #1
	bne.n	.L78
	mov	r4, fp
	lsrs	r3, r4, #31
	add	r3, fp
	asrs	r3, r3, #1
	adds	r1, r3, #0
	adds	r1, #24
	b.n	.L79
	.4byte 0x02010000
	.4byte 0x02010e00
	.4byte 0x00007784
	.4byte 0x10101010
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x0000031f
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0x080eeebc
	.4byte 0x080eeeca
.L78:
	mov	r0, fp
	lsrs	r3, r0, #31
	add	r3, fp
	asrs	r3, r3, #1
	movs	r2, #56
	subs	r1, r2, r3
.L79:
	mov	r3, fp
	lsls	r2, r3, #1
	mov	r4, fp
	movs	r3, #64
	subs	r0, r3, r2
	lsls	r3, r4, #8
	movs	r4, #128
	lsls	r4, r4, #10
	adds	r2, r3, r4
	ldr	r4, [sp, #24]
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r3, [sp, #76]
	cmp	r3, #1
	bne.n	.L80
	ldr	r4, [sp, #20]
	ldr	r2, [sp, #28]
	str	r4, [sp, #80]
	str	r4, [r2, #4]
	movs	r4, #192
	lsls	r3, r1, #16
	lsls	r4, r4, #15
	ldr	r1, [sp, #24]
	adds	r3, r3, r4
	str	r3, [r1, #0]
	movs	r3, #96
	subs	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r1, #8]
	ldr	r2, [sp, #32]
	ldr	r4, [pc, #200]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	Func_08009008
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #184]
	ldr	r3, [r0, #0]
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	Func_08009008
	b.n	.L81
.L80:
	ldr	r3, [sp, #28]
	str	r2, [sp, #80]
	movs	r4, #192
	str	r2, [r3, #4]
	lsls	r4, r4, #15
	lsls	r3, r1, #16
	ldr	r1, [sp, #24]
	adds	r3, r3, r4
	str	r3, [r1, #0]
	movs	r3, #96
	subs	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r1, #8]
	ldr	r2, [sp, #60]
	ldr	r4, [pc, #132]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	Func_08009008
.L81:
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #120]
	ldr	r3, [r0, #0]
	movs	r2, #1
	adds	r3, r3, r1
	str	r2, [r3, #0]
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	ldr	r4, [sp, #32]
	ldr	r0, [pc, #104]
	ldr	r3, [r4, #0]
	movs	r1, #1
	adds	r3, r3, r0
	str	r1, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #128
	ldr	r2, [sp, #20]
	movs	r4, #1
	lsls	r3, r3, #1
	add	fp, r4
	adds	r2, r2, r3
	mov	r0, fp
	str	r2, [sp, #20]
	cmp	r0, #54
	beq.n	.L82
	b.n	.L83
.L82:
	ldr	r0, [pc, #68]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r1, [sp, #76]
	cmp	r1, #0
	bne.n	.L84
	ldr	r2, [sp, #60]
	ldr	r4, [pc, #32]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	bl	Func_08009038
.L84:
	bl	Func_080cdbc0
	add	sp, #284
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
