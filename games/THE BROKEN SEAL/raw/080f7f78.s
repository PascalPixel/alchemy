.syntax unified
	.thumb
	.set sub_080f7db4, 0x080f7db4
	.set sub_080f7e60, 0x080f7e60
	.set sub_080f7f30, 0x080f7f30
	.global Overlay_080f7f78
Overlay_080f7f78:
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
	bl	sub_080f7db4
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
	bl	sub_080f7e60
	ldr	r2, [r5, #0]
	ldr	r3, [r2, r6]
	ldr	r1, [pc, #676]
	lsls	r3, r3, #2
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080f7ff6
	b.n	.L_080f868a
.L_080f7ff6:
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
	bne.n	.L_080f801e
	b.n	.L_080f816e
.L_080f801e:
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
	ble.n	.L_080f80d4
	cmp	r6, #0
	bne.n	.L_080f803a
	b.n	.L_080f816e
.L_080f803a:
	ldr	r2, [r6, #8]
	mov	r0, ip
	ldr	r1, [pc, #596]
	subs	r7, r0, r2
	ands	r7, r1
	subs	r3, r7, #1
	cmp	r3, #62
	bhi.n	.L_080f80cc
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
	bne.n	.L_080f80ae
.L_080f807c:
	ldr	r1, [pc, #536]
	adds	r4, #1
	cmp	r4, r1
	bgt.n	.L_080f80ae
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
	beq.n	.L_080f807c
.L_080f80ae:
	ldr	r5, [pc, #448]
	ldr	r0, [pc, #476]
	ldr	r2, [r5, #0]
	adds	r1, r2, r0
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L_080f80cc
	ldr	r5, [pc, #476]
	adds	r3, r2, r5
	str	r7, [r3, #0]
	movs	r7, #136
	lsls	r7, r7, #1
	str	r4, [r1, #0]
	cmp	r4, r7
	beq.n	.L_080f816e
.L_080f80cc:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L_080f803a
	b.n	.L_080f816e
.L_080f80d4:
	cmp	r6, #0
	beq.n	.L_080f816e
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
.L_080f80ee:
	ldr	r0, [r6, #8]
	mov	r7, ip
	subs	r5, r7, r0
	mov	r1, r9
	ands	r5, r1
	subs	r3, r5, #1
	cmp	r3, #62
	bhi.n	.L_080f8168
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
	bne.n	.L_080f814a
	ldr	r1, [pc, #340]
	adds	r7, r0, #0
	ldr	r2, [pc, #372]
	ldr	r0, [pc, #380]
	mov	r8, r1
	mov	lr, r2
	add	r0, fp
.L_080f8128:
	ldr	r3, [pc, #364]
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r3
	bgt.n	.L_080f814a
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
	beq.n	.L_080f8128
.L_080f814a:
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r7, [pc, #320]
	adds	r1, r2, r7
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L_080f8168
	ldr	r0, [pc, #320]
	adds	r3, r2, r0
	str	r5, [r3, #0]
	str	r4, [r1, #0]
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r4, r1
	beq.n	.L_080f816e
.L_080f8168:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L_080f80ee
.L_080f816e:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	beq.n	.L_080f8176
	b.n	.L_080f84fe
.L_080f8176:
	ldr	r3, [pc, #248]
	ldr	r4, [pc, #276]
	ldr	r2, [r3, #0]
	adds	r1, r2, r4
	ldr	r5, [r1, #0]
	str	r5, [sp, #28]
	cmp	r5, #1
	bgt.n	.L_080f8188
	b.n	.L_080f85ee
.L_080f8188:
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
	bne.n	.L_080f81b8
	b.n	.L_080f833e
.L_080f81b8:
	movs	r1, #192
	lsls	r3, r3, #2
	lsls	r1, r1, #6
	adds	r3, r3, r1
	ldr	r6, [r2, r3]
	movs	r3, #136
	lsls	r3, r3, #1
	add	r3, ip
	cmp	r3, r4
	ble.n	.L_080f82a4
	cmp	r6, #0
	bne.n	.L_080f81d2
	b.n	.L_080f833e
.L_080f81d2:
	ldr	r2, [r6, #8]
	mov	r3, ip
	ldr	r4, [pc, #188]
	subs	r7, r3, r2
	ands	r7, r4
	subs	r3, r7, #1
	cmp	r3, #62
	bhi.n	.L_080f8266
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
	bne.n	.L_080f8248
.L_080f8216:
	ldr	r1, [pc, #128]
	adds	r4, #1
	cmp	r4, r1
	bgt.n	.L_080f8248
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
	beq.n	.L_080f8216
.L_080f8248:
	ldr	r5, [pc, #36]
	ldr	r0, [pc, #68]
	ldr	r2, [r5, #0]
	adds	r1, r2, r0
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L_080f8266
	ldr	r5, [pc, #68]
	adds	r3, r2, r5
	str	r7, [r3, #0]
	movs	r7, #136
	lsls	r7, r7, #1
	str	r4, [r1, #0]
	cmp	r4, r7
	beq.n	.L_080f833e
.L_080f8266:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L_080f81d2
	b.n	.L_080f833e
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
	.2byte 0x3408
	.2byte 0x0000
.L_080f82a4:
	cmp	r6, #0
	beq.n	.L_080f833e
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
.L_080f82be:
	ldr	r0, [r6, #8]
	mov	r7, ip
	subs	r5, r7, r0
	mov	r1, r9
	ands	r5, r1
	subs	r3, r5, #1
	cmp	r3, #62
	bhi.n	.L_080f8338
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
	bne.n	.L_080f831a
	ldr	r1, [pc, #648]
	adds	r7, r0, #0
	ldr	r2, [pc, #640]
	ldr	r0, [pc, #632]
	mov	r8, r1
	mov	lr, r2
	add	r0, fp
.L_080f82f8:
	ldr	r3, [pc, #640]
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r3
	bgt.n	.L_080f831a
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
	beq.n	.L_080f82f8
.L_080f831a:
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r7, [pc, #608]
	adds	r1, r2, r7
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L_080f8338
	ldr	r0, [pc, #600]
	adds	r3, r2, r0
	str	r5, [r3, #0]
	str	r4, [r1, #0]
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r4, r1
	beq.n	.L_080f833e
.L_080f8338:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L_080f82be
.L_080f833e:
	ldr	r3, [pc, #564]
	ldr	r4, [pc, #572]
	ldr	r2, [r3, #0]
	adds	r1, r2, r4
	ldr	r3, [r1, #0]
	cmp	r3, #2
	bgt.n	.L_080f834e
	b.n	.L_080f84ea
.L_080f834e:
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
	bne.n	.L_080f837c
	b.n	.L_080f84ce
.L_080f837c:
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
	ble.n	.L_080f8434
	cmp	r6, #0
	bne.n	.L_080f8398
	b.n	.L_080f84ce
.L_080f8398:
	ldr	r2, [r6, #8]
	ldr	r3, [pc, #468]
	mov	r1, ip
	subs	r7, r1, r2
	ands	r7, r3
	subs	r3, r7, #1
	cmp	r3, #62
	bhi.n	.L_080f842c
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
	bne.n	.L_080f840e
.L_080f83dc:
	ldr	r1, [pc, #412]
	adds	r4, #1
	cmp	r4, r1
	bgt.n	.L_080f840e
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
	beq.n	.L_080f83dc
.L_080f840e:
	ldr	r5, [pc, #356]
	ldr	r0, [pc, #364]
	ldr	r2, [r5, #0]
	adds	r1, r2, r0
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L_080f842c
	ldr	r5, [pc, #356]
	adds	r3, r2, r5
	str	r7, [r3, #0]
	movs	r7, #136
	lsls	r7, r7, #1
	str	r4, [r1, #0]
	cmp	r4, r7
	beq.n	.L_080f84ce
.L_080f842c:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L_080f8398
	b.n	.L_080f84ce
.L_080f8434:
	cmp	r6, #0
	beq.n	.L_080f84ce
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
.L_080f844e:
	ldr	r0, [r6, #8]
	mov	r7, ip
	subs	r5, r7, r0
	mov	r1, r9
	ands	r5, r1
	subs	r3, r5, #1
	cmp	r3, #62
	bhi.n	.L_080f84c8
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
	bne.n	.L_080f84aa
	ldr	r1, [pc, #248]
	adds	r7, r0, #0
	ldr	r2, [pc, #240]
	ldr	r0, [pc, #232]
	mov	r8, r1
	mov	lr, r2
	add	r0, fp
.L_080f8488:
	ldr	r3, [pc, #240]
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r3
	bgt.n	.L_080f84aa
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
	beq.n	.L_080f8488
.L_080f84aa:
	mov	r3, sl
	ldr	r2, [r3, #0]
	ldr	r7, [pc, #208]
	adds	r1, r2, r7
	ldr	r3, [r1, #0]
	cmp	r3, r4
	bge.n	.L_080f84c8
	ldr	r0, [pc, #200]
	adds	r3, r2, r0
	str	r5, [r3, #0]
	str	r4, [r1, #0]
	movs	r1, #136
	lsls	r1, r1, #1
	cmp	r4, r1
	beq.n	.L_080f84ce
.L_080f84c8:
	ldr	r6, [r6, #0]
	cmp	r6, #0
	bne.n	.L_080f844e
.L_080f84ce:
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
	blt.n	.L_080f84ea
	movs	r0, #1
	str	r0, [sp, #20]
	str	r0, [sp, #32]
.L_080f84ea:
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
.L_080f84fe:
	ldr	r1, [pc, #116]
	ldr	r2, [pc, #124]
	ldr	r5, [r1, #0]
	adds	r6, r5, r2
	ldr	r3, [r6, #0]
	cmp	r3, #1
	ble.n	.L_080f85ee
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
	bgt.n	.L_080f8594
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
	b.n	.L_080f8618
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
	.2byte 0x4404
	.2byte 0x0000
.L_080f8594:
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
	b.n	.L_080f85ec
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0x0000442c
	.4byte 0x00004404
	.4byte 0x00004408
	.2byte 0x4409
	.2byte 0x0000
.L_080f85ec:
	b.n	.L_080f8618
.L_080f85ee:
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
.L_080f8618:
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
	bl	sub_080f7e60
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
	bne.n	.L_080f8670
	ldr	r0, [sp, #36]
	bl	sub_080f7f30
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
.L_080f8670:
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
	beq.n	.L_080f868a
	b.n	.L_080f7ff6
.L_080f868a:
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
	bl	sub_080f7f30
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
