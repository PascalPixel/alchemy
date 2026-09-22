.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_080030f8, 0x080030f8
	.set sub_0800352c, 0x0800352c
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016498, 0x08016498
	.set sub_08016738, 0x08016738
	.set sub_08017aa4, 0x08017aa4
	.set sub_08019000, 0x08019000
	.set sub_0801965c, 0x0801965c
	.set sub_0801a3d0, 0x0801a3d0
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801e8b0, 0x0801e8b0
	.set sub_0801e9d4, 0x0801e9d4
	.set sub_0801ea08, 0x0801ea08
	.set sub_080218dc, 0x080218dc
	.set sub_08021af0, 0x08021af0
	.set sub_08022768, 0x08022768
	.set sub_08025180, 0x08025180
	.set sub_080251d4, 0x080251d4
	.set sub_08026a5a, 0x08026a5a
	.set sub_08026a64, 0x08026a64
	.set sub_08026a6a, 0x08026a6a
	.set sub_08026a72, 0x08026a72
	.set sub_08026a84, 0x08026a84
	.set sub_08026b8c, 0x08026b8c
	.set sub_08026b96, 0x08026b96
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077080, 0x08077080
	.set sub_080770c0, 0x080770c0
	.set sub_080b50b8, 0x080b50b8
	.set sub_080b50e0, 0x080b50e0
	.set sub_080f9010, 0x080f9010
	.global Overlay_08025200
Overlay_08025200:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #304
	str	r0, [sp, #80]
	str	r2, [sp, #72]
	str	r1, [sp, #76]
	ldr	r5, [pc, #164]
	movs	r2, #1
	ldr	r1, [r5, #0]
	negs	r2, r2
	movs	r0, #128
	str	r1, [sp, #68]
	str	r2, [sp, #64]
	str	r2, [sp, #60]
	bl	sub_080040b4
	movs	r3, #42
	str	r0, [sp, #56]
	str	r3, [sp, #0]
	movs	r1, #5
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_080162d4
	movs	r3, #0
	str	r0, [sp, #52]
	str	r3, [sp, #48]
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #52]
	ldr	r2, [r3, #48]
	ldr	r3, [r3, #56]
	mov	r9, r1
	mov	r8, r2
	str	r3, [sp, #44]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r3, #11
	movs	r0, #15
	movs	r2, #15
	bl	sub_080162d4
	mov	r3, sp
	adds	r3, #84
	ldr	r1, [pc, #88]
	movs	r6, #128
	str	r3, [sp, #16]
	mov	fp, r0
	movs	r7, #0
	mov	ip, r1
	adds	r4, r3, #0
	lsls	r6, r6, #23
	movs	r5, #0
.L_08025278:
	lsls	r0, r7, #1
	str	r6, [r4, #4]
	str	r5, [r4, #8]
	mov	r3, fp
	ldrh	r2, [r3, #12]
	ldr	r3, [pc, #52]
	lsls	r2, r2, #3
	ldrh	r1, [r4, #6]
	adds	r2, #8
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	mov	r1, fp
	strh	r3, [r4, #6]
	ldrh	r3, [r1, #14]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r7, #1
	strb	r0, [r4, #4]
	adds	r4, #12
	cmp	r7, #4
	ble.n	.L_08025278
	mov	r2, sp
	adds	r2, #144
	ldr	r3, [pc, #20]
	str	r2, [sp, #28]
	ldr	r6, [sp, #16]
	str	r2, [sp, #8]
	movs	r5, #8
	b.n	.L_080252c8
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_080252c8:
	mov	sl, r3
	movs	r7, #4
.L_080252cc:
	movs	r0, #128
	bl	sub_080040b4
	ldr	r2, [sp, #8]
	stmia	r2!, {r0}
	adds	r1, r2, #0
	str	r1, [sp, #8]
	movs	r1, #1
	negs	r1, r1
	bl	sub_080040d0
	ldr	r3, [pc, #52]
	ands	r0, r3
	ldrh	r3, [r5, r6]
	mov	r1, sl
	ands	r3, r1
	orrs	r3, r0
	subs	r7, #1
	strh	r3, [r5, r6]
	adds	r5, #12
	cmp	r7, #0
	bge.n	.L_080252cc
	ldr	r5, [pc, #32]
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_080251d4
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl	sub_080251d4
	adds	r5, #1
	movs	r1, #132
	lsls	r1, r1, #2
	adds	r0, r5, #0
	b.n	.L_08025324
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0000f018
	.2byte 0x0201
	.2byte 0x0000
.L_08025324:
	bl	sub_080251d4
	ldr	r1, [pc, #148]
	adds	r0, r5, #0
	bl	sub_080251d4
	movs	r2, #146
	lsls	r2, r2, #1
	mov	r3, r8
	mov	r1, sp
	add	r2, sp
	lsls	r3, r3, #1
	adds	r1, #164
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	str	r1, [sp, #32]
.L_08025344:
	ldr	r2, [sp, #64]
	cmp	r9, r2
	bne.n	.L_08025352
	ldr	r3, [sp, #60]
	cmp	r8, r3
	bne.n	.L_08025352
	b.n	.L_0802552c
.L_08025352:
	ldr	r1, [sp, #68]
	ldr	r3, [pc, #108]
	adds	r2, r1, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [sp, #60]
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r3, #15
	adds	r1, #1
	str	r3, [sp, #0]
	adds	r0, #1
	subs	r2, #2
	movs	r3, #1
	bl	sub_08022768
	bl	sub_08016738
	ldr	r1, [sp, #72]
	cmp	r1, #0
	beq.n	.L_080253d0
	mov	r3, r9
	add	r3, r8
	ldr	r2, [sp, #76]
	lsls	r3, r3, #1
	adds	r5, r3, r2
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #80]
	bl	sub_08025180
	cmp	r0, #2
	bne.n	.L_080253a4
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #40]
	adds	r1, r5, #0
	b.n	.L_080253b2
.L_080253a4:
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #20]
	ldr	r5, [sp, #32]
	ands	r0, r3
	ldr	r3, [pc, #28]
	adds	r1, r5, #0
	adds	r0, r0, r3
.L_080253b2:
	movs	r2, #52
	bl	sub_0801965c
	b.n	.L_080253dc
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00000211
	.4byte 0x00000ea6
	.4byte 0x000008ee
	.2byte 0x0075
	.2byte 0x0000
.L_080253d0:
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #100]
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0801965c
.L_080253dc:
	ldr	r1, [sp, #52]
	movs	r3, #4
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_08017aa4
	ldr	r1, [sp, #64]
	mov	r3, r8
	str	r3, [sp, #60]
	cmp	r9, r1
	beq.n	.L_080254b2
	mov	r0, fp
	bl	sub_08016498
	mov	r2, r9
	ldr	r1, [sp, #76]
	lsls	r3, r2, #1
	adds	r3, r3, r1
	ldrh	r5, [r3, #0]
	movs	r7, #0
	cmp	r5, #0
	beq.n	.L_080254ac
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #16]
	adds	r6, r3, #0
	movs	r3, #8
	str	r3, [sp, #12]
	str	r1, [sp, #4]
	mov	sl, r2
.L_08025416:
	adds	r0, r5, #0
	bl	sub_08077018
	movs	r0, #15
	bl	sub_0801e71c
	adds	r1, r5, #0
	ldr	r0, [sp, #80]
	bl	sub_08025180
	cmp	r0, #0
	beq.n	.L_0802543c
	movs	r0, #4
	bl	sub_0801e71c
	b.n	.L_0802544c
	movs	r0, r0
	.2byte 0x08e5
	.2byte 0x0000
.L_0802543c:
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0802544c
	movs	r0, #2
	bl	sub_0801e71c
.L_0802544c:
	ldr	r0, [pc, #84]
	ldr	r3, [pc, #88]
	ands	r0, r5
	adds	r0, r0, r3
	mov	r1, fp
	lsls	r3, r7, #4
	movs	r2, #16
	bl	sub_0801e7c0
	movs	r0, #15
	bl	sub_0801e71c
	ldr	r3, [sp, #4]
	ldmia	r3!, {r1}
	adds	r0, r5, #0
	adds	r2, r3, #0
	str	r2, [sp, #4]
	bl	sub_08021af0
	ldr	r3, [pc, #40]
	ldr	r1, [sp, #12]
	mov	r2, sl
	ands	r0, r3
	ldrh	r3, [r1, r2]
	ldr	r2, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	mov	r2, sl
	strh	r3, [r1, r2]
	adds	r7, #1
	adds	r1, #12
	str	r1, [sp, #12]
	cmp	r7, #4
	bgt.n	.L_080254ac
	adds	r6, #2
	ldrh	r5, [r6, #0]
	cmp	r5, #0
	bne.n	.L_08025416
	b.n	.L_080254ac
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.2byte 0x0182
	.2byte 0x0000
.L_080254ac:
	mov	r3, r9
	str	r7, [sp, #48]
	str	r3, [sp, #64]
.L_080254b2:
	ldr	r1, [sp, #72]
	cmp	r1, #5
	ble.n	.L_080254fc
	movs	r7, #0
	adds	r1, #4
	mov	sl, r1
	b.n	.L_080254ee
.L_080254c0:
	ldr	r2, [pc, #120]
	mov	r0, r9
	movs	r1, #5
	adds	r6, r7, r2
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_080254d4
	ldr	r3, [pc, #108]
	adds	r6, r7, r3
.L_080254d4:
	mov	r1, fp
	ldrh	r2, [r1, #8]
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, fp
	adds	r1, r6, #0
	subs	r3, #1
	bl	sub_08019000
	adds	r7, #1
.L_080254ee:
	mov	r0, sl
	movs	r1, #5
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L_080254c0
.L_080254fc:
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldr	r2, [sp, #20]
	ldrh	r1, [r1, #14]
	mov	r3, fp
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	adds	r0, #1
	movs	r3, #1
	bl	sub_08022768
	ldr	r3, [pc, #40]
	ldr	r1, [sp, #68]
	adds	r2, r1, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #32]
	adds	r3, r1, r2
	movs	r1, #0
	strb	r1, [r3, #0]
.L_0802552c:
	ldr	r2, [sp, #72]
	cmp	r2, #5
	ble.n	.L_080255e4
	movs	r7, #0
	adds	r2, #4
	mov	sl, r2
	b.n	.L_08025592
	movs	r0, r0
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x00000ea3
	.2byte 0x0ea6
	.2byte 0x0000
.L_0802554c:
	ldr	r3, [pc, #284]
	ldr	r1, [pc, #288]
	adds	r6, r7, r3
	ldr	r3, [r1, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_0802556c
	mov	r0, r9
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_0802556c
	ldr	r2, [pc, #264]
	adds	r6, r7, r2
.L_0802556c:
	mov	r3, fp
	movs	r1, #5
	mov	r0, sl
	ldrh	r5, [r3, #8]
	bl	sub_080022ec
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r1, #0
	subs	r5, #2
	movs	r3, #1
	str	r1, [sp, #0]
	mov	r0, fp
	adds	r1, r6, #0
	adds	r2, r5, #0
	negs	r3, r3
	bl	sub_08019000
	adds	r7, #1
.L_08025592:
	mov	r0, sl
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	blt.n	.L_0802554c
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r5, #1
	negs	r5, r5
	subs	r2, r2, r0
	movs	r1, #0
	str	r1, [sp, #0]
	mov	r0, fp
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #196]
	bl	sub_08019000
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r1, #0
	str	r1, [sp, #0]
	subs	r2, #2
	mov	r0, fp
	ldr	r1, [pc, #180]
	adds	r3, r5, #0
	bl	sub_08019000
	ldr	r2, [sp, #68]
	ldr	r3, [pc, #176]
	adds	r1, r2, r3
	mov	r2, fp
	ldrh	r3, [r2, #14]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L_080255e4:
	ldr	r3, [sp, #48]
	cmp	r3, #0
	ble.n	.L_080255fe
	ldr	r5, [sp, #16]
	adds	r7, r3, #0
.L_080255ee:
	adds	r0, r5, #0
	movs	r1, #240
	subs	r7, #1
	bl	sub_08003dec
	adds	r5, #12
	cmp	r7, #0
	bne.n	.L_080255ee
.L_080255fe:
	mov	r1, fp
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	subs	r3, #2
	ldr	r2, [sp, #20]
	str	r3, [sp, #36]
	ldrh	r3, [r1, #14]
	adds	r3, r2, r3
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r1, [sp, #24]
	str	r3, [sp, #40]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #96]
	bl	sub_080040d0
	ldr	r3, [pc, #48]
	ldr	r1, [sp, #24]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #52]
	ldr	r2, [r3, #0]
	movs	r0, #4
	ldr	r1, [sp, #36]
	ands	r2, r0
	ldr	r3, [pc, #64]
	lsrs	r2, r2, #1
	adds	r2, r1, r2
	adds	r2, r2, r3
	ldr	r1, [sp, #24]
	ldr	r3, [pc, #20]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	ands	r3, r1
	b.n	.L_0802568c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0000f301
	.4byte 0x03001e40
	.4byte 0x0000f30b
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.2byte 0xfffc
	.2byte 0x0000
.L_0802568c:
	orrs	r3, r2
	ldr	r1, [pc, #640]
	ldr	r2, [sp, #24]
	strh	r3, [r2, #6]
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #40]
	ands	r3, r0
	lsrs	r3, r3, #2
	subs	r3, r2, r3
	ldr	r1, [sp, #24]
	adds	r3, #248
	strb	r3, [r1, #4]
	ldr	r2, [sp, #72]
	cmp	r2, #0
	beq.n	.L_080256b2
	ldr	r0, [sp, #24]
	movs	r1, #242
	bl	sub_08003dec
.L_080256b2:
	ldr	r3, [pc, #608]
	ldr	r1, [r3, #0]
	mov	r2, r8
	mov	r3, r9
	str	r3, [r1, #52]
	str	r2, [r1, #48]
	ldr	r3, [sp, #44]
	str	r3, [r1, #56]
	ldr	r0, [pc, #596]
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025754
	ldr	r1, [sp, #72]
	cmp	r1, #0
	beq.n	.L_0802574c
	mov	r2, r9
	add	r2, r8
	ldr	r1, [sp, #76]
	lsls	r3, r2, #1
	adds	r5, r3, r1
	ldrh	r0, [r5, #0]
	movs	r7, #128
	mov	sl, r2
	lsls	r7, r7, #3
	bl	sub_08077018
	ldrh	r2, [r5, #0]
	adds	r3, r7, #0
	ands	r3, r2
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_08025706
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #80]
	bl	sub_08025180
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_08025706
	b.n	.L_080258c8
.L_08025706:
	movs	r0, #114
	bl	sub_080f9010
	cmp	r6, #2
	bne.n	.L_08025716
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #520]
	b.n	.L_08025724
.L_08025716:
	ldrh	r2, [r5, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802572e
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #508]
.L_08025724:
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0801965c
	b.n	.L_0802573a
.L_0802572e:
	ldr	r5, [sp, #32]
	ldr	r0, [pc, #496]
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0801965c
.L_0802573a:
	bl	sub_08016738
	adds	r0, r5, #0
	ldr	r1, [sp, #52]
	movs	r2, #0
	movs	r3, #4
	bl	sub_08017aa4
	b.n	.L_08025772
.L_0802574c:
	movs	r2, #1
	negs	r2, r2
	mov	sl, r2
	b.n	.L_080258c8
.L_08025754:
	ldr	r3, [r1, #76]
	cmp	r3, #0
	beq.n	.L_08025764
	ldr	r3, [r0, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025772
.L_08025764:
	movs	r0, #113
	bl	sub_080f9010
	movs	r3, #1
	negs	r3, r3
	mov	sl, r3
	b.n	.L_080258c8
.L_08025772:
	ldr	r1, [sp, #72]
	cmp	r1, #0
	bne.n	.L_0802577a
	b.n	.L_080258c0
.L_0802577a:
	ldr	r1, [pc, #428]
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080257b0
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #5
	beq.n	.L_080257a0
	mov	r3, r9
	ldr	r1, [sp, #72]
	add	r3, r8
	cmp	r3, r1
	bne.n	.L_080257a4
.L_080257a0:
	movs	r2, #0
	mov	r8, r2
.L_080257a4:
	mov	r1, r8
	mov	r3, r8
	lsls	r1, r1, #1
	str	r3, [sp, #44]
	str	r1, [sp, #20]
	b.n	.L_080258c0
.L_080257b0:
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080257f6
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080257ec
	ldr	r0, [sp, #72]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	r9, r3
	bne.n	.L_080257e8
	ldr	r1, [sp, #72]
	mov	r2, r9
	subs	r3, r1, r2
	subs	r3, #1
	b.n	.L_080257ea
.L_080257e8:
	movs	r3, #4
.L_080257ea:
	mov	r8, r3
.L_080257ec:
	mov	r2, r8
	mov	r1, r8
	lsls	r2, r2, #1
	str	r1, [sp, #44]
	b.n	.L_080258be
.L_080257f6:
	ldr	r3, [r1, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025854
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r3, r9
	ldr	r1, [sp, #72]
	adds	r3, #5
	cmp	r3, r1
	blt.n	.L_08025828
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_080258c0
	ldr	r1, [sp, #44]
	mov	r8, r1
	mov	r2, r8
	movs	r3, #0
	lsls	r2, r2, #1
	mov	r9, r3
	b.n	.L_080258be
.L_08025828:
	ldr	r0, [sp, #72]
	mov	r9, r3
	ldr	r3, [sp, #44]
	subs	r0, #1
	movs	r1, #5
	mov	r8, r3
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	r9, r3
	bne.n	.L_080258ba
	ldr	r1, [sp, #72]
	mov	r2, r9
	subs	r3, r1, r2
	subs	r3, #1
	mov	r8, r3
	ldr	r3, [sp, #44]
	cmp	r8, r3
	ble.n	.L_080258aa
	mov	r8, r3
	b.n	.L_080258b2
.L_08025854:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080258c0
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_0802587e
	ldr	r1, [sp, #44]
	mov	r8, r1
	movs	r3, #5
	mov	r2, r8
	negs	r3, r3
	lsls	r2, r2, #1
	add	r9, r3
	b.n	.L_080258be
.L_0802587e:
	ldr	r0, [sp, #72]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	mov	r9, r3
	ldr	r3, [sp, #44]
	mov	r1, r9
	mov	r8, r3
	cmp	r1, #0
	beq.n	.L_080258b2
	ldr	r2, [sp, #72]
	subs	r3, r2, r1
	subs	r3, #1
	mov	r8, r3
	ldr	r3, [sp, #44]
	cmp	r8, r3
	ble.n	.L_080258ba
	mov	r8, r3
	b.n	.L_080258b2
.L_080258aa:
	mov	r3, r8
	lsls	r3, r3, #1
	str	r3, [sp, #20]
	b.n	.L_080258c0
.L_080258b2:
	mov	r1, r8
	lsls	r1, r1, #1
	str	r1, [sp, #20]
	b.n	.L_080258c0
.L_080258ba:
	mov	r2, r8
	lsls	r2, r2, #1
.L_080258be:
	str	r2, [sp, #20]
.L_080258c0:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_08025344
.L_080258c8:
	ldr	r0, [sp, #52]
	movs	r1, #1
	bl	sub_08016418
	movs	r1, #1
	mov	r0, fp
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #56]
	bl	sub_08003f3c
	ldr	r5, [sp, #28]
	movs	r7, #4
.L_080258e8:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	sub_08003f3c
	cmp	r7, #0
	bge.n	.L_080258e8
	movs	r0, #1
	bl	sub_080030f8
	mov	r0, sl
	add	sp, #304
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e40
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x000008ee
	.4byte 0x000008ec
	.4byte 0x000008eb
	.2byte 0x1b04
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #300
	str	r1, [sp, #72]
	str	r2, [sp, #68]
	ldr	r5, [pc, #172]
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	movs	r1, #1
	str	r0, [sp, #64]
	negs	r1, r1
	movs	r0, #128
	str	r1, [sp, #56]
	mov	r9, r1
	bl	sub_080040b4
	str	r0, [sp, #52]
	adds	r0, r6, #0
	bl	sub_08077008
	movs	r3, #42
	str	r0, [sp, #48]
	str	r3, [sp, #0]
	movs	r1, #5
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_080162d4
	movs	r2, #5
	str	r0, [sp, #44]
	str	r2, [sp, #40]
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #52]
	str	r0, [sp, #60]
	ldr	r1, [r3, #48]
	ldr	r3, [r3, #56]
	mov	sl, r1
	str	r3, [sp, #36]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r2, #21
	movs	r3, #11
	movs	r0, #9
	movs	r1, #9
	bl	sub_080162d4
	mov	r2, sp
	adds	r2, #80
	ldr	r3, [pc, #88]
	movs	r6, #128
	str	r2, [sp, #12]
	mov	fp, r0
	movs	r7, #0
	mov	ip, r3
	adds	r4, r2, #0
	lsls	r6, r6, #23
	movs	r5, #0
.L_080259ac:
	lsls	r0, r7, #1
	str	r6, [r4, #4]
	str	r5, [r4, #8]
	mov	r1, fp
	ldrh	r2, [r1, #12]
	ldr	r3, [pc, #52]
	lsls	r2, r2, #3
	ldrh	r1, [r4, #6]
	adds	r2, #8
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	mov	r2, fp
	strh	r3, [r4, #6]
	ldrh	r3, [r2, #14]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r7, #1
	strb	r0, [r4, #4]
	adds	r4, #12
	cmp	r7, #4
	ble.n	.L_080259ac
	mov	r3, sp
	adds	r3, #140
	ldr	r0, [pc, #20]
	str	r3, [sp, #8]
	ldr	r6, [sp, #12]
	str	r3, [sp, #4]
	movs	r5, #8
	b.n	.L_080259fc
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_080259fc:
	mov	r8, r0
	movs	r7, #4
.L_08025a00:
	movs	r0, #128
	bl	sub_080040b4
	ldr	r2, [sp, #4]
	stmia	r2!, {r0}
	adds	r1, r2, #0
	str	r1, [sp, #4]
	movs	r1, #1
	negs	r1, r1
	bl	sub_080040d0
	ldr	r3, [pc, #52]
	ands	r0, r3
	ldrh	r3, [r5, r6]
	mov	r1, r8
	ands	r3, r1
	orrs	r3, r0
	subs	r7, #1
	strh	r3, [r5, r6]
	adds	r5, #12
	cmp	r7, #0
	bge.n	.L_08025a00
	ldr	r5, [pc, #32]
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_080251d4
	adds	r0, r5, #0
	ldr	r1, [pc, #24]
	bl	sub_080251d4
	adds	r5, #1
	movs	r1, #132
	lsls	r1, r1, #2
	adds	r0, r5, #0
	b.n	.L_08025a58
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0000f018
	.2byte 0x0201
	.2byte 0x0000
.L_08025a58:
	bl	sub_080251d4
	ldr	r1, [pc, #120]
	adds	r0, r5, #0
	bl	sub_080251d4
	movs	r2, #144
	lsls	r2, r2, #1
	mov	r3, sl
	add	r2, sp
	lsls	r3, r3, #1
	str	r2, [sp, #24]
	str	r3, [sp, #20]
.L_08025a72:
	ldr	r0, [sp, #60]
	cmp	r0, r9
	bne.n	.L_08025a80
	ldr	r1, [sp, #56]
	cmp	sl, r1
	bne.n	.L_08025a80
	b.n	.L_08025cd6
.L_08025a80:
	ldr	r3, [sp, #64]
	ldr	r0, [pc, #88]
	adds	r2, r3, r0
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [sp, #56]
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, fp
	ldrh	r2, [r3, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	movs	r3, #1
	bl	sub_08022768
	bl	sub_08016738
	ldr	r0, [sp, #68]
	cmp	r0, #0
	beq.n	.L_08025ae4
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #72]
	add	r3, sl
	lsls	r3, r3, #1
	ldrh	r3, [r3, r1]
	ldr	r0, [pc, #20]
	ands	r0, r3
	ldr	r3, [pc, #28]
	add	r5, sp, #160
	adds	r0, r0, r3
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0801965c
	b.n	.L_08025af0
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x00000211
	.4byte 0x00000ea6
	.2byte 0x053a
	.2byte 0x0000
.L_08025ae4:
	add	r5, sp, #160
	ldr	r0, [pc, #180]
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0801965c
.L_08025af0:
	movs	r2, #0
	movs	r3, #4
	adds	r0, r5, #0
	ldr	r1, [sp, #44]
	bl	sub_08017aa4
	ldr	r3, [sp, #60]
	mov	r2, sl
	str	r2, [sp, #56]
	cmp	r3, r9
	bne.n	.L_08025b08
	b.n	.L_08025c5c
.L_08025b08:
	mov	r0, fp
	bl	sub_08016498
	ldr	r0, [sp, #60]
	ldr	r1, [sp, #72]
	lsls	r3, r0, #1
	ldrh	r5, [r3, r1]
	movs	r7, #0
	cmp	r5, #0
	bne.n	.L_08025b1e
	b.n	.L_08025c56
.L_08025b1e:
	mov	r2, sp
	adds	r2, #76
	str	r2, [sp, #16]
.L_08025b24:
	adds	r0, r5, #0
	bl	sub_08077080
	mov	r8, r0
	movs	r0, #0
	str	r0, [sp, #0]
	lsls	r3, r7, #1
	mov	r0, fp
	ldr	r1, [pc, #104]
	movs	r2, #11
	mov	r9, r3
	bl	sub_08019000
	movs	r1, #0
	str	r1, [sp, #0]
	mov	r0, fp
	ldr	r1, [pc, #92]
	movs	r2, #12
	mov	r3, r9
	bl	sub_08019000
	ldr	r3, [sp, #8]
	ldr	r0, [pc, #84]
	lsls	r2, r7, #2
	adds	r2, r3, r2
	movs	r3, #1
	ands	r0, r5
	str	r3, [sp, #0]
	movs	r1, #0
	ldr	r3, [sp, #16]
	bl	sub_0801a3d0
	mov	r0, r9
	adds	r1, r0, r7
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #40]
	lsls	r1, r1, #2
	ldr	r0, [sp, #76]
	adds	r1, #8
	ands	r0, r3
	ldrh	r3, [r2, r1]
	ldr	r2, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #12]
	strh	r3, [r0, r1]
	mov	r1, r8
	ldrb	r2, [r1, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08025bac
	movs	r0, #4
	bl	sub_0801e71c
	b.n	.L_08025bd4
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000008e7
	.4byte 0x0000f01f
	.4byte 0x0000f01e
	.2byte 0x3fff
	.2byte 0x0000
.L_08025bac:
	ldr	r1, [sp, #48]
	mov	r3, r8
	ldrb	r2, [r3, #9]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	ble.n	.L_08025bc2
	movs	r0, #2
	bl	sub_0801e71c
	b.n	.L_08025bd4
.L_08025bc2:
	ldr	r2, [sp, #48]
	ldr	r0, [pc, #284]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08025bd4
	movs	r0, #9
	bl	sub_0801e71c
.L_08025bd4:
	ldr	r1, [sp, #64]
	ldr	r2, [pc, #272]
	ldr	r0, [pc, #272]
	adds	r6, r1, r2
	adds	r0, r5, r0
	movs	r3, #5
	lsls	r5, r7, #4
	strb	r3, [r6, #0]
	mov	r1, fp
	movs	r2, #16
	adds	r3, r5, #0
	bl	sub_0801e7c0
	mov	r3, r8
	ldrb	r0, [r3, #9]
	movs	r1, #2
	movs	r3, #104
	mov	r2, fp
	str	r5, [sp, #0]
	bl	sub_0801e9d4
	movs	r0, #15
	bl	sub_0801e71c
	movs	r3, #15
	mov	r0, r8
	strb	r3, [r6, #0]
	ldrb	r3, [r0, #2]
	cmp	r3, #4
	beq.n	.L_08025c24
	ldr	r2, [pc, #220]
	adds	r1, r3, #0
	movs	r3, #0
	adds	r1, r1, r2
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r2, #15
	mov	r3, r9
	bl	sub_08019000
.L_08025c24:
	mov	r0, r8
	ldrb	r3, [r0, #8]
	cmp	r3, #255
	bne.n	.L_08025c30
	movs	r3, #11
	b.n	.L_08025c32
.L_08025c30:
	subs	r3, #1
.L_08025c32:
	movs	r1, #0
	str	r1, [sp, #0]
	mov	r0, fp
	movs	r1, #16
	mov	r2, r9
	adds	r7, #1
	bl	sub_080218dc
	cmp	r7, #4
	bgt.n	.L_08025c56
	ldr	r2, [sp, #60]
	ldr	r0, [sp, #72]
	adds	r3, r2, r7
	lsls	r3, r3, #1
	ldrh	r5, [r3, r0]
	cmp	r5, #0
	beq.n	.L_08025c56
	b.n	.L_08025b24
.L_08025c56:
	ldr	r1, [sp, #60]
	str	r7, [sp, #40]
	mov	r9, r1
.L_08025c5c:
	ldr	r2, [sp, #68]
	cmp	r2, #5
	ble.n	.L_08025ca6
	movs	r7, #0
	adds	r2, #4
	mov	r8, r2
	b.n	.L_08025c98
.L_08025c6a:
	ldr	r3, [pc, #136]
	ldr	r0, [sp, #60]
	movs	r1, #5
	adds	r6, r7, r3
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_08025c7e
	ldr	r0, [pc, #124]
	adds	r6, r7, r0
.L_08025c7e:
	mov	r1, fp
	ldrh	r2, [r1, #8]
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, fp
	adds	r1, r6, #0
	subs	r3, #1
	bl	sub_08019000
	adds	r7, #1
.L_08025c98:
	mov	r0, r8
	movs	r1, #5
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L_08025c6a
.L_08025ca6:
	mov	r1, fp
	ldrh	r0, [r1, #12]
	ldr	r2, [sp, #20]
	ldrh	r1, [r1, #14]
	mov	r3, fp
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	movs	r3, #1
	bl	sub_08022768
	ldr	r1, [pc, #52]
	ldr	r0, [sp, #64]
	movs	r3, #1
	adds	r2, r0, r1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #48]
	adds	r3, r0, r2
	movs	r0, #0
	strb	r0, [r3, #0]
.L_08025cd6:
	ldr	r1, [sp, #68]
	cmp	r1, #5
	ble.n	.L_08025d98
	movs	r7, #0
	adds	r1, #4
	mov	r8, r1
	b.n	.L_08025d48
	.4byte 0x0000013d
	.4byte 0x00000ea7
	.4byte 0x00000333
	.4byte 0x00005001
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x00000ea3
	.2byte 0x0ea6
	.2byte 0x0000
.L_08025d04:
	ldr	r0, [pc, #280]
	ldr	r2, [pc, #284]
	ldr	r3, [r0, #0]
	adds	r6, r7, r2
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_08025d24
	ldr	r0, [sp, #60]
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_08025d24
	ldr	r1, [pc, #260]
	adds	r6, r7, r1
.L_08025d24:
	mov	r2, fp
	movs	r1, #5
	mov	r0, r8
	ldrh	r5, [r2, #8]
	bl	sub_080022ec
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r3, #0
	subs	r5, #2
	str	r3, [sp, #0]
	mov	r0, fp
	adds	r1, r6, #0
	adds	r2, r5, #0
	subs	r3, #1
	bl	sub_08019000
	adds	r7, #1
.L_08025d48:
	mov	r0, r8
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	blt.n	.L_08025d04
	mov	r1, fp
	ldrh	r2, [r1, #8]
	movs	r5, #1
	negs	r5, r5
	subs	r2, r2, r0
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r0, fp
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #192]
	bl	sub_08019000
	mov	r0, fp
	ldrh	r2, [r0, #8]
	movs	r1, #0
	str	r1, [sp, #0]
	subs	r2, #2
	ldr	r1, [pc, #180]
	adds	r3, r5, #0
	bl	sub_08019000
	ldr	r2, [sp, #64]
	ldr	r3, [pc, #176]
	mov	r0, fp
	adds	r1, r2, r3
	ldrh	r3, [r0, #14]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L_08025d98:
	ldr	r1, [sp, #40]
	cmp	r1, #0
	ble.n	.L_08025db2
	ldr	r5, [sp, #12]
	adds	r7, r1, #0
.L_08025da2:
	adds	r0, r5, #0
	movs	r1, #240
	subs	r7, #1
	bl	sub_08003dec
	adds	r5, #12
	cmp	r7, #0
	bne.n	.L_08025da2
.L_08025db2:
	mov	r2, fp
	ldrh	r3, [r2, #12]
	lsls	r3, r3, #3
	subs	r3, #4
	ldr	r0, [sp, #20]
	str	r3, [sp, #28]
	ldrh	r3, [r2, #14]
	adds	r3, r0, r3
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r1, [sp, #24]
	str	r3, [sp, #32]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #96]
	bl	sub_080040d0
	ldr	r3, [pc, #48]
	ldr	r1, [sp, #24]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #48]
	ldr	r2, [r3, #0]
	movs	r0, #4
	ands	r2, r0
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #64]
	lsrs	r2, r2, #1
	adds	r2, r1, r2
	adds	r2, r2, r3
	ldr	r1, [sp, #24]
	ldr	r3, [pc, #20]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	ands	r3, r1
	b.n	.L_08025e40
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001e40
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.2byte 0xfffc
	.2byte 0x0000
.L_08025e40:
	orrs	r3, r2
	ldr	r1, [pc, #556]
	ldr	r2, [sp, #24]
	strh	r3, [r2, #6]
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #32]
	ands	r3, r0
	lsrs	r3, r3, #2
	subs	r3, r2, r3
	ldr	r0, [sp, #24]
	adds	r3, #248
	strb	r3, [r0, #4]
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L_08025e66
	ldr	r0, [sp, #24]
	movs	r1, #242
	bl	sub_08003dec
.L_08025e66:
	ldr	r3, [pc, #524]
	ldr	r2, [sp, #60]
	ldr	r1, [r3, #0]
	mov	r3, sl
	str	r2, [r1, #52]
	str	r3, [r1, #48]
	ldr	r0, [sp, #36]
	str	r0, [r1, #56]
	ldr	r0, [pc, #512]
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025eaa
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L_08025ea4
	ldr	r6, [sp, #60]
	ldr	r2, [sp, #72]
	add	r6, sl
	lsls	r3, r6, #1
	ldrh	r0, [r3, r2]
	bl	sub_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025ea2
	b.n	.L_0802602a
.L_08025ea2:
	b.n	.L_08025ec6
.L_08025ea4:
	movs	r6, #1
	negs	r6, r6
	b.n	.L_0802602a
.L_08025eaa:
	ldr	r3, [r1, #76]
	cmp	r3, #0
	beq.n	.L_08025eba
	ldr	r3, [r0, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025ec6
.L_08025eba:
	movs	r0, #113
	movs	r6, #1
	bl	sub_080f9010
	negs	r6, r6
	b.n	.L_0802602a
.L_08025ec6:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L_08025ece
	b.n	.L_08026022
.L_08025ece:
	ldr	r1, [pc, #428]
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025f04
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #5
	beq.n	.L_08025ef4
	ldr	r3, [sp, #60]
	ldr	r2, [sp, #68]
	add	r3, sl
	cmp	r3, r2
	bne.n	.L_08025ef8
.L_08025ef4:
	movs	r3, #0
	mov	sl, r3
.L_08025ef8:
	mov	r1, sl
	mov	r0, sl
	lsls	r1, r1, #1
	str	r0, [sp, #36]
	str	r1, [sp, #20]
	b.n	.L_08026022
.L_08025f04:
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025f4e
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_08025f42
	ldr	r0, [sp, #68]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r0, [sp, #60]
	cmp	r0, r3
	bne.n	.L_08025f3e
	ldr	r1, [sp, #68]
	subs	r3, r1, r0
	subs	r3, #1
	mov	sl, r3
	b.n	.L_08025f42
.L_08025f3e:
	movs	r2, #4
	mov	sl, r2
.L_08025f42:
	mov	r0, sl
	mov	r3, sl
	lsls	r0, r0, #1
	str	r3, [sp, #36]
	str	r0, [sp, #20]
	b.n	.L_08026022
.L_08025f4e:
	ldr	r3, [r1, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08025fae
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #68]
	adds	r3, #5
	cmp	r3, r1
	blt.n	.L_08025f82
	ldr	r2, [sp, #60]
	cmp	r2, #0
	beq.n	.L_08026022
	ldr	r0, [sp, #36]
	mov	sl, r0
	mov	r1, sl
	movs	r3, #0
	lsls	r1, r1, #1
	str	r3, [sp, #60]
	str	r1, [sp, #20]
	b.n	.L_08026022
.L_08025f82:
	ldr	r0, [sp, #68]
	ldr	r2, [sp, #36]
	subs	r0, #1
	movs	r1, #5
	str	r3, [sp, #60]
	mov	sl, r2
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r0, [sp, #60]
	cmp	r0, r3
	bne.n	.L_08026004
	ldr	r1, [sp, #68]
	subs	r3, r1, r0
	subs	r3, #1
	ldr	r2, [sp, #36]
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L_0802600c
	mov	sl, r2
	b.n	.L_0802601c
.L_08025fae:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08026022
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	ldr	r0, [sp, #60]
	cmp	r0, #0
	beq.n	.L_08025fd8
	ldr	r1, [sp, #36]
	mov	sl, r1
	mov	r2, sl
	subs	r0, #5
	lsls	r2, r2, #1
	str	r0, [sp, #60]
	str	r2, [sp, #20]
	b.n	.L_08026022
.L_08025fd8:
	ldr	r0, [sp, #68]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	str	r3, [sp, #60]
	ldr	r0, [sp, #60]
	ldr	r3, [sp, #36]
	mov	sl, r3
	cmp	r0, #0
	beq.n	.L_08026014
	ldr	r1, [sp, #68]
	subs	r3, r1, r0
	subs	r3, #1
	ldr	r2, [sp, #36]
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L_0802601c
	mov	sl, r2
	b.n	.L_0802601c
.L_08026004:
	mov	r0, sl
	lsls	r0, r0, #1
	str	r0, [sp, #20]
	b.n	.L_08026022
.L_0802600c:
	mov	r1, sl
	lsls	r1, r1, #1
	str	r1, [sp, #20]
	b.n	.L_08026022
.L_08026014:
	mov	r2, sl
	lsls	r2, r2, #1
	str	r2, [sp, #20]
	b.n	.L_08026022
.L_0802601c:
	mov	r3, sl
	lsls	r3, r3, #1
	str	r3, [sp, #20]
.L_08026022:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_08025a72
.L_0802602a:
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_08016418
	mov	r0, fp
	movs	r1, #1
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	ldr	r5, [sp, #8]
	movs	r7, #4
.L_08026044:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	sub_08003f3c
	cmp	r7, #0
	bge.n	.L_08026044
	ldr	r0, [sp, #52]
	bl	sub_08003f3c
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r6, #0
	add	sp, #300
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e40
	.4byte 0x03001f34
	.4byte 0x03001c94
	.2byte 0x1b04
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #324
	str	r2, [sp, #84]
	str	r3, [sp, #80]
	ldr	r5, [pc, #272]
	mov	sl, r0
	ldr	r0, [r5, #0]
	ldr	r2, [pc, #272]
	str	r0, [sp, #76]
	movs	r0, #128
	mov	r8, r1
	lsls	r0, r0, #1
	movs	r1, #0
	str	r1, [sp, #64]
	str	r2, [sp, #56]
	bl	sub_080040b4
	ldr	r4, [sp, #84]
	movs	r3, #0
	str	r0, [sp, #52]
	mov	r9, r3
	cmp	r4, #0
	bne.n	.L_080260be
	movs	r6, #1
	str	r6, [sp, #84]
.L_080260be:
	mov	r7, r8
	cmp	r7, #2
	beq.n	.L_080260c8
	cmp	r7, #4
	bne.n	.L_080260d4
.L_080260c8:
	adds	r3, r5, #0
	adds	r3, #192
	ldr	r2, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	b.n	.L_080260dc
.L_080260d4:
	adds	r3, r5, #0
	adds	r3, #192
	ldr	r2, [r3, #0]
	movs	r3, #16
.L_080260dc:
	str	r3, [r2, #40]
	mov	r0, sp
	adds	r0, #212
	mov	r3, sp
	str	r0, [sp, #36]
	movs	r2, #0
	movs	r7, #5
	adds	r3, #234
.L_080260ec:
	subs	r7, #1
	strb	r2, [r3, #0]
	subs	r3, #4
	cmp	r7, #0
	bge.n	.L_080260ec
	movs	r1, #1
	negs	r1, r1
	mov	r2, r8
	str	r1, [sp, #68]
	cmp	r2, #2
	bne.n	.L_0802613e
	ldr	r4, [sp, #76]
	movs	r3, #88
	ldrsh	r3, [r4, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L_08026194
	movs	r6, #154
	lsls	r6, r6, #1
	ldr	r0, [sp, #64]
	add	r6, sp
	adds	r2, r4, #0
	lsls	r3, r0, #1
	str	r6, [sp, #28]
	adds	r2, #88
	adds	r1, r3, r6
.L_08026120:
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldr	r3, [sp, #64]
	adds	r7, #1
	adds	r3, #1
	adds	r1, #2
	str	r3, [sp, #64]
	adds	r2, #2
	cmp	r7, #5
	bgt.n	.L_080261b8
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #255
	bne.n	.L_08026120
	b.n	.L_080261b8
.L_0802613e:
	mov	r5, r8
	cmp	r5, #4
	bne.n	.L_08026158
	movs	r6, #154
	lsls	r6, r6, #1
	add	r6, sp
	mov	r7, sl
	adds	r0, r6, #0
	movs	r1, #1
	str	r6, [sp, #28]
	strh	r7, [r0, #0]
	str	r1, [sp, #64]
	b.n	.L_080261b8
.L_08026158:
	ldr	r1, [sp, #76]
	movs	r3, #100
	adds	r1, #2
	ldrsh	r3, [r1, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L_080261b0
	movs	r3, #154
	lsls	r3, r3, #1
	add	r3, sp
	str	r3, [sp, #28]
	ldr	r4, [sp, #64]
	ldr	r5, [sp, #28]
	lsls	r3, r4, #1
	movs	r0, #100
	adds	r2, r3, r5
.L_08026178:
	ldrh	r3, [r1, r0]
	strh	r3, [r2, #0]
	ldr	r6, [sp, #64]
	adds	r7, #1
	adds	r6, #1
	adds	r2, #2
	str	r6, [sp, #64]
	adds	r0, #2
	cmp	r7, #5
	bgt.n	.L_080261b8
	ldrsh	r3, [r1, r0]
	cmp	r3, #255
	bne.n	.L_08026178
	b.n	.L_080261b8
.L_08026194:
	movs	r5, #154
	lsls	r5, r5, #1
	add	r5, sp
	str	r5, [sp, #28]
	b.n	.L_080261b8
.L_0802619e:
	ldr	r6, [sp, #28]
	mov	r7, fp
	ldrh	r6, [r6, r7]
	mov	sl, r6
	b.n	.L_080262b6
	.4byte 0x03001e74
	.2byte 0xffff
	.2byte 0x0000
.L_080261b0:
	movs	r0, #154
	lsls	r0, r0, #1
	add	r0, sp
	str	r0, [sp, #28]
.L_080261b8:
	ldr	r1, [sp, #64]
	ldr	r3, [pc, #44]
	ldr	r4, [sp, #28]
	lsls	r2, r1, #1
	mov	r5, r8
	strh	r3, [r4, r2]
	str	r1, [sp, #60]
	cmp	r5, #2
	beq.n	.L_080261cc
	b.n	.L_080262e0
.L_080261cc:
	ldr	r6, [sp, #84]
	cmp	r6, #255
	beq.n	.L_080262b6
	ldr	r7, [sp, #80]
	cmp	r7, #0
	beq.n	.L_080262b6
	movs	r5, #0
	movs	r7, #0
	cmp	r5, r1
	bge.n	.L_080262b6
	ldr	r4, [pc, #8]
	movs	r6, #0
	b.n	.L_080261f0
	movs	r0, r0
	.4byte 0x000000ff
	.2byte 0xffff
	.2byte 0x0000
.L_080261f0:
	ldr	r0, [sp, #28]
	ldrh	r3, [r6, r0]
	mov	fp, r6
	cmp	r3, #254
	beq.n	.L_080262ac
	adds	r0, r3, #0
	str	r4, [sp, #8]
	bl	sub_08077008
	ldr	r2, [sp, #80]
	adds	r1, r0, #0
	ldr	r4, [sp, #8]
	cmp	r2, #4
	beq.n	.L_08026240
	cmp	r2, #4
	bhi.n	.L_08026216
	cmp	r2, #3
	beq.n	.L_08026234
	b.n	.L_080262a6
.L_08026216:
	ldr	r3, [sp, #80]
	cmp	r3, #5
	beq.n	.L_08026222
	cmp	r3, #6
	beq.n	.L_08026264
	b.n	.L_080262a6
.L_08026222:
	movs	r0, #56
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_080262a6
	ldr	r1, [sp, #28]
	ldrh	r1, [r6, r1]
	movs	r7, #1
	mov	sl, r1
	b.n	.L_080262a6
.L_08026234:
	ldr	r2, [pc, #324]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L_080262a0
.L_08026240:
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #308]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080262a4
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080262a4
	adds	r0, #9
	adds	r3, r1, r0
	b.n	.L_0802629e
.L_08026264:
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #272]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080262a4
	movs	r0, #158
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrh	r3, [r3, #0]
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080262a4
	ldr	r2, [pc, #256]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080262a4
	subs	r0, #11
	adds	r3, r1, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080262a4
	subs	r2, #1
	adds	r3, r1, r2
.L_0802629e:
	ldrb	r3, [r3, #0]
.L_080262a0:
	cmp	r3, #0
	beq.n	.L_080262a6
.L_080262a4:
	movs	r7, #1
.L_080262a6:
	cmp	r7, #0
	beq.n	.L_080262ac
	b.n	.L_0802619e
.L_080262ac:
	ldr	r3, [sp, #64]
	adds	r5, #1
	adds	r6, #2
	cmp	r5, r3
	blt.n	.L_080261f0
.L_080262b6:
	ldr	r4, [sp, #64]
	movs	r5, #0
	cmp	r5, r4
	bge.n	.L_080262d8
	ldr	r6, [sp, #28]
	ldrh	r3, [r6, #0]
	cmp	r3, sl
	beq.n	.L_080262d8
	adds	r2, r6, #0
.L_080262c8:
	ldr	r7, [sp, #64]
	adds	r5, #1
	cmp	r5, r7
	bge.n	.L_080262d8
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, sl
	bne.n	.L_080262c8
.L_080262d8:
	ldr	r0, [sp, #64]
	cmp	r5, r0
	beq.n	.L_080262e0
	str	r5, [sp, #68]
.L_080262e0:
	ldr	r1, [sp, #68]
	cmp	r1, #0
	bge.n	.L_08026308
	ldr	r3, [sp, #64]
	subs	r3, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #68]
	b.n	.L_08026308
.L_080262f4:
	ldr	r2, [sp, #68]
	ldr	r4, [sp, #64]
	adds	r3, r2, r4
	subs	r3, #1
	adds	r0, r3, #0
	adds	r1, r4, #0
	str	r3, [sp, #68]
	bl	sub_080022fc
	str	r0, [sp, #68]
.L_08026308:
	ldr	r5, [sp, #68]
	lsls	r5, r5, #1
	str	r5, [sp, #24]
	ldr	r6, [sp, #28]
	ldrh	r3, [r6, r5]
	cmp	r3, #254
	beq.n	.L_080262f4
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_08026336
	mov	r7, r8
	cmp	r7, #1
	bne.n	.L_08026336
	ldrh	r0, [r6, r5]
	bl	sub_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_080262f4
.L_08026336:
	mov	r2, r8
	cmp	r2, #2
	beq.n	.L_08026354
	add	r5, sp, #200
	mov	r0, sl
	adds	r1, r5, #0
	bl	sub_080b50b8
	ldr	r4, [sp, #36]
	movs	r3, #8
	strb	r3, [r4, #2]
	ldr	r3, [r5, #0]
	strb	r3, [r4, #0]
	movs	r3, #128
	strb	r3, [r4, #1]
.L_08026354:
	movs	r3, #74
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #12
	movs	r2, #30
	movs	r3, #4
	bl	sub_080162d4
	movs	r6, #152
	mov	r5, sp
	mov	r7, sp
	adds	r5, #236
	add	r6, sp
	adds	r7, #88
	str	r0, [sp, #72]
	str	r5, [sp, #32]
	mov	fp, r6
	str	r7, [sp, #20]
	b.n	.L_0802638e
	movs	r0, r0
	.4byte 0x00000131
	.4byte 0xff0000ff
	.2byte 0x0141
	.2byte 0x0000
	ldr	r0, [sp, #68]
	lsls	r0, r0, #1
	str	r0, [sp, #24]
.L_0802638e:
	movs	r1, #0
	str	r1, [sp, #48]
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #28]
	mov	r1, fp
	ldrh	r0, [r2, r3]
	bl	sub_080b50b8
	ldr	r3, [pc, #72]
	ldr	r4, [sp, #32]
	str	r3, [r4, #4]
	ldr	r5, [sp, #48]
	str	r5, [r4, #8]
	ldr	r5, [pc, #64]
	ldr	r1, [r5, #0]
	movs	r3, #31
	lsrs	r1, r1, #2
	ands	r1, r3
	ldr	r3, [pc, #60]
	lsls	r1, r1, #8
	adds	r1, r1, r3
	ldr	r0, [sp, #52]
	bl	sub_080040d0
	ldr	r3, [pc, #36]
	ldr	r6, [sp, #32]
	ands	r0, r3
	ldrh	r2, [r6, #8]
	ldr	r3, [pc, #44]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [r5, #0]
	adds	r7, r6, #0
	strh	r3, [r7, #8]
	lsls	r0, r0, #12
	bl	sub_08002322
	cmp	r0, #0
	bge.n	.L_080263fc
	ldr	r1, [pc, #24]
	adds	r0, r0, r1
	b.n	.L_080263fc
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x40002000
	.4byte 0x03001e40
	.4byte 0x080346f8
	.4byte 0xfffffc00
	.2byte 0x7fff
	.2byte 0x0000
.L_080263fc:
	mov	r4, fp
	ldr	r3, [r4, #4]
	asrs	r2, r0, #15
	adds	r0, r3, r2
	str	r0, [r4, #4]
	ldr	r5, [sp, #36]
	movs	r1, #1
	ldrb	r2, [r5, #2]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802644e
	ldr	r4, [r4, #0]
	ldrb	r3, [r5, #0]
	adds	r3, r4, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r3, #1
	ldrb	r3, [r5, #1]
	adds	r3, r0, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L_08026436
	cmp	r3, #7
	ble.n	.L_0802643c
	b.n	.L_08026440
.L_08026436:
	subs	r3, r1, r4
	cmp	r3, #7
	bgt.n	.L_08026440
.L_0802643c:
	movs	r6, #1
	str	r6, [sp, #48]
.L_08026440:
	mov	r7, fp
	str	r1, [r7, #0]
	ldr	r2, [sp, #36]
	str	r0, [r7, #4]
	strb	r1, [r2, #0]
	strb	r0, [r2, #1]
	b.n	.L_08026486
.L_0802644e:
	movs	r4, #192
	lsls	r3, r2, #24
	lsls	r4, r4, #18
	cmp	r3, r4
	bhi.n	.L_08026468
	mov	r5, fp
	ldr	r6, [sp, #36]
	ldr	r3, [r5, #0]
	str	r0, [r5, #4]
	strb	r3, [r6, #0]
	strb	r0, [r6, #1]
	strb	r1, [r6, #2]
	b.n	.L_08026486
.L_08026468:
	ldr	r7, [sp, #36]
	ldrb	r3, [r7, #0]
	mov	r0, fp
	str	r3, [r0, #0]
	ldrb	r3, [r7, #1]
	str	r3, [r0, #4]
	adds	r3, r2, #0
	adds	r3, #252
	movs	r2, #192
	strb	r3, [r7, #2]
	lsls	r2, r2, #18
	lsls	r3, r3, #24
	cmp	r3, r2
	bhi.n	.L_08026486
	strb	r1, [r7, #2]
.L_08026486:
	mov	r3, fp
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #32]
	ldr	r3, [pc, #48]
	subs	r2, #8
	ldrh	r1, [r4, #6]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r1
	orrs	r3, r2
	adds	r5, r4, #0
	mov	r6, fp
	strh	r3, [r5, #6]
	ldr	r3, [r6, #4]
	subs	r3, #16
	strb	r3, [r5, #4]
	ldr	r0, [sp, #32]
	movs	r1, #240
	bl	sub_08003dec
	ldr	r7, [sp, #84]
	cmp	r7, #255
	bne.n	.L_080264d8
	ldr	r2, [pc, #16]
	ldr	r3, [sp, #88]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #1
	b.n	.L_080264cc
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0000
	.2byte 0xffff
.L_080264cc:
	.2byte 0x4313
	ldr	r2, [pc, #284]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #17
	b.n	.L_080264ea
.L_080264d8:
	ldr	r2, [pc, #276]
	ldr	r3, [sp, #88]
	ands	r3, r2
	movs	r2, #176
	orrs	r3, r2
	ldr	r2, [pc, #264]
	ands	r3, r2
	movs	r2, #176
	lsls	r2, r2, #16
.L_080264ea:
	orrs	r3, r2
	str	r3, [sp, #88]
	ldr	r0, [sp, #20]
	ldr	r3, [pc, #252]
	ldr	r2, [r0, #4]
	ands	r2, r3
	str	r2, [r0, #4]
	ldr	r0, [sp, #20]
	bl	sub_08003d28
	ldr	r1, [sp, #56]
	movs	r3, #1
	ands	r3, r1
	str	r0, [sp, #44]
	cmp	r3, #0
	bne.n	.L_0802650c
	b.n	sub_08026b96
.L_0802650c:
	movs	r2, #0
	str	r2, [sp, #64]
	ldr	r1, [sp, #36]
	movs	r0, #253
	movs	r7, #5
.L_08026516:
	ldrb	r2, [r1, #2]
	adds	r3, r0, #0
	ands	r3, r2
	subs	r7, #1
	strb	r3, [r1, #2]
	adds	r1, #4
	cmp	r7, #0
	bge.n	.L_08026516
	ldr	r3, [sp, #84]
	movs	r7, #0
	cmp	r7, r3
	bcs.n	.L_080265f4
	ldr	r6, [sp, #64]
	add	r4, sp, #172
	add	r0, sp, #324
	ldr	r1, [sp, #64]
	adds	r3, r6, r0
	mov	sl, r4
	ldr	r5, [sp, #28]
	ldr	r4, [sp, #36]
	adds	r6, r3, #0
	mov	r2, sl
	lsls	r3, r1, #1
	mov	lr, r5
	adds	r0, r4, #0
	adds	r5, r3, r2
	movs	r3, #254
	subs	r6, #160
	mov	r8, r3
	adds	r0, #24
.L_08026552:
	ldr	r1, [sp, #68]
	ldr	r2, [sp, #60]
	adds	r3, r1, r7
	cmp	r3, r2
	bge.n	.L_08026592
	lsls	r3, r3, #1
	mov	r1, lr
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #254
	beq.n	.L_08026592
	ldrb	r3, [r4, #2]
	strh	r2, [r5, #0]
	movs	r2, #2
	orrs	r2, r3
	movs	r3, #0
	orrs	r2, r3
	movs	r3, #3
	ldrsb	r3, [r4, r3]
	strb	r2, [r4, #2]
	cmp	r3, r7
	beq.n	.L_08026586
	mov	r1, r8
	ands	r2, r1
	strb	r2, [r4, #2]
	strb	r7, [r4, #3]
.L_08026586:
	strb	r7, [r6, #0]
	ldr	r2, [sp, #64]
	adds	r2, #1
	str	r2, [sp, #64]
	adds	r6, #1
	adds	r5, #2
.L_08026592:
	cmp	r7, #0
	beq.n	.L_080265de
	ldr	r1, [sp, #68]
	subs	r3, r1, r7
	cmp	r3, #0
	blt.n	.L_080265de
	lsls	r3, r3, #1
	mov	r1, lr
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #254
	beq.n	.L_080265de
	strh	r2, [r5, #0]
	movs	r2, #6
	ldrb	r3, [r0, #2]
	subs	r2, r2, r7
	mov	ip, r2
	movs	r2, #2
	orrs	r2, r3
	movs	r3, #0
	orrs	r2, r3
	movs	r3, #3
	ldrsb	r3, [r0, r3]
	negs	r1, r7
	strb	r2, [r0, #2]
	cmp	r3, r1
	beq.n	.L_080265d0
	mov	r3, r8
	ands	r2, r3
	strb	r2, [r0, #2]
	strb	r1, [r0, #3]
.L_080265d0:
	mov	r1, ip
	strb	r1, [r6, #0]
	ldr	r2, [sp, #64]
	adds	r2, #1
	str	r2, [sp, #64]
	adds	r6, #1
	adds	r5, #2
.L_080265de:
	ldr	r3, [sp, #84]
	adds	r7, #1
	adds	r4, #4
	subs	r0, #4
	cmp	r7, r3
	bcc.n	.L_08026552
	b.n	.L_080265f8
	.4byte 0x0000ffff
	.2byte 0x0000
	.2byte 0xffff
.L_080265f4:
	.2byte 0xac2b
	mov	sl, r4
.L_080265f8:
	ldr	r1, [sp, #36]
	movs	r4, #2
	movs	r0, #6
	movs	r7, #5
.L_08026600:
	ldrb	r2, [r1, #2]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802660c
	strb	r0, [r1, #3]
.L_0802660c:
	subs	r7, #1
	adds	r1, #4
	cmp	r7, #0
	bge.n	.L_08026600
	ldr	r5, [sp, #64]
	ldr	r2, [pc, #44]
	lsls	r3, r5, #1
	mov	r6, sl
	strh	r2, [r6, r3]
	mov	r0, sl
	movs	r1, #1
	bl	sub_080b50e0
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #24]
	ldrh	r3, [r7, r0]
	cmp	r3, #7
	bls.n	.L_08026632
	b.n	sub_08026a84
.L_08026632:
	ldr	r1, [sp, #84]
	cmp	r1, #255
	bne.n	.L_0802663a
	b.n	sub_08026b8c
.L_0802663a:
	ldr	r2, [sp, #80]
	cmp	r2, #0
	bne.n	.L_08026642
	b.n	sub_08026b8c
.L_08026642:
	b.n	.L_08026648
	.2byte 0x00ff
	.2byte 0x0000
.L_08026648:
	adds	r0, r3, #0
	bl	sub_08077008
	ldr	r3, [sp, #24]
	adds	r6, r0, #0
	mov	r1, fp
	ldrh	r0, [r7, r3]
	bl	sub_080b50b8
	mov	r4, r9
	cmp	r4, #0
	beq.n	.L_08026668
	mov	r0, r9
	movs	r1, #1
	bl	sub_08016418
.L_08026668:
	ldr	r3, [sp, #80]
	subs	r3, #1
	cmp	r3, #6
	bls.n	.L_08026672
	b.n	sub_08026b8c
.L_08026672:
	ldr	r2, [pc, #616]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	str	r0, [r1, #108]
	lsrs	r2, r0, #32
	str	r6, [r3, #112]
	lsrs	r2, r0, #32
	str	r4, [r6, #120]
	lsrs	r2, r0, #32
	str	r0, [r7, #124]
	lsrs	r2, r0, #32
	str	r0, [r0, #120]
	lsrs	r2, r0, #32
	ldr	r4, [r3, #16]
	lsrs	r2, r0, #32
	str	r0, [r3, #104]
	lsrs	r2, r0, #32
	mov	r5, fp
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_080266a2
	adds	r3, #7
.L_080266a2:
	asrs	r3, r3, #3
	subs	r0, r3, #4
	adds	r3, #4
	cmp	r3, #29
	ble.n	.L_080266ae
	movs	r0, #22
.L_080266ae:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #9
	movs	r3, #3
	bl	sub_080162d4
	mov	r9, r0
	movs	r0, #2
	bl	sub_0801e71c
	ldr	r0, [pc, #536]
	b.n	sub_08026a72
	mov	r7, fp
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_080266d2
	adds	r3, #7
.L_080266d2:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #6
	cmp	r3, #29
	ble.n	.L_080266de
	movs	r0, #17
.L_080266de:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #13
	movs	r3, #3
	bl	sub_080162d4
	mov	r9, r0
	mov	r1, r9
	ldr	r0, [pc, #496]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r5, #0
	movs	r1, #56
	ldrsh	r0, [r6, r1]
	mov	r2, r9
	movs	r1, #4
	movs	r3, #16
	str	r5, [sp, #0]
	bl	sub_0801ea08
	movs	r2, #48
	ldr	r0, [pc, #472]
	mov	r1, r9
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r2, #52
	ldrsh	r0, [r6, r2]
	b.n	.L_08026772
	mov	r4, fp
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_08026728
	adds	r3, #7
.L_08026728:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #6
	cmp	r3, #29
	ble.n	.L_08026734
	movs	r0, #17
.L_08026734:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #13
	movs	r3, #3
	bl	sub_080162d4
	mov	r9, r0
	mov	r1, r9
	ldr	r0, [pc, #420]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r5, #58
	ldrsh	r0, [r6, r5]
	movs	r1, #4
	movs	r5, #0
	mov	r2, r9
	movs	r3, #16
	str	r5, [sp, #0]
	bl	sub_0801ea08
	ldr	r0, [pc, #388]
	mov	r1, r9
	movs	r2, #48
	movs	r3, #0
	bl	sub_0801e8b0
	movs	r7, #54
	ldrsh	r0, [r6, r7]
.L_08026772:
	movs	r1, #4
	mov	r2, r9
	movs	r3, #56
	str	r5, [sp, #0]
	bl	sub_0801ea08
	b.n	sub_08026b8c
	mov	r0, fp
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0802678a
	adds	r3, #7
.L_0802678a:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #5
	cmp	r3, #29
	ble.n	.L_08026796
	movs	r0, #18
.L_08026796:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #3
	movs	r2, #12
	bl	sub_080162d4
	movs	r1, #56
	ldrsh	r3, [r6, r1]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_080267b0
	b.n	sub_08026a6a
.L_080267b0:
	ldr	r0, [pc, #316]
	b.n	.L_080267ec
	mov	r2, fp
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_080267be
	adds	r3, #7
.L_080267be:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #5
	cmp	r3, #29
	ble.n	.L_080267ca
	movs	r0, #18
.L_080267ca:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #3
	movs	r2, #12
	bl	sub_080162d4
	ldr	r4, [pc, #280]
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L_080267ea
	b.n	sub_08026a6a
.L_080267ea:
	ldr	r0, [pc, #268]
.L_080267ec:
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e7c0
	b.n	sub_08026b8c
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r7, r6, r0
	ldrb	r3, [r7, #0]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08026808
	movs	r5, #1
.L_08026808:
	ldr	r1, [pc, #240]
	adds	r1, r1, r6
	ldrb	r3, [r1, #0]
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_08026816
	adds	r5, #1
.L_08026816:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r2, r2, r6
	ldrb	r3, [r2, #0]
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_08026826
	adds	r5, #1
.L_08026826:
	ldr	r3, [pc, #216]
	adds	r3, r6, r3
	str	r3, [sp, #40]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08026834
	adds	r5, #1
.L_08026834:
	ldr	r4, [pc, #204]
	adds	r6, r6, r4
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_08026840
	adds	r5, #1
.L_08026840:
	cmp	r5, #0
	bne.n	.L_08026846
	movs	r5, #1
.L_08026846:
	movs	r3, #9
	subs	r1, r3, r5
	cmp	r1, #3
	bgt.n	.L_08026850
	movs	r1, #4
.L_08026850:
	mov	r0, fp
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bge.n	.L_0802685a
	adds	r3, #7
.L_0802685a:
	asrs	r3, r3, #3
	subs	r0, r3, #7
	adds	r3, #9
	cmp	r3, #29
	ble.n	.L_08026866
	movs	r0, #14
.L_08026866:
	movs	r2, #6
	adds	r3, r5, #2
	str	r2, [sp, #0]
	movs	r2, #16
	bl	sub_080162d4
	ldrb	r3, [r7, #0]
	mov	r9, r0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0802688a
	ldr	r0, [pc, #136]
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e7c0
	movs	r5, #1
.L_0802688a:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080268a0
	lsls	r3, r5, #3
	ldr	r0, [pc, #116]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_080268a0:
	mov	r2, sl
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080268b6
	lsls	r3, r5, #3
	ldr	r0, [pc, #100]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_080268b6:
	ldr	r4, [sp, #40]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_080268cc
	lsls	r3, r5, #3
	ldr	r0, [pc, #80]
	mov	r1, r9
	movs	r2, #0
	bl	sub_0801e7c0
	adds	r5, #1
.L_080268cc:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_080268d4
	b.n	sub_08026a64
.L_080268d4:
	lsls	r3, r5, #3
	ldr	r0, [pc, #64]
	b.n	sub_08026a5a
