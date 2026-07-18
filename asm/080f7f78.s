@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f7f78
	.thumb_func
Func_080f7f78:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	ldr	r5, [pc, #740]
	ldr	r3, [pc, #744]
	str	r0, [sp, #40]
	movs	r0, #0
	str	r1, [sp, #36]
	str	r0, [sp, #32]
	str	r3, [r5, #0]
	adds	r6, r2, #0
	bl	Func_080f7db4
	ldr	r2, [r5, #0]
	ldr	r1, [pc, #728]
	adds	r3, r2, r1
	str	r6, [r3, #0]
	ldr	r6, [pc, #724]
	ldr	r4, [sp, #32]
	ldr	r7, [pc, #724]
	adds	r3, r2, r6
	ldr	r0, [pc, #724]
	str	r4, [r3, #0]
	adds	r3, r2, r7
	str	r4, [r3, #0]
	adds	r3, r2, r0
	str	r4, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #6
	ldr	r4, [pc, #712]
	add	r7, sp, #32
	adds	r1, r2, r3
	ldrb	r7, [r7, #0]
	movs	r3, #128
	str	r3, [r1, #0]
	subs	r0, #56
	adds	r3, r2, r4
	strb	r7, [r3, #0]
	adds	r2, r2, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r1, #168
	lsls	r1, r1, #2
	ldr	r2, [sp, #40]
	movs	r0, #0
	bl	Func_080f7e60
	ldr	r2, [r5, #0]
	ldr	r3, [r2, r6]
	ldr	r1, [pc, #676]
	lsls	r3, r3, #2
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r0, [pc, #632]
	ldr	r4, [pc, #640]
	ldr	r1, [r0, #0]
	adds	r3, r1, r4
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #652]
	mov	ip, r3
	adds	r2, r1, r5
	movs	r3, #1
	str	r3, [r2, #0]
	mov	r7, ip
	ldr	r2, [pc, #636]
	lsls	r3, r7, #2
	adds	r3, r3, r2
	movs	r4, #1
	ldr	r3, [r1, r3]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L2
	b.n	.L3
.L2:
	movs	r5, #192
	lsls	r3, r3, #2
	lsls	r5, r5, #6
	adds	r3, r3, r5
	ldr	r6, [r1, r3]
	movs	r3, #136
	lsls	r3, r3, #1
	ldr	r7, [pc, #612]
	add	r3, ip
	cmp	r3, r7
	ble.n	.L4
	cmp	r6, #0
	bne.n	.L5
	b.n	.L3
.L5:
	ldr	r2, [r6, #8]
	mov	r0, ip
	ldr	r1, [pc, #596]
	subs	r7, r0, r2
	ands	r7, r1
	subs	r3, r7, #1
	cmp	r3, #62
	bhi.n	.L6
	ldr	r3, [pc, #548]
	ldr	r0, [pc, #580]
	adds	r5, r2, #0
	mov	r2, ip
	adds	r2, #1
	ldr	r1, [r3, #0]
	mov	lr, r3
	ands	r2, r0
	ldr	r3, [pc, #560]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r5, #1
	ands	r3, r0
	ldr	r2, [pc, #548]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	mov	r3, r8
	ldr	r2, [r1, r3]
	mov	r3, sl
	ldr	r1, [r1, r3]
	movs	r4, #1
	cmp	r2, r1
	bne.n	.L7
.L8:
	ldr	r1, [pc, #536]
	adds	r4, #1
	cmp	r4, r1
	bgt.n	.L7
	mov	r2, lr
	mov	r3, ip
	ldr	r1, [r2, #0]
	adds	r2, r3, r4
	ands	r2, r0
	ldr	r3, [pc, #508]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r5, r4
	ands	r3, r0
	ldr	r2, [pc, #496]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	mov	r3, r8
	ldr	r2, [r1, r3]
	mov	r3, sl
	ldr	r1, [r1, r3]
	cmp	r2, r1
	beq.n	.L8
.L7:
	ldr	r5, [pc, #448]
	ldr	r0, [pc, #476]
	ldr	r2, [r5, #0]
	adds	r1, r2, r0
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L6
	ldr	r5, [pc, #476]
	adds	r3, r2, r5
	str	r7, [r3, #0]
	movs	r7, #136
	lsls	r7, r7, #1
	str	r4, [r1, #0]
	cmp	r4, r7
	beq.n	.L3
.L6:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L5
	b.n	.L3
.L4:
	cmp	r6, #0
	beq.n	.L3
	mov	r2, ip
	ldr	r4, [pc, #452]
	lsls	r3, r2, #2
	ldr	r0, [pc, #436]
	ldr	r1, [pc, #396]
	adds	r4, r3, r4
	adds	r2, r3, #0
	str	r4, [sp, #8]
	mov	r9, r0
	mov	sl, r1
	mov	fp, r2
.L12:
	ldr	r0, [r6, #8]
	mov	r7, ip
	subs	r5, r7, r0
	mov	r1, r9
	ands	r5, r1
	subs	r3, r5, #1
	cmp	r3, #62
	bhi.n	.L9
	mov	r2, sl
	mov	r7, r9
	adds	r3, r0, #1
	ldr	r1, [r2, #0]
	ands	r3, r7
	ldr	r2, [pc, #384]
	lsls	r3, r3, #2
	ldr	r7, [sp, #8]
	adds	r3, r3, r2
	ldr	r3, [r1, r3]
	ldr	r2, [r1, r7]
	movs	r4, #1
	cmp	r2, r3
	bne.n	.L10
	ldr	r1, [pc, #340]
	adds	r7, r0, #0
	ldr	r2, [pc, #372]
	ldr	r0, [pc, #380]
	mov	r8, r1
	mov	lr, r2
	add	r0, fp
.L11:
	ldr	r3, [pc, #364]
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r3
	bgt.n	.L10
	mov	r2, r8
	ldr	r1, [r2, #0]
	adds	r3, r7, r4
	mov	r2, lr
	ands	r3, r2
	ldr	r2, [pc, #332]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r3, [r1, r3]
	ldr	r2, [r1, r0]
	cmp	r2, r3
	beq.n	.L11
.L10:
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r7, [pc, #320]
	adds	r1, r2, r7
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L9
	ldr	r0, [pc, #320]
	adds	r3, r2, r0
	str	r5, [r3, #0]
	str	r4, [r1, #0]
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r4, r1
	beq.n	.L3
.L9:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L12
.L3:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	beq.n	.L13
	b.n	.L14
.L13:
	ldr	r3, [pc, #248]
	ldr	r4, [pc, #276]
	ldr	r2, [r3, #0]
	adds	r1, r2, r4
	ldr	r5, [r1, #0]
	str	r5, [sp, #28]
	cmp	r5, #1
	bgt.n	.L15
	b.n	.L16
.L15:
	ldr	r7, [pc, #272]
	adds	r3, r2, r7
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #236]
	str	r3, [sp, #24]
	str	r5, [sp, #20]
	adds	r3, r2, r0
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #248]
	adds	r3, #1
	ands	r3, r4
	mov	ip, r3
	mov	r5, ip
	movs	r3, #1
	ldr	r7, [pc, #228]
	str	r3, [r1, #0]
	lsls	r3, r5, #2
	adds	r3, r3, r7
	movs	r0, #1
	ldr	r3, [r2, r3]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L17
	b.n	.L18
.L17:
	movs	r1, #192
	lsls	r3, r3, #2
	lsls	r1, r1, #6
	adds	r3, r3, r1
	ldr	r6, [r2, r3]
	movs	r3, #136
	lsls	r3, r3, #1
	add	r3, ip
	cmp	r3, r4
	ble.n	.L19
	cmp	r6, #0
	bne.n	.L20
	b.n	.L18
.L20:
	ldr	r2, [r6, #8]
	mov	r3, ip
	ldr	r4, [pc, #188]
	subs	r7, r3, r2
	ands	r7, r4
	subs	r3, r7, #1
	cmp	r3, #62
	bhi.n	.L21
	ldr	r5, [pc, #140]
	mov	lr, r5
	adds	r5, r2, #0
	mov	r2, lr
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #164]
	mov	r2, ip
	adds	r2, #1
	ands	r2, r0
	ldr	r3, [pc, #148]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r5, #1
	ands	r3, r0
	ldr	r2, [pc, #136]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	mov	r3, r8
	ldr	r2, [r1, r3]
	mov	r3, sl
	ldr	r1, [r1, r3]
	movs	r4, #1
	cmp	r2, r1
	bne.n	.L22
.L23:
	ldr	r1, [pc, #128]
	adds	r4, #1
	cmp	r4, r1
	bgt.n	.L22
	mov	r2, lr
	mov	r3, ip
	ldr	r1, [r2, #0]
	adds	r2, r3, r4
	ands	r2, r0
	ldr	r3, [pc, #96]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r5, r4
	ands	r3, r0
	ldr	r2, [pc, #84]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	mov	r3, r8
	ldr	r2, [r1, r3]
	mov	r3, sl
	ldr	r1, [r1, r3]
	cmp	r2, r1
	beq.n	.L23
.L22:
	ldr	r5, [pc, #36]
	ldr	r0, [pc, #68]
	ldr	r2, [r5, #0]
	adds	r1, r2, r0
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L21
	ldr	r5, [pc, #68]
	adds	r3, r2, r5
	str	r7, [r3, #0]
	movs	r7, #136
	lsls	r7, r7, #1
	str	r4, [r1, #0]
	cmp	r4, r7
	beq.n	.L18
.L21:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L20
	b.n	.L18
	movs	r0, r0
	.4byte 0x02004c00
	.4byte 0x02010000
	.4byte 0x00004440
	.4byte 0x00004434
	.4byte 0x00004438
	.4byte 0x0000443c
	.4byte 0x00004408
	.4byte 0x00003404
	.4byte 0x00004430
	.4byte 0x000003ff
	.4byte 0x0000010f
	.4byte 0x0000442c
	.4byte 0x00003408
.L19:
	cmp	r6, #0
	beq.n	.L18
	mov	r2, ip
	ldr	r4, [pc, #704]
	lsls	r3, r2, #2
	ldr	r0, [pc, #704]
	ldr	r1, [pc, #704]
	adds	r4, r3, r4
	adds	r2, r3, #0
	str	r4, [sp, #4]
	mov	r9, r0
	mov	sl, r1
	mov	fp, r2
.L27:
	ldr	r0, [r6, #8]
	mov	r7, ip
	subs	r5, r7, r0
	mov	r1, r9
	ands	r5, r1
	subs	r3, r5, #1
	cmp	r3, #62
	bhi.n	.L24
	mov	r2, sl
	mov	r7, r9
	adds	r3, r0, #1
	ldr	r1, [r2, #0]
	ands	r3, r7
	ldr	r2, [pc, #668]
	lsls	r3, r3, #2
	ldr	r7, [sp, #4]
	adds	r3, r3, r2
	ldr	r3, [r1, r3]
	ldr	r2, [r1, r7]
	movs	r4, #1
	cmp	r2, r3
	bne.n	.L25
	ldr	r1, [pc, #648]
	adds	r7, r0, #0
	ldr	r2, [pc, #640]
	ldr	r0, [pc, #632]
	mov	r8, r1
	mov	lr, r2
	add	r0, fp
.L26:
	ldr	r3, [pc, #640]
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r3
	bgt.n	.L25
	mov	r2, r8
	ldr	r1, [r2, #0]
	adds	r3, r7, r4
	mov	r2, lr
	ands	r3, r2
	ldr	r2, [pc, #616]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r3, [r1, r3]
	ldr	r2, [r1, r0]
	cmp	r2, r3
	beq.n	.L26
.L25:
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r7, [pc, #608]
	adds	r1, r2, r7
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L24
	ldr	r0, [pc, #600]
	adds	r3, r2, r0
	str	r5, [r3, #0]
	str	r4, [r1, #0]
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r4, r1
	beq.n	.L18
.L24:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L27
.L18:
	ldr	r3, [pc, #564]
	ldr	r4, [pc, #572]
	ldr	r2, [r3, #0]
	adds	r1, r2, r4
	ldr	r3, [r1, #0]
	cmp	r3, #2
	bgt.n	.L28
	b.n	.L29
.L28:
	ldr	r5, [pc, #568]
	adds	r3, #1
	str	r3, [sp, #12]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	ldr	r7, [sp, #28]
	adds	r3, r3, r7
	mov	ip, r3
	ldr	r3, [pc, #528]
	mov	r0, ip
	ands	r0, r3
	ldr	r4, [pc, #528]
	movs	r3, #1
	str	r3, [r1, #0]
	lsls	r3, r0, #2
	adds	r3, r3, r4
	movs	r5, #1
	ldr	r3, [r2, r3]
	negs	r5, r5
	mov	ip, r0
	cmp	r3, r5
	bne.n	.L30
	b.n	.L31
.L30:
	movs	r7, #192
	lsls	r3, r3, #2
	lsls	r7, r7, #6
	adds	r3, r3, r7
	ldr	r6, [r2, r3]
	movs	r3, #136
	lsls	r3, r3, #1
	ldr	r0, [pc, #484]
	add	r3, ip
	cmp	r3, r0
	ble.n	.L32
	cmp	r6, #0
	bne.n	.L33
	b.n	.L31
.L33:
	ldr	r2, [r6, #8]
	ldr	r3, [pc, #468]
	mov	r1, ip
	subs	r7, r1, r2
	ands	r7, r3
	subs	r3, r7, #1
	cmp	r3, #62
	bhi.n	.L34
	ldr	r5, [pc, #456]
	mov	lr, r5
	adds	r5, r2, #0
	mov	r2, lr
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #444]
	mov	r2, ip
	adds	r2, #1
	ands	r2, r0
	ldr	r3, [pc, #444]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r5, #1
	ands	r3, r0
	ldr	r2, [pc, #432]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	mov	r3, r8
	ldr	r2, [r1, r3]
	mov	r3, sl
	ldr	r1, [r1, r3]
	movs	r4, #1
	cmp	r2, r1
	bne.n	.L35
.L36:
	ldr	r1, [pc, #412]
	adds	r4, #1
	cmp	r4, r1
	bgt.n	.L35
	mov	r2, lr
	mov	r3, ip
	ldr	r1, [r2, #0]
	adds	r2, r3, r4
	ands	r2, r0
	ldr	r3, [pc, #392]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r5, r4
	ands	r3, r0
	ldr	r2, [pc, #380]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	mov	r3, r8
	ldr	r2, [r1, r3]
	mov	r3, sl
	ldr	r1, [r1, r3]
	cmp	r2, r1
	beq.n	.L36
.L35:
	ldr	r5, [pc, #356]
	ldr	r0, [pc, #364]
	ldr	r2, [r5, #0]
	adds	r1, r2, r0
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L34
	ldr	r5, [pc, #356]
	adds	r3, r2, r5
	str	r7, [r3, #0]
	movs	r7, #136
	lsls	r7, r7, #1
	str	r4, [r1, #0]
	cmp	r4, r7
	beq.n	.L31
.L34:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L33
	b.n	.L31
.L32:
	cmp	r6, #0
	beq.n	.L31
	mov	r2, ip
	ldr	r4, [pc, #304]
	lsls	r3, r2, #2
	ldr	r0, [pc, #304]
	ldr	r1, [pc, #304]
	adds	r4, r3, r4
	adds	r2, r3, #0
	str	r4, [sp, #0]
	mov	r9, r0
	mov	sl, r1
	mov	fp, r2
.L40:
	ldr	r0, [r6, #8]
	mov	r7, ip
	subs	r5, r7, r0
	mov	r1, r9
	ands	r5, r1
	subs	r3, r5, #1
	cmp	r3, #62
	bhi.n	.L37
	mov	r2, sl
	mov	r7, r9
	adds	r3, r0, #1
	ldr	r1, [r2, #0]
	ands	r3, r7
	ldr	r2, [pc, #268]
	lsls	r3, r3, #2
	ldr	r7, [sp, #0]
	adds	r3, r3, r2
	ldr	r3, [r1, r3]
	ldr	r2, [r1, r7]
	movs	r4, #1
	cmp	r2, r3
	bne.n	.L38
	ldr	r1, [pc, #248]
	adds	r7, r0, #0
	ldr	r2, [pc, #240]
	ldr	r0, [pc, #232]
	mov	r8, r1
	mov	lr, r2
	add	r0, fp
.L39:
	ldr	r3, [pc, #240]
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r3
	bgt.n	.L38
	mov	r2, r8
	ldr	r1, [r2, #0]
	adds	r3, r7, r4
	mov	r2, lr
	ands	r3, r2
	ldr	r2, [pc, #216]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r3, [r1, r3]
	ldr	r2, [r1, r0]
	cmp	r2, r3
	beq.n	.L39
.L38:
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r7, [pc, #208]
	adds	r1, r2, r7
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L37
	ldr	r0, [pc, #200]
	adds	r3, r2, r0
	str	r5, [r3, #0]
	str	r4, [r1, #0]
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r4, r1
	beq.n	.L31
.L37:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L40
.L31:
	ldr	r2, [pc, #164]
	ldr	r4, [pc, #172]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r3, [r3, #0]
	ldr	r5, [sp, #28]
	ldr	r7, [sp, #12]
	adds	r3, r3, r5
	str	r3, [sp, #16]
	cmp	r7, r3
	blt.n	.L29
	movs	r0, #1
	str	r0, [sp, #20]
	str	r0, [sp, #32]
.L29:
	ldr	r1, [pc, #136]
	ldr	r4, [pc, #148]
	ldr	r2, [r1, #0]
	ldr	r5, [sp, #24]
	adds	r3, r2, r4
	str	r5, [r3, #0]
	ldr	r7, [pc, #136]
	ldr	r0, [sp, #20]
	adds	r2, r2, r7
	str	r0, [r2, #0]
.L14:
	ldr	r1, [pc, #116]
	ldr	r2, [pc, #124]
	ldr	r5, [r1, #0]
	adds	r6, r5, r2
	ldr	r3, [r6, #0]
	cmp	r3, #1
	ble.n	.L16
	movs	r3, #0
	ldr	r4, [pc, #124]
	movs	r7, #208
	str	r3, [sp, #32]
	lsls	r7, r7, #6
	adds	r1, r5, r4
	adds	r3, r5, r7
	ldr	r3, [r3, #0]
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r0, [r6, #0]
	cmp	r0, #16
	bgt.n	.L41
	ldr	r1, [pc, #88]
	adds	r3, r5, r1
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #52]
	lsls	r1, r2, #4
	ands	r1, r3
	movs	r3, #255
	ands	r2, r3
	orrs	r1, r2
	subs	r3, r0, #1
	ldr	r2, [pc, #40]
	lsls	r3, r3, #8
	ands	r3, r2
	ldr	r2, [pc, #76]
	adds	r0, r5, r2
	ldr	r2, [r0, #0]
	orrs	r1, r3
	lsls	r1, r1, #16
	ldr	r7, [pc, #60]
	adds	r3, r2, r4
	adds	r2, #1
	asrs	r4, r1, #16
	lsrs	r1, r1, #24
	strb	r1, [r5, r3]
	str	r2, [r0, #0]
	adds	r3, r2, r7
	adds	r2, #1
	strb	r4, [r5, r3]
	str	r2, [r0, #0]
	b.n	.L42
	.4byte 0xfffff000
	.4byte 0x00000f00
	.4byte 0x00003408
	.4byte 0x000003ff
	.4byte 0x02004c00
	.4byte 0x00003404
	.4byte 0x0000010f
	.4byte 0x00004430
	.4byte 0x0000442c
	.4byte 0x00004434
	.4byte 0x00004408
	.4byte 0x00004404
.L41:
	ldr	r0, [pc, #68]
	adds	r3, r5, r0
	ldr	r1, [r3, #0]
	ldr	r2, [pc, #60]
	lsls	r3, r1, #4
	ands	r3, r2
	movs	r2, #255
	ands	r1, r2
	orrs	r3, r1
	ldr	r1, [pc, #56]
	lsls	r3, r3, #16
	adds	r0, r5, r1
	ldr	r2, [r0, #0]
	asrs	r4, r3, #16
	ldr	r3, [pc, #48]
	ldr	r7, [pc, #48]
	adds	r1, r2, r3
	adds	r2, #1
	lsrs	r3, r4, #8
	strb	r3, [r5, r1]
	adds	r3, r2, r7
	adds	r1, r2, #1
	str	r2, [r0, #0]
	strb	r4, [r5, r3]
	str	r1, [r0, #0]
	ldr	r3, [pc, #32]
	adds	r2, r2, r3
	ldr	r3, [r6, #0]
	adds	r1, #1
	subs	r3, #17
	strb	r3, [r5, r2]
	str	r1, [r0, #0]
	b.n	.L43
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0x0000442c
	.4byte 0x00004404
	.4byte 0x00004408
	.4byte 0x00004409
.L43:
	b.n	.L42
.L16:
	ldr	r4, [pc, #232]
	ldr	r5, [pc, #232]
	ldr	r2, [r4, #0]
	adds	r4, r2, r5
	adds	r5, #48
	ldr	r1, [r4, #0]
	ldr	r7, [pc, #228]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	adds	r0, r1, r7
	ldr	r7, [pc, #224]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	ldr	r3, [r2, r3]
	strb	r3, [r2, r0]
	ldr	r0, [pc, #216]
	adds	r1, #1
	adds	r2, r2, r0
	movs	r3, #1
	str	r1, [r4, #0]
	str	r3, [r2, #0]
.L42:
	ldr	r7, [pc, #188]
	ldr	r5, [pc, #208]
	ldr	r3, [r7, #0]
	ldr	r6, [pc, #200]
	ldr	r0, [r3, r5]
	movs	r1, #168
	lsls	r1, r1, #2
	adds	r0, r0, r1
	ldr	r2, [sp, #40]
	ldr	r1, [r3, r6]
	bl	Func_080f7e60
	ldr	r1, [r7, #0]
	adds	r5, r1, r5
	ldr	r2, [r1, r6]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	ldr	r2, [pc, #180]
	ands	r3, r2
	str	r3, [r5, #0]
	movs	r3, #208
	lsls	r3, r3, #6
	adds	r1, r1, r3
	ldr	r3, [r1, #0]
	asrs	r5, r3, #1
	str	r5, [r1, #0]
	cmp	r5, #0
	bne.n	.L44
	ldr	r0, [sp, #36]
	bl	Func_080f7f30
	ldr	r2, [r7, #0]
	movs	r4, #208
	lsls	r4, r4, #6
	adds	r1, r2, r4
	movs	r3, #128
	ldr	r0, [pc, #124]
	str	r3, [r1, #0]
	ldr	r1, [pc, #116]
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r2, r2, r1
	movs	r3, #1
	str	r3, [r2, #0]
.L44:
	ldr	r2, [r7, #0]
	ldr	r4, [pc, #120]
	adds	r3, r2, r4
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #104]
	lsls	r3, r3, #2
	adds	r3, r3, r5
	movs	r7, #1
	ldr	r3, [r2, r3]
	negs	r7, r7
	cmp	r3, r7
	beq.n	.L1
	b.n	.L0
.L1:
	ldr	r6, [pc, #76]
	ldr	r0, [pc, #80]
	ldr	r4, [r6, #0]
	movs	r2, #208
	lsls	r2, r2, #6
	adds	r1, r4, r0
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #56]
	adds	r1, r4, r3
	ldr	r3, [r1, #0]
	movs	r5, #0
	adds	r2, r3, r0
	adds	r3, #1
	strb	r5, [r4, r2]
	adds	r0, r3, r0
	str	r3, [r1, #0]
	adds	r3, #1
	strb	r5, [r4, r0]
	str	r3, [r1, #0]
	ldr	r0, [sp, #36]
	bl	Func_080f7f30
	ldr	r3, [r6, #0]
	ldr	r4, [pc, #48]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02004c00
	.4byte 0x00004404
	.4byte 0x00004408
	.4byte 0x00003404
	.4byte 0x00004430
	.4byte 0x00004434
	.4byte 0x000003ff
	.4byte 0x0000443c
