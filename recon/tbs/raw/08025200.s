.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080030f8, 0x080030f8
	.set sub_0800352c, 0x0800352c
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
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_08021af0, 0x08021af0
	.set sub_08022768, 0x08022768
	.set sub_08025180, 0x08025180
	.set sub_080251d4, 0x080251d4
	.set sub_08077018, 0x08077018
	.set sub_080f9010, 0x080f9010
	.global ItemList_SelectEntry
	.global Func_08025200
	.thumb_func
ItemList_SelectEntry:
Func_08025200:
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
