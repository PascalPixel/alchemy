.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020082f9, 0x02008039, 0x02008069, 0x02008071, 0x0200815d, 0x02008041, 0x0200830d
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8764
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000058
	ldr	r0, [pc, #12]
.L_02000058:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000038
	.2byte 0x8794
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x87c4
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000088
	ldr	r0, [pc, #12]
	b.n	.L_0200008a
.L_02000088:
	ldr	r0, [pc, #12]
.L_0200008a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000037
	.4byte 0x02008fa0
	.2byte 0x90c0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x02008648
	movs	r0, #0
	bl 0x020086d0
	movs	r5, #8
.L_020000b0:
	adds	r0, r5, #0
	bl 0x02008660
	cmp	r0, #0
	beq.n	.L_020000c2
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_020000c2:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_020000b0
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r7, r6, r2
	movs	r3, #0
	ldrsh	r5, [r7, r3]
	movs	r0, #158
	bl 0x020086f0
	subs	r5, #1
	ldr	r0, [pc, #104]
	lsls	r4, r5, #3
	adds	r3, r4, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r4]
	bl 0x02008610
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	ldr	r0, [r6, #0]
	lsls	r2, r2, #7
	bl 0x02008668
	ldr	r0, [r6, #0]
	bl 0x02008660
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #2
	ldr	r0, [r6, #0]
	bl 0x02008690
	cmp	r5, #4
	beq.n	.L_0200012c
	movs	r2, #8
	ldr	r0, [r6, #0]
	movs	r1, #2
	negs	r2, r2
	bl 0x02008678
	movs	r0, #10
	bl 0x02008640
.L_0200012c:
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	bl 0x020086b8
	bl 0x020086c0
	bl 0x020086c8
	bl 0x02008650
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200873c
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #18
	movs	r1, #2
	movs	r2, #8
	bl 0x020086d8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000174
	ldr	r0, [pc, #12]
	b.n	.L_02000176
.L_02000174:
	ldr	r0, [pc, #12]
.L_02000176:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000038
	.4byte 0x020092ac
	.2byte 0x90d8
	.2byte 0x0200
	push	{r5, lr}
	bl 0x02008648
	movs	r0, #0
	bl 0x020086d0
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008698
	movs	r1, #0
	movs	r0, #9
	bl 0x020086a0
	bl 0x020086e8
	movs	r1, #0
	bl 0x02008658
	cmp	r0, #0
	bne.n	.L_020001c0
	movs	r0, #10
	bl 0x02008640
	adds	r0, r5, #1
	bl 0x02008698
	b.n	.L_020001cc
.L_020001c0:
	movs	r0, #20
	bl 0x02008640
	adds	r0, r5, #2
	bl 0x02008698
.L_020001cc:
	movs	r0, #9
	movs	r1, #0
	bl 0x020086a8
	bl 0x02008650
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x191f
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #5
	movs	r1, #49
	movs	r2, #5
	movs	r3, #41
	bl 0x02008618
	movs	r3, #4
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #48
	movs	r2, #3
	movs	r3, #2
	movs	r0, #4
	bl 0x02008630
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #91
	bl 0x02008608
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #2
	sub	sp, #8
	bl 0x02008600
	cmp	r0, #0
	bne.n	.L_020002ee
	movs	r0, #0
	bl 0x02008620
	movs	r0, #1
	bl 0x02008620
	movs	r0, #2
	bl 0x02008620
	movs	r5, #16
	movs	r0, #56
	movs	r1, #46
	movs	r2, #30
	movs	r3, #46
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008618
	movs	r0, #56
	movs	r1, #174
	movs	r2, #30
	movs	r3, #174
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008618
	movs	r2, #15
	movs	r3, #23
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r2
	movs	r0, #28
	movs	r1, #23
	movs	r2, #16
	movs	r3, #16
	bl 0x02008628
	movs	r3, #21
	movs	r2, #88
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	bl 0x02008628
	movs	r3, #89
	str	r3, [sp, #4]
	movs	r6, #17
	movs	r0, #28
	movs	r1, #90
	movs	r2, #4
	movs	r3, #4
	str	r6, [sp, #0]
	bl 0x02008628
	movs	r5, #91
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl 0x02008628
	movs	r3, #19
	str	r3, [sp, #0]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x02008628
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r5, #92
	movs	r0, #28
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x02008628
	movs	r0, #30
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008628
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008608
.L_020002ee:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #16
	sub	sp, #8
	bl 0x02008660
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	ldr	r5, [pc, #268]
	strb	r3, [r0, #0]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl 0x02008660
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r2, #240
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #236]
	cmp	r2, r3
	bne.n	.L_02000414
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008600
	cmp	r0, #0
	beq.n	.L_02000426
	movs	r0, #0
	bl 0x02008620
	movs	r0, #1
	bl 0x02008620
	movs	r0, #2
	bl 0x02008620
	movs	r5, #16
	movs	r0, #56
	movs	r1, #46
	movs	r2, #30
	movs	r3, #46
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008618
	movs	r0, #56
	movs	r1, #174
	movs	r2, #30
	movs	r3, #174
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008618
	movs	r2, #15
	movs	r3, #23
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r2
	movs	r0, #28
	movs	r1, #23
	movs	r2, #16
	movs	r3, #16
	bl 0x02008628
	movs	r3, #21
	movs	r2, #88
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	bl 0x02008628
	movs	r3, #89
	str	r3, [sp, #4]
	movs	r6, #17
	movs	r0, #28
	movs	r1, #90
	movs	r2, #4
	movs	r3, #4
	str	r6, [sp, #0]
	bl 0x02008628
	movs	r5, #91
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl 0x02008628
	movs	r3, #19
	str	r3, [sp, #0]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x02008628
	mov	r3, r8
	movs	r5, #92
	str	r3, [sp, #0]
	movs	r0, #28
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x02008628
	movs	r0, #30
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x02008628
	b.n	.L_02000426
.L_02000414:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #91
	bl 0x02008600
	cmp	r0, #0
	beq.n	.L_02000426
	bl 0x020081e0
.L_02000426:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0038
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #168]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #160]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000454
	movs	r6, #12
	movs	r5, #17
	b.n	.L_02000458
.L_02000454:
	movs	r6, #3
	movs	r5, #9
.L_02000458:
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x020086b0
	movs	r0, #10
	bl 0x02008640
	movs	r0, #4
	bl 0x02008660
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #1
	movs	r3, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #14
	movs	r0, #96
	movs	r1, #14
	movs	r2, #72
	bl 0x02008618
	movs	r2, #4
	movs	r1, #0
	movs	r0, #4
	bl 0x02008680
	movs	r0, #4
	bl 0x02008688
	movs	r0, #4
	bl 0x02008660
	movs	r1, #0
	bl 0x02008638
	movs	r0, #4
	movs	r1, #13
	bl 0x02008690
	adds	r2, r5, #0
	movs	r1, #0
	movs	r0, #4
	bl 0x02008680
	movs	r0, #4
	bl 0x02008688
	movs	r1, #10
	movs	r0, #4
	bl 0x02008690
	movs	r0, #10
	bl 0x02008640
	movs	r0, #123
	bl 0x020086f0
	adds	r0, r6, #0
	bl 0x020086b8
	bl 0x02008650
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0037
	.2byte 0x0000
	push	{lr}
	bl 0x02008648
	movs	r0, #0
	bl 0x020086d0
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x020086e0
	movs	r0, #4
	movs	r1, #16
	movs	r2, #0
	bl 0x020086e0
	bl 0x02008438
	pop	{pc}
	push	{lr}
	bl 0x02008648
	movs	r0, #0
	bl 0x020086d0
	movs	r1, #2
	movs	r2, #6
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x020086e0
	movs	r1, #14
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x020086e0
	bl 0x02008438
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl 0x02008648
	movs	r0, #0
	bl 0x020086d0
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020086b0
	bl 0x02008438
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #4
	bl 0x02008660
	movs	r3, #10
	ldrsh	r5, [r0, r3]
	bl 0x02008648
	movs	r0, #0
	bl 0x020086d0
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_02000582
	adds	r3, #15
.L_02000582:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r5, r3
	cmp	r3, #7
	bgt.n	.L_020005aa
	movs	r1, #4
	movs	r2, #4
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x020086e0
	movs	r1, #4
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl 0x020086e0
	b.n	.L_020005c2
.L_020005aa:
	movs	r2, #4
	movs	r0, #4
	movs	r1, #4
	negs	r2, r2
	bl 0x020086e0
	movs	r2, #8
	movs	r0, #4
	movs	r1, #4
	negs	r2, r2
	bl 0x020086e0
.L_020005c2:
	movs	r1, #212
	movs	r0, #4
	lsls	r1, r1, #1
	movs	r2, #99
	bl 0x02008670
	bl 0x02008438
	pop	{r5, pc}
	push	{lr}
	bl 0x02008648
	movs	r0, #0
	bl 0x020086d0
	movs	r2, #4
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x020086e0
	movs	r1, #212
	movs	r0, #4
	lsls	r1, r1, #1
	movs	r2, #99
	bl 0x02008670
	bl 0x02008438
	pop	{pc}
	.2byte 0x0000
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x08020171, 0x08020179, 0x080201a1, 0x080201e1, 0x080201e9, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c8119, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8581, 0x080c85f9, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00320051
	.4byte 0x00020001
	.4byte 0x00500006
	.4byte 0x00010032
	.4byte 0x00060002
	.4byte 0x004fffff
	.4byte 0x00010032
	.4byte 0x00060002
	.4byte 0x0032004e
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x0034004e
	.4byte 0x00020002
	.4byte 0x004e0006
	.4byte 0x00020035
	.4byte 0x00060002
	.4byte 0x0000ffff
	.4byte 0x020086f8
	.4byte 0x001d0054
	.4byte 0x020086f8
	.4byte 0x0022004d
	.4byte 0x0200870e
	.4byte 0x00260056
	.4byte 0x020086f8
	.4byte 0x0029004b
	.4byte 0x02008724
	.4byte 0x001b0045
	.4byte 0xffff0000
	.4byte 0x000001c8
	.4byte 0x40000248
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001c0044
	.4byte 0x004c0194
	.4byte 0x019c0024
	.4byte 0x0001ffff
	.4byte 0x001c00f4
	.4byte 0x00fc0064
	.4byte 0x006c0024
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000037
	.4byte 0x10101039
	.4byte 0xffffffff
	.4byte 0x10202039
	.4byte 0xffffffff
	.4byte 0x10303039
	.4byte 0xffffffff
	.4byte 0x10404039
	.4byte 0xffffffff
	.4byte 0x1050103a
	.4byte 0xffffffff
	.4byte 0x1060e002
	.4byte 0xffffffff
	.4byte 0x10709037
	.4byte 0xffffffff
	.4byte 0x1080a037
	.4byte 0xffffffff
	.4byte 0x10907037
	.4byte 0xffffffff
	.4byte 0x10a08037
	.4byte 0xffffffff
	.4byte 0x10b04038
	.4byte 0xffffffff
	.4byte 0x10c01038
	.4byte 0xffffffff
	.4byte 0x00000038
	.4byte 0x1010c037
	.4byte 0xffffffff
	.4byte 0x10203038
	.4byte 0xffffffff
	.4byte 0x10302038
	.4byte 0xffffffff
	.4byte 0x1040b037
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00014000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00034000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0000c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00015000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0001e000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0001a000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00012000
	.4byte 0xffff00cb
	.4byte 0x02008850
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0xffff0051
	.4byte 0x02008bf8
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00008000
	.4byte 0x006000f5
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200809d
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte 0x02008545
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x020084ed
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02008515
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000191e
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008189
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001922
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001923
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001924
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001925
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001926
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001927
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001938
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001939
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001928
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001929
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000192a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000192b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000192c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000192d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000192e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000192f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000193a
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000193b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200814d
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303e
	.4byte 0x50008805
	.4byte 0xffff0032
	.4byte 0x020081e1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte 0x02008545
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x020084ed
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02008515
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x02008565
	.4byte 0x0000c602
	.4byte 0xffff0022
	.4byte 0x020085d5
	.4byte 0x50008a05
	.4byte 0x0200003c
	.4byte 0x02008219
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
