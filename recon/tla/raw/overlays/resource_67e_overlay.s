.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020089d1, 0x02008199, 0x020081a5, 0x020081ad, 0x0200849d, 0x020081a1, 0x02008b05
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x02008de8
	pop	{pc}
	.2byte 0x0000
	.2byte 0x8e10
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #12
	sub	sp, #8
	bl 0x02008d18
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r5, r3, #20
	bl 0x02008df0
	movs	r0, #132
	bl 0x02008e08
	cmp	r6, #20
	bne.n	.L_0200014c
	cmp	r5, #9
	bne.n	.L_0200014c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_020000a0
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #36
	movs	r1, #12
	movs	r2, #22
	movs	r3, #8
	bl 0x02008ce0
	movs	r3, #22
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r1, #12
	movs	r2, #3
	movs	r3, #3
	bl 0x02008ce8
.L_020000a0:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_02000142
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	movs	r0, #22
	movs	r1, #1
	bl 0x02008d98
	bl 0x02008da0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #22
	bl 0x02008d88
	movs	r0, #60
	bl 0x02008cf8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #22
	bl 0x02008d78
	movs	r0, #60
	bl 0x02008cf8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x02008d88
	movs	r0, #60
	bl 0x02008cf8
	ldr	r0, [pc, #152]
	bl 0x02008d60
	movs	r1, #0
	movs	r0, #22
	bl 0x02008d70
	movs	r0, #30
	bl 0x02008cf8
	movs	r1, #131
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x02008d88
	movs	r0, #30
	bl 0x02008cf8
	movs	r0, #22
	movs	r1, #0
	bl 0x02008d70
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008d78
	bl 0x02008d08
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x02008cd0
.L_02000142:
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x02008cd0
	b.n	.L_02000190
.L_0200014c:
	cmp	r6, #19
	bne.n	.L_02000190
	cmp	r5, #9
	bne.n	.L_02000190
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x02008cd8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_02000190
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #36
	movs	r1, #8
	movs	r2, #22
	movs	r3, #8
	bl 0x02008ce0
	movs	r3, #22
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r1, #8
	movs	r2, #3
	movs	r3, #3
	bl 0x02008ce8
.L_02000190:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x22b0
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9414
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9444
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_020001c0
	ldr	r0, [pc, #4]
	b.n	.L_020001c2
.L_020001c0:
	ldr	r0, [pc, #4]
.L_020001c2:
	pop	{pc}
	.4byte 0x02009644
	.2byte 0x9494
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #23
	movs	r2, #30
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #53
	movs	r1, #5
	movs	r2, #8
	movs	r3, #9
	bl 0x02008df8
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x02008d18
	movs	r3, #23
	movs	r2, #30
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #5
	movs	r0, #53
	movs	r2, #8
	movs	r3, #9
	bl 0x02008df8
	movs	r0, #234
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_02000274
	movs	r1, #228
	movs	r2, #146
	movs	r0, #64
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02008d40
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #28
	bne.n	.L_02000274
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #36
	bne.n	.L_02000274
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02008d90
	movs	r1, #16
	ldr	r0, [r6, #0]
	negs	r1, r1
	movs	r2, #0
	bl 0x02008d30
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x02008d38
	bl 0x02008d08
.L_02000274:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x02008d18
	movs	r3, #23
	movs	r2, #30
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #5
	movs	r0, #53
	movs	r2, #8
	movs	r3, #9
	bl 0x02008df8
	movs	r0, #250
	lsls	r0, r0, #4
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_02000302
	movs	r1, #236
	movs	r2, #138
	movs	r0, #65
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02008d40
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #29
	bne.n	.L_02000302
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #34
	bne.n	.L_02000302
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02008d90
	movs	r2, #16
	ldr	r0, [r6, #0]
	movs	r1, #0
	negs	r2, r2
	bl 0x02008d30
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x02008d38
	bl 0x02008d08
.L_02000302:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #7
	movs	r2, #32
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #44
	movs	r1, #5
	movs	r2, #8
	movs	r3, #5
	bl 0x02008df8
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #16
	movs	r2, #23
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #55
	movs	r2, #4
	movs	r3, #2
	str	r1, [sp, #8]
	bl 0x02008df8
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #9
	movs	r2, #18
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #42
	movs	r2, #6
	movs	r3, #2
	str	r1, [sp, #8]
	bl 0x02008df8
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x02008d18
	movs	r3, #9
	movs	r2, #18
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r5, r0, #0
	movs	r2, #6
	movs	r0, #42
	movs	r3, #2
	str	r1, [sp, #8]
	bl 0x02008df8
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #161
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_020003f0
	movs	r1, #168
	movs	r2, #164
	movs	r0, #66
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x02008d40
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #10
	bne.n	.L_020003f0
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #20
	bne.n	.L_020003f0
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02008d90
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x02008d30
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x02008d38
	bl 0x02008d08
.L_020003f0:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #24
	movs	r2, #16
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #49
	movs	r2, #5
	movs	r3, #4
	str	r1, [sp, #8]
	bl 0x02008df8
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #16
	movs	r2, #9
	movs	r1, #0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r2, #5
	movs	r3, #3
	str	r1, [sp, #8]
	bl 0x02008df8
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x02008d20
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x02008d80
	ldr	r0, [r5, #0]
	bl 0x02008d18
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #123
	bl 0x02008e08
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl 0x02008db0
	bl 0x02008dc0
	bl 0x02008dc8
	bl 0x02008d08
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_020004b0
	ldr	r0, [pc, #4]
	b.n	.L_020004b2
.L_020004b0:
	ldr	r0, [pc, #4]
.L_020004b2:
	pop	{pc}
	.4byte 0x02009b00
	.2byte 0x98b4
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_020004e8
	ldr	r0, [pc, #84]
	bl 0x02008d60
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d70
	b.n	.L_02000528
.L_020004e8:
	ldr	r6, [pc, #72]
	adds	r0, r6, #0
	bl 0x02008d60
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02008d68
	bl 0x02008e00
	movs	r1, #0
	bl 0x02008d10
	cmp	r0, #0
	bne.n	.L_02000514
	movs	r0, #10
	bl 0x02008cf8
	adds	r0, r6, #1
	bl 0x02008d60
	b.n	.L_02000520
.L_02000514:
	movs	r0, #20
	bl 0x02008cf8
	adds	r0, r6, #2
	bl 0x02008d60
.L_02000520:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d70
.L_02000528:
	bl 0x02008d08
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x000022b2
	.2byte 0x22ad
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_02000564
	ldr	r0, [pc, #84]
	bl 0x02008d60
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d70
	b.n	.L_020005a4
.L_02000564:
	ldr	r6, [pc, #72]
	adds	r0, r6, #0
	bl 0x02008d60
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02008d68
	bl 0x02008e00
	movs	r1, #0
	bl 0x02008d10
	cmp	r0, #0
	bne.n	.L_02000590
	movs	r0, #10
	bl 0x02008cf8
	adds	r0, r6, #1
	bl 0x02008d60
	b.n	.L_0200059c
.L_02000590:
	movs	r0, #20
	bl 0x02008cf8
	adds	r0, r6, #2
	bl 0x02008d60
.L_0200059c:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008d70
.L_020005a4:
	bl 0x02008d08
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00002328
	.2byte 0x2323
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008d60
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02008d68
	bl 0x02008e00
	movs	r1, #0
	bl 0x02008d10
	cmp	r0, #0
	bne.n	.L_020005ee
	movs	r0, #10
	bl 0x02008cf8
	adds	r0, r5, #1
	bl 0x02008d60
	b.n	.L_020005fa
.L_020005ee:
	movs	r0, #20
	bl 0x02008cf8
	adds	r0, r5, #2
	bl 0x02008d60
.L_020005fa:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02008d70
	bl 0x02008d08
	pop	{r5, r6, pc}
	.2byte 0x2338
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #144]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200069c
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #124
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000628:
	movs	r4, #160
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #96
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000628
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #98
	strh	r0, [r3, #0]
	adds	r3, #58
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000652:
	movs	r4, #160
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #128
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000652
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #130
	strh	r0, [r3, #0]
	adds	r3, #58
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_0200067c:
	movs	r4, #160
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #160
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_0200067c
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #162
	strh	r0, [r3, #0]
.L_0200069c:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #41
	bl 0x02008cd0
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	ldr	r0, [pc, #336]
	bl 0x02008d60
	ldr	r5, [pc, #332]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #132
	lsls	r2, r2, #1
	movs	r1, #200
	ldr	r0, [r5, #0]
	bl 0x02008d28
	ldr	r0, [r5, #0]
	bl 0x02008d38
	movs	r1, #0
	movs	r0, #32
	bl 0x02008d70
	movs	r0, #15
	bl 0x02008cf8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	ldr	r0, [r5, #0]
	bl 0x02008d88
	movs	r1, #200
	movs	r2, #216
	movs	r0, #32
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02008d40
	movs	r2, #248
	movs	r1, #200
	movs	r0, #32
	bl 0x02008d28
	movs	r0, #32
	bl 0x02008d38
	movs	r0, #30
	bl 0x02008cf8
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r2, #0
	movs	r1, #32
	ldr	r0, [r5, #0]
	bl 0x02008d58
	movs	r0, #30
	bl 0x02008cf8
	movs	r1, #4
	movs	r0, #32
	bl 0x02008d48
	movs	r0, #30
	bl 0x02008cf8
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	ldr	r0, [r5, #0]
	bl 0x02008d88
	movs	r1, #131
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x02008d88
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r1, #3
	movs	r0, #32
	bl 0x02008d48
	movs	r0, #15
	bl 0x02008cf8
	movs	r1, #0
	movs	r0, #32
	bl 0x02008d68
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x02008d10
	cmp	r0, #1
	bne.n	.L_02000802
	movs	r0, #90
	bl 0x02008cf8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #32
	adds	r1, #102
	adds	r2, #51
	bl 0x02008d20
	movs	r2, #216
	movs	r1, #200
	movs	r0, #32
	bl 0x02008d28
	movs	r0, #32
	bl 0x02008d38
	movs	r1, #1
	movs	r0, #32
	bl 0x02008da8
	movs	r0, #60
	bl 0x02008cf8
	movs	r1, #128
	movs	r0, #32
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008d78
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #32
	bl 0x02008d88
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r0, #32
	movs	r1, #3
	bl 0x02008d80
	movs	r0, #32
	bl 0x02008d18
	adds	r0, #85
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008cd0
	b.n	.L_02000806
.L_02000802:
	bl 0x02008814
.L_02000806:
	bl 0x02008d08
	pop	{r5, pc}
	.4byte 0x000023b1
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	ldr	r0, [pc, #308]
	bl 0x02008d60
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x02008d88
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #32
	bl 0x02008d78
	movs	r0, #30
	bl 0x02008cf8
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	ldr	r5, [pc, #260]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #32
	bl 0x02008d58
	movs	r1, #3
	movs	r0, #32
	bl 0x02008d48
	movs	r0, #30
	bl 0x02008cf8
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r1, #1
	movs	r0, #32
	bl 0x02008d50
	movs	r0, #30
	bl 0x02008cf8
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r1, #4
	movs	r0, #32
	bl 0x02008d48
	movs	r0, #30
	bl 0x02008cf8
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x02008d48
	movs	r0, #15
	bl 0x02008cf8
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #32
	bl 0x02008d88
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r2, #0
	ldr	r1, [r5, #0]
	movs	r0, #32
	bl 0x02008d58
	movs	r0, #15
	bl 0x02008cf8
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
	movs	r1, #3
	movs	r0, #32
	bl 0x02008d48
	movs	r0, #15
	bl 0x02008cf8
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x02008d48
	movs	r0, #30
	bl 0x02008cf8
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	adds	r2, #153
	adds	r1, #51
	movs	r0, #32
	bl 0x02008d20
	movs	r0, #32
	bl 0x02008d18
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #3
	movs	r0, #32
	bl 0x02008d80
	movs	r1, #200
	movs	r2, #208
	movs	r0, #32
	bl 0x02008d28
	movs	r0, #32
	bl 0x02008d38
	movs	r1, #0
	movs	r2, #0
	movs	r0, #32
	bl 0x02008d40
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x02008cd0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008cd8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000023b8
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_020009b8
	bl 0x02008d00
	movs	r0, #0
	bl 0x02008dd0
	movs	r1, #128
	movs	r2, #60
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x02008d88
	ldr	r5, [pc, #72]
	adds	r0, r5, #0
	bl 0x02008d60
	movs	r1, #0
	movs	r0, #32
	bl 0x02008d68
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x02008d10
	cmp	r0, #0
	bne.n	.L_020009a4
	bl 0x02008814
	b.n	.L_020009b2
.L_020009a4:
	subs	r0, r5, #1
	bl 0x02008d60
	movs	r0, #32
	movs	r1, #0
	bl 0x02008d70
.L_020009b2:
	bl 0x02008d08
	b.n	.L_020009c0
.L_020009b8:
	ldr	r0, [pc, #16]
	movs	r1, #0
	bl 0x02008d70
.L_020009c0:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000023b7
	.4byte 0x02000240
	.2byte 0x23bf
	.2byte 0x0000
	push	{lr}
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008cd8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #200
	subs	r2, #172
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #260]
	bl 0x02008cc0
	ldr	r0, [pc, #260]
	bl 0x02008de0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_02000adc
	movs	r0, #15
	bl 0x02008d18
	movs	r3, #2
	adds	r0, #90
	strb	r3, [r0, #0]
	movs	r1, #5
	movs	r0, #25
	bl 0x02008d48
	movs	r0, #26
	movs	r1, #5
	bl 0x02008d48
	movs	r0, #27
	movs	r1, #5
	bl 0x02008d48
	movs	r0, #28
	movs	r1, #5
	bl 0x02008d48
	movs	r0, #29
	movs	r1, #6
	bl 0x02008d48
	movs	r0, #30
	movs	r1, #6
	bl 0x02008d48
	movs	r0, #31
	movs	r1, #5
	bl 0x02008d48
	ldr	r3, [pc, #176]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #8
	bne.n	.L_02000a90
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #40
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_02000a90
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #41
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_02000a90
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_02000a90
	bl 0x02008db8
	bl 0x020086a4
.L_02000a90:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #41
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_02000adc
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #42
	bl 0x02008cc8
	cmp	r0, #0
	bne.n	.L_02000adc
	movs	r1, #200
	movs	r2, #216
	lsls	r2, r2, #16
	movs	r0, #32
	lsls	r1, r1, #16
	bl 0x02008d40
	movs	r0, #32
	movs	r1, #3
	bl 0x02008d80
	movs	r0, #32
	bl 0x02008d18
	adds	r0, #85
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02008cd0
.L_02000adc:
	movs	r1, #3
	movs	r0, #24
	bl 0x02008d80
	movs	r0, #24
	bl 0x02008d18
	adds	r0, #85
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #0
	pop	{pc}
	.4byte 0x0200860d
	.4byte 0x02008e10
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	ldr	r0, [r7, #0]
	sub	sp, #8
	bl 0x02008d18
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	asrs	r5, r3, #20
	bl 0x02008cd0
	movs	r0, #148
	lsls	r0, r0, #2
	bl 0x02008cc8
	cmp	r0, #0
	beq.n	.L_02000b76
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #36
	movs	r1, #12
	movs	r2, #22
	movs	r3, #8
	bl 0x02008ce0
	movs	r3, #22
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r1, #12
	movs	r2, #3
	movs	r3, #3
	bl 0x02008ce8
	cmp	r6, #23
	bne.n	.L_02000b9c
	cmp	r5, #9
	bne.n	.L_02000b9c
	movs	r1, #188
	movs	r2, #136
	ldr	r0, [r7, #0]
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02008d40
	b.n	.L_02000b9c
.L_02000b76:
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #36
	movs	r1, #8
	movs	r2, #22
	movs	r3, #8
	bl 0x02008ce0
	movs	r3, #22
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #36
	movs	r1, #8
	movs	r2, #3
	movs	r3, #3
	bl 0x02008ce8
.L_02000b9c:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	sub	sp, #8
	bl 0x02008cd8
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #40
	movs	r1, #8
	movs	r2, #22
	movs	r3, #8
	bl 0x02008ce0
	movs	r3, #22
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #40
	movs	r1, #8
	movs	r2, #3
	movs	r3, #3
	bl 0x02008ce8
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #192
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02008d78
	movs	r0, #10
	bl 0x02008cf8
	movs	r0, #4
	bl 0x02008d18
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #4
	movs	r1, #0
	movs	r0, #4
	bl 0x02008d30
	movs	r0, #4
	bl 0x02008d38
	movs	r0, #4
	bl 0x02008d18
	movs	r1, #0
	bl 0x02008cf0
	movs	r0, #129
	bl 0x02008e08
	movs	r2, #16
	movs	r1, #0
	movs	r0, #4
	bl 0x02008d30
	movs	r0, #4
	bl 0x02008d38
	movs	r1, #13
	movs	r0, #4
	bl 0x02008d48
	movs	r0, #14
	bl 0x02008cf8
	adds	r0, r5, #0
	bl 0x02008db0
	pop	{r5, pc}
	push	{lr}
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x02008dd8
	movs	r1, #16
	movs	r2, #0
	movs	r0, #4
	bl 0x02008dd8
	movs	r0, #10
	bl 0x02008cf8
	bl 0x02008bdc
	pop	{pc}
	push	{lr}
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x02008dd8
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #4
	bl 0x02008dd8
	movs	r0, #10
	bl 0x02008cf8
	bl 0x02008bdc
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02008d78
	movs	r0, #10
	bl 0x02008cf8
	bl 0x02008bdc
	pop	{pc}
	.2byte 0x0000
	.irp EntryTarget, 0x080000d1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020179, 0x080201e9, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80d1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8141, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8241, 0x080c8249, 0x080c8279, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85f9, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8761, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0xffff000c
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
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
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
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
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
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
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000003c
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
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00004ccc
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ab
	.4byte 0x10126002
	.4byte 0xffffffff
	.4byte 0x102010aa
	.4byte 0xffffffff
	.4byte 0x103020aa
	.4byte 0xffffffff
	.4byte 0x104030aa
	.4byte 0xffffffff
	.4byte 0x105040aa
	.4byte 0xffffffff
	.4byte 0x106050aa
	.4byte 0xffffffff
	.4byte 0x107060aa
	.4byte 0xffffffff
	.4byte 0x108070aa
	.4byte 0xffffffff
	.4byte 0x10a010ac
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0075
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00018000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00004000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff007a
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0001c000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0xffff007e
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001e000
	.4byte 0xffff007f
	.4byte 0x00000003
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0075
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00018000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff007a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff007e
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001e000
	.4byte 0xffff007f
	.4byte 0x00000003
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x009b0000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte 0x02008e14
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte 0x02008f14
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte 0x02009014
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001e000
	.4byte 0xffff007c
	.4byte 0x02009114
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x02009214
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00014000
	.4byte 0xffff007d
	.4byte 0x02009314
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008439
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x02008ca5
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x02008c7d
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte 0x02008c59
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000022a5
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000022a6
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000022a7
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000022a8
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000022a9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000022aa
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000022ab
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000022ac
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020084bd
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000022ce
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x000022cf
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000022b3
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000022b4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000022b5
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000022b6
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000022b7
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000022b8
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000022b9
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000022ba
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000022bb
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000022d0
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000022d1
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008039
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008049
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte 0x02008039
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x02008049
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte 0x020081cd
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte 0x020081ed
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte 0x0200827d
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte 0x0200830d
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte 0x0200832d
	.4byte 0x50008905
	.4byte 0xffff001a
	.4byte 0x0200834d
	.4byte 0x50008905
	.4byte 0xffff001b
	.4byte 0x0200836d
	.4byte 0x50008905
	.4byte 0xffff001c
	.4byte 0x020083f9
	.4byte 0x50008905
	.4byte 0xffff001d
	.4byte 0x02008419
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008b09
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008ba5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008439
	.4byte 0x0000c602
	.4byte 0x03020008
	.4byte 0x02008439
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x02008ca5
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x02008c7d
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte 0x02008c59
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000231b
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000231c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000231d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000231e
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000231f
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002320
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002321
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002322
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008539
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002342
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002343
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002332
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002333
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002334
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00002335
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00002336
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00002337
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x020085b5
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte 0x02008955
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002329
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000232a
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000232b
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000232c
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000232d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000232e
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000232f
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002330
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002331
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002344
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002345
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000233b
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x0000233c
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x0000233d
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x0000233e
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x0000233f
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00002340
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x00002341
	.4byte 0x00008d15
	.4byte 0xffff0420
	.4byte 0x02008955
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte 0x02008039
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x02008049
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008039
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008049
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008b09
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008ba5
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte 0x020081cd
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte 0x020081ed
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte 0x0200827d
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte 0x0200830d
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte 0x0200832d
	.4byte 0x50008905
	.4byte 0xffff001a
	.4byte 0x0200834d
	.4byte 0x50008905
	.4byte 0xffff001b
	.4byte 0x0200836d
	.4byte 0x50008905
	.4byte 0xffff001c
	.4byte 0x020083f9
	.4byte 0x50008905
	.4byte 0xffff001d
	.4byte 0x02008419
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.2byte 0x0001
