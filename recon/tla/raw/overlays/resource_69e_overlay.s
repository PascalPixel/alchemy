.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008ae9, 0x02008039, 0x02008045, 0x0200804d, 0x02008ae1, 0x02008041, 0x02008bf1
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xafc0
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xaff0
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb01c
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x0200a9d0
	pop	{pc}
	.2byte 0x0000
	.2byte 0xafb8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	adds	r0, r7, #0
	bl 0x0200a8c8
	adds	r5, r0, #0
	cmp	r7, #11
	bne.n	.L_020000c2
	movs	r0, #160
	lsls	r0, r0, #4
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_020000c6
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #13
	bne.n	.L_020000c6
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #34
	bne.n	.L_020000c6
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r6, #15
.L_0200009a:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r5, #12]
	movs	r0, #1
	subs	r6, #1
	bl 0x0200a768
	cmp	r6, #0
	bge.n	.L_0200009a
	adds	r0, r7, #0
	bl 0x02008e04
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #9
	bl 0x0200a7f0
	b.n	.L_020000c6
.L_020000c2:
	bl 0x0200a9d8
.L_020000c6:
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r1, [r0, #80]
	adds	r0, #100
	ldrh	r3, [r0, #0]
	movs	r2, #3
	ands	r2, r3
	ldr	r4, [r1, #40]
	cmp	r2, #1
	beq.n	.L_020000fc
	cmp	r2, #1
	bgt.n	.L_020000e4
	cmp	r2, #0
	beq.n	.L_020000ee
	b.n	.L_02000118
.L_020000e4:
	cmp	r2, #2
	beq.n	.L_02000100
	cmp	r2, #3
	beq.n	.L_0200010e
	b.n	.L_02000118
.L_020000ee:
	movs	r3, #7
	strb	r3, [r4, #5]
	movs	r3, #1
	strb	r3, [r1, #25]
	movs	r3, #2
	strb	r3, [r1, #26]
	b.n	.L_02000118
.L_020000fc:
	movs	r3, #0
	b.n	.L_02000108
.L_02000100:
	movs	r2, #7
	movs	r3, #0
	strb	r2, [r4, #5]
	movs	r2, #1
.L_02000108:
	strb	r2, [r1, #25]
	strb	r3, [r1, #26]
	b.n	.L_02000118
.L_0200010e:
	movs	r2, #0
	movs	r3, #1
	strb	r2, [r4, #5]
	strb	r3, [r1, #25]
	strb	r2, [r1, #26]
.L_02000118:
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	pop	{pc}
	push	{r5, lr}
	ldr	r3, [pc, #32]
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_02000140
	movs	r1, #6
	lsrs	r0, r0, #1
	bl 0x0200a760
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl 0x0200a880
.L_02000140:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	movs	r0, #23
	bl 0x0200a8c8
	adds	r6, r5, #0
	adds	r6, #100
	ldrh	r1, [r6, #0]
	mov	sl, r0
	mov	r8, r1
	mov	r0, r8
	bl 0x0200a790
	ldr	r3, [r5, #48]
	mov	r1, sl
	adds	r3, #3
	adds	r2, r3, #0
	muls	r2, r0
	ldr	r3, [r1, #8]
	mov	r0, r8
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x0200a788
	mov	r2, sl
	ldr	r3, [r2, #16]
	ldr	r2, [r5, #8]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r5, #16]
	str	r2, [r5, #56]
	str	r3, [r5, #64]
	ldr	r1, [pc, #16]
	ldrh	r3, [r6, #0]
	adds	r3, r3, r1
	strh	r3, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7e8
	cmp	r0, #0
	bne.n	.L_020001c6
	ldr	r3, [pc, #44]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl 0x0200a760
	cmp	r0, #0
	bne.n	.L_0200028c
.L_020001c6:
	movs	r0, #23
	bl 0x0200a8c8
	adds	r5, r0, #0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_020001ec
	bl 0x0200a780
	adds	r2, r0, #0
	ldr	r3, [r5, #12]
	lsls	r2, r2, #8
	b.n	.L_020001f6
	.2byte 0x122c
	.2byte 0x0300
.L_020001ec:
	bl 0x0200a780
	adds	r2, r0, #0
	ldr	r3, [r5, #12]
	lsls	r2, r2, #6
.L_020001f6:
	lsrs	r2, r2, #16
	lsls	r2, r2, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #124]
	movs	r0, #168
	adds	r2, r2, r3
	ldr	r1, [r5, #8]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #2
	bl 0x0200a828
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0200028c
	ldr	r1, [pc, #108]
	adds	r0, r7, #0
	ldr	r6, [r7, #80]
	bl 0x0200a820
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200a920
	adds	r3, r7, #0
	adds	r3, #85
	movs	r5, #0
	strb	r5, [r3, #0]
	bl 0x0200a780
	ldr	r3, [pc, #80]
	adds	r2, r7, #0
	adds	r2, #100
	ands	r3, r0
	strh	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r5, [r3, #0]
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #52]
	str	r3, [r7, #108]
	mov	r8, r1
	bl 0x0200a780
	adds	r3, r0, #0
	lsls	r0, r3, #16
	subs	r0, r0, r3
	lsrs	r0, r0, #20
	bl 0x0200a788
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	str	r3, [r7, #48]
	mov	r3, r8
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #26]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	b.n	.L_0200028c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffe40000
	.4byte 0x0200b214
	.4byte 0x0ffff000
	.2byte 0x8149
	.2byte 0x0200
.L_0200028c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020002a4
	movs	r0, #201
	bl 0x0200aa40
.L_020002a4:
	ldr	r3, [pc, #12]
	ldr	r0, [r3, #0]
	bl 0x02009650
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200b278
	.2byte 0xb274
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #680]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r3, r1
	ldr	r0, [r5, #0]
	sub	sp, #52
	bl 0x0200a8c8
	adds	r7, r0, #0
	movs	r0, #13
	bl 0x0200a8c8
	str	r0, [sp, #24]
	movs	r0, #14
	bl 0x0200a8c8
	str	r0, [sp, #20]
	movs	r0, #15
	bl 0x0200a8c8
	str	r0, [sp, #16]
	movs	r0, #16
	bl 0x0200a8c8
	str	r0, [sp, #12]
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r3, #42
	beq.n	.L_02000302
	b.n	.L_0200069e
.L_02000302:
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	r3, #41
	beq.n	.L_0200030c
	b.n	.L_0200069e
.L_0200030c:
	movs	r0, #160
	lsls	r0, r0, #4
	bl 0x0200a7f0
	movs	r0, #0
	bl 0x0200a9a8
	movs	r1, #192
	lsls	r1, r1, #8
	ldr	r0, [r5, #0]
	bl 0x0200a940
	movs	r0, #30
	bl 0x0200a8a8
	ldr	r3, [pc, #584]
	movs	r0, #30
	str	r3, [r7, #108]
	bl 0x0200a8a8
	movs	r0, #220
	bl 0x0200aa40
	movs	r1, #64
	adds	r0, r7, #0
	bl 0x0200a818
	movs	r0, #50
	bl 0x0200a8a8
	movs	r0, #178
	bl 0x0200aa40
	adds	r2, r7, #0
	adds	r2, #100
	adds	r3, r2, #0
	movs	r5, #0
	str	r2, [sp, #8]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #540]
	movs	r0, #30
	str	r3, [r7, #108]
	bl 0x0200a8a8
	ldr	r3, [r7, #8]
	add	r0, sp, #40
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r1, #160
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	add	r1, sp, #28
	str	r3, [r0, #8]
	movs	r3, #170
	lsls	r3, r3, #18
	str	r3, [r1, #0]
	ldr	r3, [pc, #504]
	str	r5, [r1, #4]
	str	r3, [r1, #8]
	bl 0x02009718
	ldr	r3, [pc, #500]
	ldr	r2, [pc, #500]
	str	r5, [r3, #0]
	movs	r1, #144
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r0, [pc, #496]
	lsls	r1, r1, #3
	bl 0x0200a770
.L_0200039e:
	ldr	r2, [pc, #480]
	movs	r0, #20
	movs	r3, #0
	subs	r0, r0, r5
	str	r3, [r2, #0]
	mov	fp, r3
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	mov	r9, r2
	asrs	r0, r0, #1
	bl 0x0200a768
	movs	r1, #1
	mov	r2, r9
	str	r1, [r2, #0]
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a768
	cmp	r5, #19
	ble.n	.L_0200039e
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200aa40
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	bl 0x0200a870
	movs	r3, #1
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r0, #90
	bl 0x0200a8a8
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a870
	movs	r2, #3
	mov	r3, r9
	str	r2, [r3, #0]
	ldr	r3, [pc, #384]
	mov	r1, fp
	str	r1, [r3, #0]
	movs	r0, #40
	bl 0x0200a8a8
	movs	r1, #2
	movs	r0, #23
	bl 0x0200a948
	movs	r0, #23
	bl 0x0200a8c8
	movs	r1, #7
	bl 0x0200a920
	movs	r0, #23
	bl 0x0200a8c8
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #85
	mov	r1, fp
	strb	r1, [r3, #0]
	adds	r1, r2, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r5, #2
	orrs	r3, r5
	strb	r3, [r1, #0]
	movs	r6, #200
	ldr	r3, [pc, #328]
	ldr	r1, [pc, #332]
	lsls	r6, r6, #5
	adds	r6, #153
	str	r3, [r2, #28]
	str	r6, [r2, #24]
	movs	r0, #23
	mov	sl, r3
	mov	r8, r1
	bl 0x0200a8d8
	movs	r1, #2
	movs	r0, #24
	bl 0x0200a948
	movs	r0, #24
	bl 0x0200a8c8
	movs	r1, #7
	bl 0x0200a920
	movs	r0, #24
	bl 0x0200a8c8
	adds	r2, r0, #0
	adds	r3, r2, #0
	adds	r3, #85
	mov	r1, fp
	strb	r1, [r3, #0]
	adds	r1, r2, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r0, #24
	orrs	r5, r3
	mov	r3, sl
	strb	r5, [r1, #0]
	str	r3, [r2, #28]
	str	r6, [r2, #24]
	mov	r1, r8
	bl 0x0200a8d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #252]
	bl 0x0200a770
	movs	r0, #144
	bl 0x0200aa40
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	bl 0x0200a870
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200a980
	movs	r1, #0
	ldr	r0, [pc, #216]
	bl 0x0200a978
	movs	r0, #60
	bl 0x0200a988
	movs	r0, #60
	bl 0x0200a768
	movs	r0, #144
	bl 0x0200aa40
	movs	r0, #30
	bl 0x0200a768
	movs	r0, #144
	bl 0x0200aa40
	movs	r0, #30
	bl 0x0200a768
	movs	r0, #144
	bl 0x0200aa40
	movs	r0, #30
	bl 0x0200a768
	movs	r3, #41
	movs	r2, #99
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #58
	movs	r1, #99
	movs	r2, #3
	movs	r3, #3
	bl 0x0200a860
	movs	r3, #106
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #5
	movs	r1, #32
	movs	r2, #1
	movs	r0, #123
	bl 0x0200a860
	movs	r0, #2
	bl 0x02008d80
	movs	r0, #144
	bl 0x0200aa40
	movs	r0, #30
	bl 0x0200a768
	ldr	r1, [pc, #60]
	ldr	r2, [sp, #8]
	movs	r0, #1
	strh	r1, [r2, #0]
	bl 0x0200a768
	mov	r3, fp
	str	r3, [r7, #108]
	adds	r0, r7, #0
	movs	r1, #16
	bl 0x0200a818
	ldr	r3, [pc, #40]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	movs	r1, #1
	bl 0x0200a868
	ldr	r5, [pc, #64]
	movs	r0, #23
	adds	r1, r5, #0
	bl 0x0200a8d8
	adds	r1, r5, #0
	movs	r0, #24
	b.n	.L_020005a0
	.2byte 0x0000
	.4byte 0x00000003
	.4byte 0x02000240
	.4byte 0x02008121
	.4byte 0x020080c9
	.4byte 0x023e0000
	.4byte 0x0200b274
	.4byte 0x0200b278
	.4byte 0x02008295
	.4byte 0xffff0000
	.4byte 0x0200af70
	.4byte 0x020081a5
	.4byte 0x004063ff
	.2byte 0xaf94
	.2byte 0x0200
.L_020005a0:
	bl 0x0200a8d8
	mov	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r0, #30
	bl 0x0200a8a8
	mov	r1, r9
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r0, #30
	bl 0x0200a8a8
	mov	r3, r9
	mov	r2, fp
	str	r2, [r3, #0]
	movs	r0, #23
	bl 0x0200a8e0
	movs	r0, #24
	bl 0x0200a8e0
	movs	r0, #23
	bl 0x0200a8c8
	mov	r1, fp
	str	r1, [r0, #24]
	movs	r0, #24
	bl 0x0200a8c8
	mov	r2, fp
	str	r2, [r0, #24]
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200a978
	movs	r0, #30
	bl 0x0200a988
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200a870
	movs	r0, #30
	bl 0x0200a8a8
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	movs	r0, #0
	bl 0x0200a870
	ldr	r0, [pc, #48]
	bl 0x0200a778
	bl 0x0200985c
	movs	r0, #30
	bl 0x0200a8a8
	ldr	r3, [sp, #24]
	ldr	r6, [pc, #28]
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r3, [sp, #20]
	movs	r5, #15
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r3, [sp, #16]
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r3, [sp, #12]
	adds	r3, #85
	strb	r6, [r3, #0]
	b.n	.L_0200064c
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x8295
	.2byte 0x0200
.L_0200064c:
	ldr	r1, [sp, #24]
	movs	r2, #128
	ldr	r3, [r1, #12]
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r1, [sp, #20]
	movs	r0, #1
	ldr	r3, [r1, #12]
	subs	r5, #1
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #12]
	bl 0x0200a768
	cmp	r5, #0
	bge.n	.L_0200064c
	movs	r0, #13
	bl 0x02008e04
	movs	r0, #14
	bl 0x02008e04
	movs	r0, #15
	bl 0x02008e04
	movs	r0, #16
	bl 0x02008e04
	movs	r0, #80
	bl 0x0200aa40
	bl 0x0200aa38
.L_0200069e:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #14
	bl 0x0200a7e8
	cmp	r0, #0
	bne.n	.L_0200071e
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #14
	bl 0x0200a7f0
	bl 0x0200a8b0
	movs	r0, #0
	bl 0x0200a9a8
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200a980
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #3
	bl 0x0200a978
	movs	r0, #30
	bl 0x0200a988
	bl 0x0200a9a0
	movs	r1, #1
	movs	r0, #0
	bl 0x0200a908
	movs	r0, #30
	bl 0x0200a8a8
	movs	r2, #2
	negs	r2, r2
	ldr	r0, [pc, #28]
	movs	r1, #0
	bl 0x0200a898
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200a978
	movs	r0, #30
	bl 0x0200a988
	bl 0x0200a8b8
.L_0200071e:
	pop	{pc}
	.2byte 0x2b49
	.2byte 0x0000
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #14
	bl 0x0200a7f8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #22
	sub	sp, #56
	bl 0x0200a8c8
	mov	sl, r0
	movs	r0, #190
	bl 0x0200aa40
	movs	r0, #22
	bl 0x0200a8c8
	movs	r1, #0
	bl 0x0200a868
	movs	r3, #1
	add	r6, sp, #16
	str	r3, [r6, #0]
	movs	r3, #5
	str	r3, [r6, #4]
	movs	r3, #168
	lsls	r3, r3, #2
	strh	r3, [r6, #24]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r6, #8]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r6, #12]
	movs	r2, #0
	mov	r8, r2
.L_0200077a:
	movs	r0, #1
	bl 0x0200a8a8
	movs	r7, #1
	mov	r3, r8
	ands	r7, r3
	cmp	r7, #0
	bne.n	.L_020007d0
	bl 0x0200a780
	lsls	r3, r0, #1
	mov	r2, sl
	adds	r3, r3, r0
	ldr	r5, [r2, #8]
	lsls	r3, r3, #3
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	adds	r5, r5, r3
	ldr	r3, [pc, #104]
	adds	r5, r5, r3
	bl 0x0200a780
	mov	r2, sl
	ldr	r1, [r2, #12]
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	lsls	r0, r0, #16
	movs	r3, #128
	adds	r1, r1, r0
	lsls	r3, r3, #14
	adds	r1, r1, r3
	ldr	r3, [pc, #80]
	ldr	r2, [r2, #16]
	str	r3, [sp, #0]
	movs	r3, #216
	lsls	r3, r3, #13
	str	r3, [sp, #8]
	adds	r0, r5, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl 0x02009c88
.L_020007d0:
	mov	r2, r8
	cmp	r2, #20
	bne.n	.L_020007e0
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #1
	bl 0x0200a918
.L_020007e0:
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	cmp	r2, #31
	bls.n	.L_0200077a
	movs	r1, #0
	movs	r0, #22
	bl 0x0200a918
	movs	r0, #22
	bl 0x0200a8c8
	movs	r1, #1
	bl 0x0200a868
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfff40000
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb5e0
	adds	r7, r0, #0
	adds	r6, r1, #0
	bl 0x0200aa00
	adds	r0, r7, #0
	bl 0x0200a8c8
	movs	r1, #2
	bl 0x0200aa18
	movs	r0, #201
	bl 0x0200aa40
	movs	r0, #50
	bl 0x0200a768
	movs	r0, #82
	bl 0x0200aa40
	movs	r5, #0
.L_0200083a:
	adds	r0, r6, #0
	bl 0x0200a8c8
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	strh	r3, [r0, #6]
	adds	r0, r6, #0
	bl 0x0200a8c8
	movs	r3, #4
	ands	r3, r5
	movs	r1, #7
	cmp	r3, #0
	bne.n	.L_0200085c
	movs	r1, #0
.L_0200085c:
	bl 0x0200a920
	adds	r5, #1
	movs	r0, #1
	bl 0x0200a768
	cmp	r5, #119
	ble.n	.L_0200083a
	adds	r0, r6, #0
	bl 0x0200a8c8
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	adds	r0, r6, #0
	bl 0x0200a8c8
	movs	r1, #0
	bl 0x0200a920
	adds	r0, r7, #0
	bl 0x0200a8c8
	movs	r1, #0
	bl 0x0200aa18
	bl 0x0200aa10
	bl 0x0200aa08
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a908
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #36
	bl 0x0200a7e8
	cmp	r0, #0
	bne.n	.L_020008b6
	b.n	.L_02000ada
.L_020008b6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #35
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_020008c6
	b.n	.L_02000ada
.L_020008c6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #34
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_020008d6
	b.n	.L_02000ada
.L_020008d6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #34
	bl 0x0200a7f0
	bl 0x0200a8b0
	movs	r0, #0
	bl 0x0200a9a8
	ldr	r0, [pc, #496]
	bl 0x0200a928
	movs	r0, #22
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a938
	movs	r2, #140
	movs	r0, #4
	movs	r1, #68
	lsls	r2, r2, #1
	bl 0x0200a8f0
	movs	r2, #136
	lsls	r2, r2, #1
	movs	r0, #4
	movs	r1, #68
	bl 0x0200a8f0
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a940
	movs	r0, #20
	bl 0x0200a8a8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a940
	movs	r0, #20
	bl 0x0200a8a8
	movs	r1, #0
	movs	r0, #4
	bl 0x0200a940
	movs	r0, #20
	bl 0x0200a8a8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200a940
	movs	r0, #10
	bl 0x0200a8a8
	movs	r0, #22
	bl 0x0200a8c8
	movs	r1, #0
	bl 0x0200a868
	movs	r0, #22
	bl 0x0200a8c8
	movs	r1, #15
	bl 0x0200a920
	movs	r3, #128
	movs	r1, #128
	movs	r2, #225
	lsls	r3, r3, #7
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	movs	r0, #22
	bl 0x0200a900
	bl 0x02008734
	movs	r0, #40
	bl 0x0200a8a8
	movs	r0, #136
	movs	r1, #1
	movs	r2, #240
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200a968
	movs	r1, #0
	movs	r0, #22
	bl 0x0200a930
	movs	r0, #4
	movs	r1, #0
	bl 0x0200a8c0
	cmp	r0, #0
	bne.n	.L_020009d0
	movs	r0, #22
	movs	r1, #4
	bl 0x0200a910
	movs	r2, #5
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a938
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020009fa
.L_020009d0:
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a910
	movs	r0, #10
	bl 0x0200a8a8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #22
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a938
.L_020009fa:
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a910
	movs	r0, #10
	bl 0x0200a8a8
	movs	r0, #22
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a938
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #22
	bl 0x0200a950
	movs	r2, #5
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a938
	movs	r0, #22
	movs	r1, #4
	bl 0x0200a910
	movs	r0, #22
	movs	r1, #0
	movs	r2, #5
	bl 0x0200a938
	movs	r2, #5
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a938
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a910
	movs	r0, #30
	bl 0x0200a8a8
	movs	r0, #136
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r0, r0, #15
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200a968
	movs	r2, #16
	movs	r1, #0
	movs	r0, #22
	bl 0x0200a9b8
	movs	r0, #30
	bl 0x0200a8a8
	movs	r1, #4
	movs	r0, #22
	bl 0x02008810
	movs	r0, #1
	bl 0x0200a8a0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r0, [r3, #0]
	movs	r1, #5
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r3, #0]
	asrs	r0, r0, #16
	bl 0x0200a890
	movs	r0, #30
	bl 0x0200a8a8
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a910
	movs	r0, #30
	bl 0x0200a8a8
	movs	r1, #0
	movs	r2, #5
	movs	r0, #22
	bl 0x0200a938
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200a7f0
	movs	r0, #98
	adds	r0, #255
	bl 0x0200a7f0
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200a7f0
	bl 0x0200a8b8
.L_02000ada:
	pop	{pc}
	.2byte 0x2a7a
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb27c
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #160
	adds	r3, r3, r2
	lsls	r0, r0, #4
	adds	r2, #88
	str	r2, [r3, #0]
	adds	r0, #35
	sub	sp, #8
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02000b18
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #34
	bl 0x0200a7f8
.L_02000b18:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #34
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02000b38
	movs	r3, #128
	movs	r1, #128
	movs	r2, #225
	lsls	r3, r3, #7
	movs	r0, #22
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x0200a900
.L_02000b38:
	ldr	r0, [pc, #172]
	bl 0x0200a9c8
	movs	r0, #64
	bl 0x0200a8c8
	movs	r1, #9
	bl 0x0200a9c0
	movs	r0, #23
	bl 0x0200a8c8
	adds	r3, r0, #0
	movs	r5, #0
	adds	r3, #85
	str	r5, [r0, #24]
	strb	r5, [r3, #0]
	movs	r3, #170
	lsls	r3, r3, #18
	str	r3, [r0, #8]
	mov	r8, r3
	movs	r6, #145
	movs	r3, #128
	lsls	r3, r3, #14
	lsls	r6, r6, #18
	str	r3, [r0, #12]
	str	r6, [r0, #16]
	movs	r0, #23
	bl 0x0200a8c8
	movs	r1, #0
	bl 0x0200a868
	movs	r0, #24
	bl 0x0200a8c8
	adds	r3, r0, #0
	adds	r3, #85
	str	r5, [r0, #24]
	strb	r5, [r3, #0]
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #15
	str	r2, [r0, #8]
	str	r3, [r0, #12]
	str	r6, [r0, #16]
	movs	r0, #24
	bl 0x0200a8c8
	movs	r1, #0
	bl 0x0200a868
	movs	r0, #160
	lsls	r0, r0, #4
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02000bde
	movs	r3, #41
	movs	r2, #99
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #58
	movs	r1, #99
	movs	r2, #3
	movs	r3, #3
	bl 0x0200a860
	movs	r3, #106
	movs	r2, #32
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #123
	movs	r1, #32
	movs	r2, #1
	movs	r3, #5
	bl 0x0200a860
	movs	r1, #144
	ldr	r0, [pc, #20]
	lsls	r1, r1, #3
	bl 0x0200a770
.L_02000bde:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x0200afb8
	.2byte 0x81a5
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x02008c8c
	movs	r0, #0
	bl 0x02008d80
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02000c32
	movs	r0, #208
	movs	r1, #168
	lsls	r0, r0, #16
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #236
	bl 0x0200aa28
.L_02000c32:
	movs	r0, #160
	lsls	r0, r0, #4
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02000c62
	movs	r0, #10
	bl 0x02008e04
	movs	r0, #12
	bl 0x02008e04
	movs	r0, #13
	bl 0x02008e04
	movs	r0, #14
	bl 0x02008e04
	movs	r0, #15
	bl 0x02008e04
	movs	r0, #16
	bl 0x02008e04
.L_02000c62:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #9
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02000c84
	movs	r1, #216
	movs	r2, #138
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200a8f8
	movs	r0, #11
	bl 0x02008e04
.L_02000c84:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #20
	adds	r0, #255
	bl 0x0200a7f0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02000cb4
	movs	r0, #98
	adds	r0, #255
	bl 0x0200a7f0
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200a7f0
.L_02000cb4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xb3dc
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_02000cd6
	adds	r0, #3
.L_02000cd6:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200a758
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000d2a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r1, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_02000d0a
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_02000d66
.L_02000d0a:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_02000d66
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_02000d66
.L_02000d2a:
	movs	r5, #0
	movs	r6, #4
.L_02000d2e:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl 0x0200a758
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_02000d2e
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	ldr	r1, [pc, #36]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_02000d66:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b3d8
	.4byte 0x0200b3dc
	.4byte 0x0200b404
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	0x02008da4
	ldr	r1, [pc, #80]
	movs	r2, #32
	ldr	r0, [pc, #80]
	ldr	r5, [pc, #80]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4814
	ldr	r1, [pc, #80]
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	bl 0x0200a7e8
	cmp	r0, #0
	bne.n	.L_02000db4
	cmp	r6, #1
	bne.n	.L_02000dc6
.L_02000db4:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl 0x0200a770
	b.n	.L_02000dda
.L_02000dc6:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_02000dda:
	pop	{r5, r6, pc}
	.4byte 0x0200b3dc
	.4byte 0x05000180
	.4byte 0x0200b404
	.4byte 0x03000730
	.4byte 0x0200b424
	.4byte 0x050001a0
	.4byte 0x0200b3d8
	.4byte 0x02008cc5
	.4byte 0x0200b428
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, lr}
	bl 0x0200a8c8
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #85
	movs	r3, #4
	strb	r3, [r1, #0]
	movs	r2, #0
	ldr	r3, [r5, #20]
	str	r2, [r5, #68]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r5, #12]
	subs	r1, #50
	ldrb	r2, [r1, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #34
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r3, #0]
	bl 0x0200a850
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl 0x0200a888
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200aa30
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #244]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl 0x0200a8b0
	movs	r0, #0
	bl 0x0200a9a8
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	movs	r5, #0
	strh	r5, [r3, #0]
	movs	r3, #85
	adds	r3, r3, r6
	mov	r9, r3
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a868
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000ef8
.L_02000eb2:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
.L_02000eb8:
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_02000ece
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_02000ece:
	ldr	r3, [pc, #156]
	adds	r1, r6, #0
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	adds	r1, #35
	lsls	r3, r3, #12
	strh	r3, [r6, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200a768
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_02000eb2
.L_02000ef8:
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #68
	movs	r2, #1
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	strh	r2, [r3, #0]
	ldr	r3, [r7, #8]
	ldrh	r1, [r7, #6]
	subs	r2, #3
	asrs	r3, r3, #19
	ands	r3, r2
	asrs	r1, r1, #13
	adds	r3, r3, r1
	subs	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #56]
	asrs	r3, r3, #19
	ands	r3, r2
	movs	r2, #2
	ands	r1, r2
	subs	r3, r3, r1
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #16]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	adds	r3, r6, #0
	adds	r3, #35
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a868
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_02000f70
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_02000f70:
	bl 0x0200a968
	bl 0x0200a970
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	ldr	r5, [pc, #52]
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200a850
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl 0x0200a840
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_02000fd6
	b.n	.L_02000fbc
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02000fbc:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a768
	cmp	r5, #59
	bgt.n	.L_02000fd6
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_02000fbc
.L_02000fd6:
	movs	r0, #127
	bl 0x0200aa40
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02000ff6
.L_02000fe4:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a768
	cmp	r5, #59
	bgt.n	.L_02000ff6
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02000fe4
.L_02000ff6:
	adds	r0, r7, #0
	bl 0x0200a848
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200a8c8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200a960
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #70
	add	r2, sl
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #170
	lsls	r3, r3, #1
	movs	r6, #0
	add	r3, sl
	strh	r6, [r3, #0]
	bl 0x0200a8b8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [r1, #0]
	ldr	r4, [r0, #0]
	ldr	r2, [r1, #8]
	subs	r4, r4, r3
	ldr	r3, [r0, #8]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x0000
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #80]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200a8c8
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_020010ce
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
.L_020010b0:
	bne.n	.L_020010ce
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020010ce
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020010dc
.L_020010ce:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a818
	b.n	.L_0200121a
	.2byte 0x0240
	.2byte 0x0200
.L_020010dc:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200a818
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_020010fc
	movs	r0, #231
	bl 0x0200aa40
.L_020010fc:
	ldr	r3, [r7, #80]
	ldr	r0, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	movs	r2, #2
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	bl 0x0200aa20
	cmp	r0, #255
	beq.n	.L_020011fe
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl 0x0200a9b0
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_020011fe
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_020011fe
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020011c4
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
	bge.n	.L_0200115c
	subs	r5, r3, r2
.L_0200115c:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl 0x02009044
	cmp	r0, #12
	bgt.n	.L_0200117c
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_0200117c
	movs	r2, #1
	mov	r8, r2
.L_0200117c:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_020011c4
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200a7e8
	cmp	r0, #0
	bne.n	.L_020011c4
	ldrh	r3, [r6, #6]
	str	r6, [r7, #104]
	strh	r3, [r7, #6]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	strb	r3, [r1, #0]
	add	r2, sl
	movs	r3, #200
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r2, #128
	ldr	r0, [pc, #56]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_020011c4:
	ldrh	r0, [r6, #6]
	bl 0x0200a790
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200a788
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_020011f8
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_020011f8:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_0200121a
.L_020011fe:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
	bl 0x0200a820
	movs	r0, #228
	bl 0x0200aa40
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_0200121a:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b3e0
	.2byte 0xb400
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl 0x0200aa40
	ldrh	r0, [r5, #6]
	bl 0x0200a790
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r6, [pc, #152]
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68ab
	add	r2, sp, #56
	adds	r3, r3, r0
	str	r3, [r2, #0]
	mov	r8, r2
	ldrh	r0, [r5, #6]
	bl 0x0200a788
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x692b
	mov	r2, r8
	adds	r3, r3, r0
	str	r3, [r2, #8]
	movs	r0, #140
	ldr	r1, [r2, #0]
	lsls	r0, r0, #1
	ldr	r2, [r5, #12]
	bl 0x0200a828
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200a810
	adds	r0, r7, #0
.L_0200128a:
	movs	r1, #0
	bl 0x0200a868
	adds	r3, r7, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
.L_020012a0:
	ands	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldrh	r3, [r5, #6]
	add	r4, sp, #16
	strh	r3, [r7, #6]
	adds	r3, r7, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #1
	str	r3, [r4, #0]
	movs	r3, #7
	str	r3, [r4, #4]
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldr	r2, [r3, #8]
	ldr	r3, [pc, #24]
	ldr	r1, [r5, #12]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	b.n	.L_020012f0
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x02009071
	.2byte 0x0000
	.2byte 0xfffa
.L_020012f0:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl 0x02009c88
	adds	r0, r7, #0
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	sub	sp, #56
	ldr	r7, [r3, #0]
	movs	r3, #7
	ands	r7, r3
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_02001396
	add	r6, sp, #16
	movs	r3, #3
	str	r3, [r6, #0]
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	movs	r3, #14
	str	r3, [r6, #4]
	bl 0x0200a780
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl 0x0200a780
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	lsrs	r3, r3, #16
	movs	r2, #32
	subs	r2, r2, r3
	mov	r3, sl
	ldr	r5, [r3, #12]
	lsls	r2, r2, #16
	adds	r5, r5, r2
	bl 0x0200a780
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl 0x0200a750
	mov	r3, sl
	ldr	r2, [r3, #16]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r0, [sp, #0]
	str	r3, [sp, #8]
	mov	r0, r8
	adds	r1, r5, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl 0x02009c88
.L_02001396:
	movs	r0, #0
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl 0x0200a8c8
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200a8c8
	adds	r7, r0, #0
	movs	r0, #23
	bl 0x0200a8c8
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200a8f8
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200a8f8
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200a8f8
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200a8f8
	movs	r0, #24
	bl 0x0200a8c8
	movs	r3, #85
	movs	r2, #0
	adds	r3, r3, r5
	str	r2, [r0, #24]
	strb	r2, [r3, #0]
	mov	fp, r3
	ldr	r3, [r5, #20]
	movs	r0, #160
	str	r3, [r5, #12]
	movs	r3, #85
	adds	r3, r3, r6
	strb	r2, [r3, #0]
	mov	r9, r3
	ldr	r3, [r6, #20]
	lsls	r0, r0, #4
	str	r3, [r6, #12]
	movs	r3, #85
	adds	r3, r3, r7
	strb	r2, [r3, #0]
	mov	sl, r3
	ldr	r3, [r7, #20]
	adds	r0, #10
	str	r3, [r7, #12]
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02001484
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #208]
	movs	r0, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r2, [pc, #204]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #200]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200a8c8
	movs	r1, #4
	bl 0x0200a880
	movs	r0, #11
	bl 0x0200a8c8
	movs	r1, #4
	bl 0x0200a880
.L_02001484:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
.L_0200148a:
	bl 0x0200a7e8
	cmp	r0, #0
.L_02001490:
	beq.n	.L_020014ce
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #132]
.L_02001496:
	movs	r0, #10
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	ldr	r2, [pc, #120]
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200a8c8
	movs	r1, #4
	bl 0x0200a880
	movs	r0, #12
	bl 0x0200a8c8
	movs	r1, #4
	bl 0x0200a880
.L_020014ce:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_0200150e
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_0200150e
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #56]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r2, sl
	strb	r3, [r2, #0]
	mov	r2, fp
	strb	r3, [r2, #0]
.L_0200150e:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00066640
	.4byte 0x0001eb80
	.4byte 0xfffd70c0
	.4byte 0x00028f40
	.4byte 0xfffff800
	.4byte 0x00199900
	.2byte 0x8480
	.2byte 0x001b
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #16
	mov	sl, r3
	movs	r2, #232
	movs	r3, #95
	movs	r7, #128
	lsls	r2, r2, #4
	str	r3, [sp, #0]
	lsls	r7, r7, #3
	add	r2, sl
	add	r7, sl
	mov	fp, r2
.L_02001564:
	ldr	r2, [r7, #20]
	ldr	r0, [r7, #24]
	mov	r9, r2
	cmp	r0, #0
	blt.n	.L_02001632
	cmp	r0, r9
	bgt.n	.L_02001632
	mov	r1, r9
	lsls	r0, r0, #15
	bl 0x0200a750
	bl 0x0200a788
	movs	r3, #232
	movs	r2, #236
	lsls	r3, r3, #5
	lsls	r2, r2, #5
	adds	r3, #140
	add	r2, sl
	add	r3, sl
	ldr	r5, [r2, #0]
	ldr	r3, [r3, #0]
	ldr	r2, [r7, #24]
	subs	r3, r3, r5
	mov	r8, r0
	mov	r1, r9
	adds	r0, r2, #0
	muls	r0, r3
	add	r6, sp, #4
	bl 0x0200a750
	ldr	r3, [r7, #12]
	adds	r5, r5, r0
	mov	r2, r8
	muls	r2, r3
	adds	r3, r2, #0
	adds	r5, r5, r3
	str	r5, [r6, #0]
	ldr	r0, [r7, #24]
	mov	r1, r9
	lsls	r0, r0, #15
	bl 0x0200a750
	bl 0x0200a788
	movs	r2, #232
	movs	r3, #232
	lsls	r2, r2, #5
	lsls	r3, r3, #5
	adds	r2, #132
	adds	r3, #144
	add	r2, sl
	add	r3, sl
	ldr	r5, [r2, #0]
	ldr	r3, [r3, #0]
	ldr	r2, [r7, #24]
	subs	r3, r3, r5
	mov	r8, r0
	mov	r1, r9
	adds	r0, r2, #0
	muls	r0, r3
	bl 0x0200a750
	ldr	r3, [r7, #16]
	adds	r5, r5, r0
	mov	r2, r8
	muls	r2, r3
	adds	r3, r2, #0
	adds	r5, r5, r3
	movs	r2, #232
	movs	r3, #232
	str	r5, [r6, #4]
	lsls	r2, r2, #5
	lsls	r3, r3, #5
	adds	r2, #136
	adds	r3, #148
	add	r2, sl
	add	r3, sl
	ldr	r5, [r2, #0]
	ldr	r3, [r3, #0]
	ldr	r2, [r7, #24]
	subs	r3, r3, r5
	adds	r0, r2, #0
	muls	r0, r3
	mov	r1, r9
	bl 0x0200a750
	adds	r5, r5, r0
	adds	r0, r6, #0
	str	r5, [r6, #8]
	bl 0x0200a9b0
	ldr	r3, [r6, #0]
	mov	r2, fp
	str	r3, [r2, #12]
	mov	r0, fp
	ldr	r3, [r6, #8]
	str	r3, [r2, #16]
	bl 0x0200a9e8
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
.L_02001632:
	ldr	r2, [sp, #0]
	movs	r3, #40
	subs	r2, #1
	add	fp, r3
	str	r2, [sp, #0]
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_02001564
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	str	r3, [sp, #0]
	cmp	r0, #0
	ble.n	.L_02001704
	adds	r7, r0, #0
.L_02001670:
	ldr	r1, [sp, #0]
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #152
	adds	r1, r1, r2
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	r9, r1
	lsls	r6, r3, #3
	subs	r6, r6, r3
	ldr	r3, [sp, #0]
	lsls	r6, r6, #2
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r6, r3, r6
	adds	r6, r6, r1
	bl 0x0200a780
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r2, #192
	movs	r3, #128
	lsls	r3, r3, #15
	lsls	r2, r2, #7
	lsrs	r5, r5, #2
	mov	fp, r3
	adds	r5, r5, r2
	bl 0x0200a780
	mov	r8, r0
	mov	r1, r8
	lsls	r1, r1, #4
	lsrs	r1, r1, #16
	movs	r2, #16
	mov	r8, r1
	adds	r0, r5, #0
	add	r8, r2
	bl 0x0200a790
	ldr	r3, [pc, #84]
	mov	r1, fp
	mov	sl, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1400
	str	r0, [r6, #12]
	adds	r0, r5, #0
	bl 0x0200a788
	mov	r1, fp
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4240
	asrs	r0, r0, #16
	mov	r1, r8
	movs	r3, #0
	str	r1, [r6, #20]
	str	r3, [r6, #24]
	str	r0, [r6, #16]
	mov	r2, r9
	ldrh	r0, [r2, #0]
	mov	r3, r9
	adds	r0, #1
	strh	r0, [r3, #0]
	lsls	r0, r0, #16
	movs	r1, #96
	asrs	r0, r0, #16
	bl 0x0200a758
	subs	r7, #1
	mov	r1, r9
	strh	r0, [r1, #0]
	cmp	r7, #0
	bne.n	.L_02001670
.L_02001704:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	movs	r1, #237
	adds	r5, r0, #0
	lsls	r1, r1, #5
	movs	r0, #220
	sub	sp, #4
	bl 0x0200a798
	ldr	r3, [pc, #280]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	movs	r3, #236
	lsls	r3, r3, #5
	adds	r2, r7, r3
	ldr	r3, [r5, #0]
	movs	r1, #232
	str	r3, [r2, #0]
	lsls	r1, r1, #5
	ldr	r3, [r5, #4]
	adds	r1, #132
	adds	r2, r7, r1
	str	r3, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #136
	adds	r2, r7, r3
	ldr	r3, [r5, #8]
	adds	r1, #8
	str	r3, [r2, #0]
	adds	r2, r7, r1
	ldr	r3, [r6, #0]
	adds	r1, #8
	str	r3, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #144
	adds	r2, r7, r3
	ldr	r3, [r6, #4]
	mov	fp, r0
	str	r3, [r2, #0]
	adds	r2, r7, r1
	ldr	r3, [r6, #8]
	ldr	r0, [pc, #204]
	str	r3, [r2, #0]
	bl 0x0200a7e0
	adds	r1, r7, #0
	bl 0x0200a7b8
	bl 0x0200a7d0
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r2, r7, #0
	adds	r5, r0, #0
	bl 0x0200a7c8
	movs	r2, #232
	lsls	r2, r2, #5
	adds	r2, #154
	mov	r9, r0
	adds	r3, r7, r2
	mov	r1, r9
	strh	r1, [r3, #0]
	adds	r2, #2
	movs	r1, #128
	adds	r3, r7, r2
	lsls	r1, r1, #3
	movs	r2, #232
	strh	r5, [r3, #0]
	adds	r1, r1, r7
	movs	r3, #0
	lsls	r2, r2, #4
	mov	sl, r3
	mov	r8, r1
	adds	r6, r7, r2
.L_020017c8:
	movs	r5, #15
	mov	r3, sl
	ands	r3, r5
	lsls	r3, r3, #1
	add	r3, r9
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	bl 0x0200a9e0
	ldrb	r3, [r6, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r6, #5]
	ldrb	r3, [r6, #9]
	mov	r0, fp
	ands	r5, r3
	strb	r5, [r6, #9]
	bl 0x0200a9f0
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r6, #9]
	mov	r0, fp
	bl 0x0200a9f8
	movs	r3, #1
	mov	r2, r8
	negs	r3, r3
	subs	r0, #1
	strh	r0, [r6, #30]
	str	r3, [r2, #24]
	movs	r3, #1
	add	sl, r3
	movs	r1, #28
	mov	r2, sl
	adds	r6, #40
	add	r8, r1
	cmp	r2, #95
	ble.n	.L_020017c8
	movs	r1, #232
	lsls	r1, r1, #5
	adds	r1, #152
	adds	r2, r7, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	bl 0x0200a770
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000001ef
	.2byte 0x9539
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl 0x0200a778
	movs	r3, #232
	lsls	r3, r3, #5
	adds	r3, #156
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200a7c0
	movs	r0, #220
	bl 0x0200a7a0
.L_02001882:
	pop	{r5, pc}
	.2byte 0x9539
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #380]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [pc, #372]
	mov	sl, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	mov	r8, r2
	ldr	r2, [pc, #364]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	sub	sp, #8
	lsrs	r3, r3, #5
	str	r3, [sp, #4]
	ldr	r6, [pc, #356]
	ldr	r3, [r1, #0]
	movs	r1, #0
	ldr	r3, [r3, #4]
	mov	r9, r1
	str	r3, [sp, #0]
	ldr	r3, [pc, #348]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r9, r3
	blt.n	.L_020018da
	b.n	.L_02001a0e
.L_020018da:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_020018e8
	b.n	.L_020019fe
.L_020018e8:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_020018f0
	b.n	.L_020019fe
.L_020018f0:
	mov	r1, sl
	subs	r0, r3, r1
	ldr	r2, [sp, #0]
	ldr	r3, [r5, #12]
	movs	r1, #128
.L_020018fa:
	subs	r3, r3, r2
	ldr	r2, [r5, #16]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	mov	r1, r8
	subs	r2, r2, r1
	ldr	r1, [sp, #0]
	subs	r2, r2, r1
	subs	r4, r2, r3
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r3, #58
	mov	fp, r3
	ldr	r3, [pc, #284]
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r3, r5, #0
	mov	ip, r2
	asrs	r1, r0, #16
	mov	r0, ip
	adds	r3, #100
	asrs	r2, r4, #16
	cmp	r0, #0
	bne.n	.L_02001966
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #7
	movs	r1, #167
	adds	r4, r2, #0
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #16
	cmp	r3, r1
	bhi.n	.L_020019fe
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020019fe
	cmp	r4, #239
	bgt.n	.L_020019fe
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	mov	r3, ip
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #212]
	b.n	.L_020019a2
.L_02001966:
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #0
	movs	r1, #175
	adds	r4, r2, #0
	adds	r3, #23
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #64
	cmp	r3, r1
.L_0200197c:
	bhi.n	.L_020019fe
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_020019fe
	cmp	r4, #175
	bgt.n	.L_020019fe
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #152]
.L_020019a2:
	movs	r2, #128
	orrs	r4, r3
	stmia	r1!, {r4}
	ldr	r0, [sp, #4]
	lsls	r3, r7, #3
	adds	r3, r0, r3
	lsls	r2, r2, #4
	orrs	r3, r2
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	movs	r0, #1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_020019e0
	adds	r0, r5, #0
	bl 0x0200a9f0
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #2
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r6, #9]
	b.n	.L_020019f4
.L_020019e0:
	movs	r3, #3
	ands	r3, r2
	movs	r0, #13
	ldrb	r2, [r6, #9]
	negs	r0, r0
	adds	r1, r0, #0
	lsls	r3, r3, #2
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_020019f4:
	adds	r0, r6, #0
	mov	r1, fp
	bl 0x0200a7d8
	adds	r6, #12
.L_020019fe:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_02001a0e
	b.n	.L_020018da
.L_02001a0e:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200b444
	.4byte 0x020036e0
	.4byte 0x0200b488
	.4byte 0x0200b446
	.2byte 0xb448
	.2byte 0x0200
	push	{r3, r6, lr}
	lsls	r0, r0, #8
	movs	r0, #0
	ands	r0, r0
	add	r0, pc, #0
	.2byte 0xc000
	push	{r1, r3, r6, lr}
	lsls	r0, r0, #8
	push	{r5, r6, lr}
.L_02001a46:
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200a7a8
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
.L_02001a52:
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
.L_02001a58:
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
.L_02001a5e:
	bl 0x0200a7b8
	ldr	r5, [pc, #76]
.L_02001a64:
	bl 0x0200a7d0
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a7c8
	adds	r0, r6, #0
	bl 0x0200a7b0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200a770
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b448
	.4byte 0x0200aa48
	.4byte 0x0200b444
	.4byte 0x02009889
	.2byte 0xb446
	.2byte 0x0200
	push	{r3, r6, lr}
	lsls	r0, r0, #8
	push	{r1, r3, r6, lr}
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200a7a8
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200a7b8
	ldr	r5, [pc, #76]
	bl 0x0200a7d0
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a7c8
	adds	r0, r6, #0
	bl 0x0200a7b0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200a770
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b448
	.4byte 0x0200abab
	.4byte 0x0200b444
	.4byte 0x02009889
	.2byte 0xb446
	.2byte 0x0200
	push	{r3, r6, lr}
	lsls	r0, r0, #8
	push	{r1, r3, r6, lr}
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200a7a8
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl 0x0200a7b8
	ldr	r5, [pc, #80]
	bl 0x0200a7d0
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a7c8
	adds	r0, r6, #0
	bl 0x0200a7b0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200a770
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_02001bc8
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b448
	.4byte 0x0200adda
	.4byte 0x0200b444
	.4byte 0x02009889
	.2byte 0xb446
	.2byte 0x0200
	push	{r3, r6, lr}
	lsls	r0, r0, #8
	push	{r1, r3, r6, lr}
	lsls	r0, r0, #8
.L_02001bc8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200a8c8
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_02001bf2
	adds	r3, r4, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #16]
	ldr	r0, [pc, #20]
	ldrh	r2, [r1, #0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	adds	r2, #1
	lsls	r3, r3, #2
	str	r4, [r0, r3]
	strh	r2, [r1, #0]
.L_02001bf2:
	pop	{r5, pc}
	.4byte 0x0200b446
	.4byte 0x0200b448
	.4byte 0x80184b01
	.2byte 0x4770
	.2byte 0x0000
	push	{r1, r3, r6, lr}
	lsls	r0, r0, #8
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02001c4c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001c4c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_02001c4c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200a8c8
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02001cd0
	cmp	r7, #0
	beq.n	.L_02001cd0
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02001cd8
.L_02001cd0:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02001cd8:
	mov	r3, sl
	bl 0x0200a828
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02001ce6
	b.n	.L_02001e32
.L_02001ce6:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200a810
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200a820
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a868
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02009c08
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001e32
	cmp	r7, #0
	beq.n	.L_02001e32
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001d68
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200a920
.L_02001d68:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001d88
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02009c08
.L_02001d88:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02001d9c
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02001d9c:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001de2
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02001dca
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200a750
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02001ddc
.L_02001dca:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200a750
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02001ddc:
	bl 0x0200a750
	str	r0, [r6, #52]
.L_02001de2:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001dfe
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a810
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200a820
.L_02001dfe:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001e10
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02001e10:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001e22
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02001e22:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001e32
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02001e32:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200b3f4
	.4byte 0x02009c51
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_02001f5c
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, #32
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
.L_02001e78:
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl 0x0200a8c8
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_02001e9c
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02001ea4
.L_02001e9c:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_02001ea4:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02001f5c
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02001f5c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	str	r4, [sp, #0]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r5, [r3, #4]
	ldr	r3, [r2, #0]
	ands	r0, r1
	ands	r5, r1
	ldr	r6, [r3, #4]
	movs	r1, #16
	ldrsh	r3, [r4, r1]
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	lsls	r1, r1, #20
	subs	r7, r1, r0
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #20
	bl 0x0200a850
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	subs	r0, r0, r6
	lsls	r3, r3, #20
	subs	r3, r3, r5
	subs	r3, r3, r6
	subs	r2, r3, r0
	asrs	r7, r7, #16
	adds	r0, r0, r3
	asrs	r0, r0, #16
	adds	r3, r7, #0
	movs	r5, #167
	asrs	r2, r2, #16
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r5, r5, #1
	adds	r2, #14
	adds	r1, #58
	ldr	r4, [sp, #0]
	cmp	r3, r5
	bhi.n	.L_02001f5c
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02001f5c
	cmp	r2, #239
	bgt.n	.L_02001f5c
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r4, #20]
	lsls	r3, r7, #16
	orrs	r2, r3
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	orrs	r2, r3
	movs	r3, #128
	str	r2, [r4, #24]
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r4, #28]
	adds	r0, #20
	bl 0x0200a7d8
.L_02001f5c:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r2, r3, r6, lr}
	lsls	r0, r0, #8
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x36e0
	lsls	r0, r0, #8
	ldrh	r0, [r0, #0]
	strh	r0, [r0, #0]
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #44]
	ldr	r0, [pc, #540]
	str	r1, [sp, #40]
	mov	r8, r0
	movs	r1, #32
	add	r1, r8
	mov	r9, r1
	mov	ip, r9
	adds	r5, r2, #0
	mov	r2, ip
	adds	r6, r3, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #520]
	movs	r1, #4
	ldr	r7, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xa80b
	ldrh	r0, [r0, #0]
	mov	r1, r8
	strh	r0, [r1, #4]
	add	r1, sp, #40
	ldrh	r1, [r1, #0]
	mov	r3, r8
	strh	r1, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r5, r8
	mov	r0, r8
	adds	r3, #255
	mov	r1, r8
	strh	r6, [r5, #6]
	movs	r2, #0
	strh	r7, [r0, #8]
	strh	r3, [r1, #12]
	mov	r3, r8
	strh	r2, [r3, #10]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	mov	ip, r3
	lsls	r2, r2, #1
	mov	r1, ip
	add	r2, ip
	adds	r1, #236
	ldr	r0, [r1, #0]
	ldr	r3, [r2, #8]
	ldr	r5, [r2, #48]
	adds	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	adds	r1, #4
	ldr	r3, [r2, #12]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	adds	r2, r2, r0
	lsls	r2, r2, #2
	asrs	r3, r3, #20
	adds	r5, r5, r2
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_02002100
.L_02002030:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_020020f4
.L_02002044:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_020020e4
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_020020e4
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_020020e4
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	mov	r0, r9
	adds	r7, r2, r3
	strh	r7, [r0, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #28]
	add	r0, sp, #40
	ldrh	r0, [r0, #0]
	adds	r6, r1, r2
	mov	r3, r9
	mov	r1, r9
	strh	r6, [r3, #2]
	strh	r0, [r1, #4]
	ldr	r1, [sp, #40]
	movs	r0, #10
	adds	r1, #1
	adds	r0, #255
	str	r1, [sp, #40]
	bl 0x0200a7e8
	cmp	r0, #0
	bne.n	.L_02002098
	cmp	r5, sl
	bne.n	.L_020020d6
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200a7f0
	b.n	.L_020020d6
.L_02002098:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_020020d6
	mov	r2, r8
	ldrh	r4, [r2, #6]
	ldrh	r5, [r2, #8]
	movs	r3, #8
	ldrsh	r1, [r2, r3]
	movs	r3, #6
	ldrsh	r0, [r2, r3]
	movs	r2, #64
	adds	r3, r2, #0
	ands	r3, r4
	ands	r2, r5
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	asrs	r3, r3, #16
	asrs	r2, r2, #16
	orrs	r7, r3
	orrs	r6, r2
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200a858
.L_020020d6:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_020020e4:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02002044
.L_020020f4:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02002030
.L_02002100:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a7e8
	cmp	r0, #0
	beq.n	.L_02002158
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	ldr	r3, [r0, #8]
	movs	r2, #0
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	mov	r0, r8
	asrs	r1, r3, #20
	ldr	r3, [sp, #8]
	mov	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	cmp	r2, r3
	bge.n	.L_02002158
.L_02002132:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02002148
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02002148
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02002148:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02002132
.L_02002158:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200a7a8
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02002166:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02002166
	bl 0x0200a7d0
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200a7c8
	adds	r0, r5, #0
	bl 0x0200a7b0
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200a770
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r2, r3, r6, lr}
.L_020021ae:
	lsls	r0, r0, #8
	lsls	r0, r3, #9
	lsls	r0, r0, #12
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	asrs	r1, r2, #4
	asrs	r1, r2, #4
	ldr	r6, [sp, #324]
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200a8d0
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200a8e8
	movs	r0, #1
	bl 0x0200a768
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	adds	r5, r5, r3
	mov	r1, sl
	adds	r6, r6, r3
	ldr	r2, [r1, #12]
	adds	r3, r6, #0
	adds	r1, r5, #0
	mov	r0, sl
	bl 0x0200a830
	movs	r0, #4
	bl 0x0200a8a8
	bl 0x0200a970
	ldr	r2, [pc, #20]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #100]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200a8c8
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	asrs	r6, r6, #20
	orrs	r6, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r0, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200a1c0
	movs	r0, #161
	bl 0x0200aa40
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a858
	movs	r0, #12
	bl 0x0200a8a8
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r8, r0
	ldr	r0, [r1, #0]
	sub	sp, #8
	mov	sl, r1
	bl 0x0200a8c8
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
	asrs	r7, r3, #20
	mov	r3, r8
	ldrh	r1, [r3, #6]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	orrs	r7, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r6, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200a1c0
	movs	r0, #229
	bl 0x0200aa40
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200a858
	movs	r0, #12
	bl 0x0200a8a8
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	movs	r1, #0
	bl 0x0200a868
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #16]
	movs	r7, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r2, sl
	b.n	.L_0200236c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_0200236c:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200a908
	movs	r0, #16
	bl 0x0200a8a8
.L_02002380:
	cmp	r7, #5
	bne.n	.L_0200238a
	movs	r0, #204
	bl 0x0200aa40
.L_0200238a:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl 0x0200a768
	cmp	r7, #39
	ble.n	.L_02002380
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200a998
	bl 0x0200a9a0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200a750
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002428
	adds	r3, #15
.L_02002428:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
.L_02002456:
	push	{r6, r7}
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200a8c8
	adds	r7, r0, #0
	bl 0x0200a8b0
	movs	r0, #0
	bl 0x0200a9a8
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200a968
	bl 0x0200a838
	movs	r0, #1
	bl 0x0200a768
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl 0x0200a990
	bl 0x0200a9a0
	movs	r0, #204
	bl 0x0200aa40
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200a8a8
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #256]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_020024ea:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200a790
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200a788
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200a780
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200a780
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #176]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #156]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x02009c88
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020024ea
	movs	r0, #188
	bl 0x0200aa40
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200a958
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200a908
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200a870
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200a870
	bl 0x0200a878
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200a958
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200a8a8
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200a908
	bl 0x0200a8b8
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a3f9
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #156]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200a8c8
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #144]
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	adds	r5, r6, #0
	asrs	r3, r3, #20
	mov	sl, r3
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r7, #0
	adds	r5, #32
	ldrh	r2, [r6, #10]
	cmp	r7, r3
	bge.n	.L_02002684
.L_0200261c:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02002678
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02002678
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200a7e8
	cmp	r0, #0
	bne.n	.L_0200264c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200a248
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200a7f0
	strh	r7, [r6, #12]
	b.n	.L_02002684
.L_0200264c:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02002684
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200a2b8
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200a808
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200a808
	movs	r0, #1
	b.n	.L_02002686
.L_02002678:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_0200261c
.L_02002684:
	movs	r0, #0
.L_02002686:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r2, r3, r6, lr}
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	ldr	r3, [pc, #156]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r1
	bl 0x0200a8c8
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200a800
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200a800
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_020026e6
	cmp	r0, #0
	beq.n	.L_0200273a
.L_020026e6:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200a808
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200a808
	mov	r3, r9
	adds	r2, r7, r3
	mov	r3, r8
	movs	r1, #128
	add	r3, fp
	lsls	r1, r1, #12
	lsls	r3, r3, #20
	adds	r3, r3, r1
	str	r3, [r6, #16]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r2, r2, #20
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	str	r2, [r6, #8]
	str	r2, [r1, #8]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	bl 0x0200a838
	bl 0x0200a450
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_0200273a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r2, r3, r6, lr}
	lsls	r0, r0, #8
	.irp EntryTarget, 0x03000528, 0x03000508, 0x03000514, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000141, 0x08000151, 0x08000169, 0x08000179, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x080001e9, 0x08000291, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x080201c1, 0x080201e1, 0x080201f1, 0x08020219, 0x08020229, 0x08020231, 0x08020279, 0x08020361, 0x08038041, 0x08038211, 0x080ad209, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80c9, 0x080c80d9, 0x080c80f9, 0x080c8101, 0x080c8119, 0x080c8129, 0x080c8169, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8259, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85c1, 0x080c85f9, 0x080c8681, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c87c1, 0x080c87c9, 0x080c87e9, 0x080c87f1, 0x080c8809, 0x080c8811, 0x080c8819, 0x080c8821, 0x080c8831, 0x080c8841, 0x080c8849, 0x08108079, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x06345d01
	.4byte 0x08003b01
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte 0x08071768
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte 0x020010df
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte 0x02007800
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte 0x0800200e
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte 0x08076918
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.4byte 0x00000002
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte 0x08e7ee5a
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte 0x087d19d7
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.4byte 0x01000000
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc40
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00120011
	.4byte 0xffff0013
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
	.4byte 0x00000102
	.4byte 0x00101101
	.4byte 0x00203102
	.4byte 0x00302102
	.4byte 0x00505107
	.4byte 0x00606107
	.4byte 0x00708102
	.4byte 0x00807102
	.4byte 0x00909103
	.4byte 0x00a0a103
	.4byte 0x000001ff
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x00026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x01022000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00022000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01022000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x0a000015
	.4byte 0x020086ad
	.4byte 0x00000002
	.4byte 0x0a000016
	.4byte 0x02008725
	.4byte 0x00000002
	.4byte 0xffff0064
	.4byte 0x020088a5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002a84
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002a85
	.4byte 0x00000008
	.4byte 0xffffffff
	.4byte 0x02008055
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte 0x02008065
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte 0x02008055
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x02008065
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte 0x02008055
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte 0x02008065
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte 0x02008055
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x02008065
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte 0x02008055
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02008065
	.4byte 0x50009985
	.4byte 0x0a000000
	.4byte 0x020082b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200aebc
	.4byte 0x0200aef8
	.4byte 0x0200af34
