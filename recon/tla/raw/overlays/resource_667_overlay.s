.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008515, 0x02008039, 0x02008045, 0x0200804d, 0x0200850d, 0x02008041, 0x02008655
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9600
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9678
	.2byte 0x0200
	push	{lr}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02009498
	cmp	r0, #0
	beq.n	.L_02000060
	ldr	r0, [pc, #4]
	b.n	.L_02000062
.L_02000060:
	ldr	r0, [pc, #4]
.L_02000062:
	pop	{pc}
	.4byte 0x02009840
	.2byte 0x9708
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x02009538
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009540
	bl 0x020095e0
	movs	r1, #0
	bl 0x020094c8
	cmp	r0, #0
	bne.n	.L_0200009c
	movs	r0, #10
	bl 0x020094b0
	adds	r0, r5, #1
	bl 0x02009538
	b.n	.L_020000a8
.L_0200009c:
	movs	r0, #20
	bl 0x020094b0
	adds	r0, r5, #2
	bl 0x02009538
.L_020000a8:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009550
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1cac
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #136]
	adds	r6, r0, #0
	bl 0x020094b8
	movs	r0, #0
	bl 0x020095b8
	adds	r0, r5, #0
	bl 0x02009538
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009540
	bl 0x020095e0
	movs	r1, #0
	bl 0x020094c8
	cmp	r0, #0
	bne.n	.L_02000130
	movs	r0, #20
	bl 0x020094b0
	ldr	r5, [pc, #92]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x020095d8
	movs	r1, #190
	movs	r2, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x020094f0
	movs	r1, #160
	movs	r0, #14
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	movs	r2, #0
	bl 0x02009558
	movs	r0, #10
	bl 0x020094b0
	ldr	r0, [pc, #36]
	movs	r1, #67
	bl 0x02009588
	b.n	.L_0200013e
.L_02000130:
	adds	r0, r5, #2
	bl 0x02009538
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009550
.L_0200013e:
	bl 0x020094c0
	pop	{r5, r6, pc}
	.4byte 0x00001cb5
	.4byte 0x02000240
	.2byte 0x0002
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x02009538
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009540
	bl 0x020095e0
	movs	r1, #0
	bl 0x020094c8
	cmp	r0, #0
	bne.n	.L_02000180
	movs	r0, #10
	bl 0x020094b0
	adds	r0, r5, #1
	bl 0x02009538
	b.n	.L_0200018c
.L_02000180:
	movs	r0, #20
	bl 0x020094b0
	adds	r0, r5, #2
	bl 0x02009538
.L_0200018c:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009550
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1cbb
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	movs	r3, #192
	lsls	r0, r0, #4
	lsls	r3, r3, #18
	adds	r0, #186
	ldr	r5, [r3, #108]
	bl 0x020094a0
	bl 0x020094b8
	movs	r0, #0
	bl 0x020095b8
	ldr	r0, [pc, #116]
	bl 0x02009538
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020001d8
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #1
	movs	r2, #60
	bl 0x02009560
.L_020001d8:
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #18
	movs	r2, #0
	bl 0x02009530
	movs	r1, #8
	adds	r1, #255
	movs	r2, #20
	movs	r0, #18
	bl 0x02009560
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x02009548
	movs	r1, #2
	movs	r0, #18
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x02009548
	movs	r0, #18
	movs	r1, #4
	bl 0x02009510
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	bl 0x020094c0
	pop	{r5, pc}
	.4byte 0x00001cc1
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	movs	r3, #192
	lsls	r0, r0, #4
	lsls	r3, r3, #18
	adds	r0, #187
	ldr	r5, [r3, #108]
	bl 0x020094a0
	bl 0x020094b8
	movs	r0, #0
	bl 0x020095b8
	ldr	r0, [pc, #272]
	bl 0x02009538
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02000274
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x02009560
.L_02000274:
	ldr	r5, [pc, #244]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x020095d8
	movs	r1, #152
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	movs	r2, #152
	bl 0x020094f0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r2, #0
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl 0x02009558
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #2
	movs	r0, #8
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x02009548
	movs	r1, #3
	movs	r0, #8
	bl 0x02009510
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x02009548
	movs	r1, #2
	movs	r0, #8
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x02009558
	movs	r0, #50
	bl 0x020094b0
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x02009558
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x02009548
	movs	r1, #2
	movs	r0, #8
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x02009548
	movs	r0, #8
	movs	r1, #4
	bl 0x02009510
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	bl 0x020094c0
	pop	{r5, pc}
	.4byte 0x00001cc7
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r6, [r5, #108]
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200039e
	ldr	r3, [pc, #380]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #9
	ldr	r1, [r3, #0]
	movs	r2, #0
	bl 0x02009530
	movs	r0, #10
	bl 0x020094b0
.L_0200039e:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #188
	bl 0x020094a0
	bl 0x020094b8
	movs	r0, #0
	bl 0x020095b8
	ldr	r0, [pc, #340]
	bl 0x02009538
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #2
	movs	r0, #9
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009548
	movs	r1, #3
	movs	r0, #9
	bl 0x02009510
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #9
	bl 0x02009530
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #9
	bl 0x02009560
	movs	r0, #9
	movs	r1, #0
	movs	r2, #20
	bl 0x02009548
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #9
	bl 0x02009560
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009548
	movs	r0, #9
	movs	r1, #4
	bl 0x02009510
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009548
	movs	r1, #3
	movs	r0, #9
	bl 0x02009510
	movs	r0, #10
	bl 0x020094b0
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009548
	movs	r1, #2
	movs	r0, #9
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #0
	movs	r0, #9
	bl 0x02009540
	movs	r0, #4
	movs	r1, #0
	bl 0x020094c8
	cmp	r0, #0
	bne.n	.L_020004c4
	movs	r0, #20
	bl 0x020094b0
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009548
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020004e2
.L_020004c4:
	movs	r0, #30
	bl 0x020094b0
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #9
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
.L_020004e2:
	bl 0x020094c0
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02000500
	movs	r1, #208
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
.L_02000500:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1ccd
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9930
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	movs	r0, #15
	bl 0x020094d0
	ldr	r3, [pc, #272]
	ldr	r5, [pc, #276]
	str	r3, [r0, #108]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r6, r5, r3
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #80
	bne.n	.L_0200057c
	movs	r3, #245
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r1, #160
	movs	r3, #14
	strh	r3, [r2, #0]
	lsls	r1, r1, #7
	strh	r3, [r6, #0]
	movs	r2, #0
	movs	r0, #14
	bl 0x02009558
	bl 0x020095a8
	bl 0x020095b0
	movs	r0, #10
	bl 0x020094b0
	ldr	r0, [pc, #224]
	bl 0x02009538
	movs	r0, #14
	movs	r1, #0
	bl 0x02009550
	movs	r0, #48
	adds	r0, #255
	bl 0x020094a8
.L_0200057c:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #81
	bne.n	.L_02000594
	movs	r3, #245
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #8
	strh	r3, [r2, #0]
	strh	r3, [r6, #0]
	bl 0x02008658
.L_02000594:
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02009498
	cmp	r0, #0
	bne.n	.L_020005c4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009498
	cmp	r0, #0
	beq.n	.L_020005c4
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x02009500
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x02009500
.L_020005c4:
	ldr	r3, [pc, #124]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_0200063a
	movs	r0, #152
	movs	r1, #1
	movs	r2, #160
	negs	r1, r1
	lsls	r0, r0, #17
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009580
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	bl 0x020095a8
	bl 0x020095b0
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x02009578
	movs	r0, #144
	movs	r1, #1
	movs	r2, #160
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x02009580
	b.n	.L_0200062a
.L_02000624:
	movs	r0, #1
	bl 0x02009490
.L_0200062a:
	ldr	r3, [pc, #32]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000624
	ldr	r0, [pc, #28]
	movs	r1, #80
	bl 0x02009588
.L_0200063a:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02009489
	.4byte 0x02000240
	.4byte 0x00001cb6
	.4byte 0x03001150
	.2byte 0x0061
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	bl 0x020094b8
	movs	r0, #0
	bl 0x020095b8
	ldr	r0, [pc, #1016]
	bl 0x02009538
	movs	r1, #172
	movs	r2, #192
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	bl 0x02009500
	movs	r1, #204
	movs	r2, #136
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009500
	movs	r1, #156
	movs	r2, #208
	movs	r0, #13
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	bl 0x02009500
	movs	r1, #148
	movs	r2, #240
	movs	r0, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #15
	bl 0x02009500
	movs	r1, #148
	movs	r2, #136
	movs	r0, #6
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009500
	movs	r1, #156
	movs	r2, #136
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009500
	movs	r1, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r0, #172
	movs	r1, #1
	movs	r2, #240
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #15
	movs	r3, #0
	bl 0x02009580
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r1, [r2, #108]
	movs	r3, #218
	lsls	r3, r3, #1
	movs	r6, #214
	movs	r5, #128
	lsls	r5, r5, #1
	mov	r8, r2
	lsls	r6, r6, #1
	adds	r2, r1, r3
	movs	r3, #60
	str	r3, [r2, #0]
	str	r5, [r1, r6]
	bl 0x020095a8
	bl 0x020095b0
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #3
	movs	r0, #13
	bl 0x02009510
	movs	r0, #20
	bl 0x020094b0
	movs	r2, #10
	movs	r0, #13
	movs	r1, #0
	bl 0x02009548
	movs	r0, #9
	movs	r1, #4
	bl 0x02009510
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #9
	movs	r1, #0
	bl 0x02009548
	movs	r1, #2
	movs	r0, #6
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #6
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #3
	movs	r0, #5
	bl 0x02009510
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #9
	movs	r1, #0
	bl 0x02009548
	movs	r0, #6
	movs	r1, #3
	bl 0x02009520
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x02009568
	movs	r0, #5
	movs	r1, #3
	bl 0x02009520
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x02009568
	movs	r0, #40
	bl 0x020094b0
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r0, #6
	movs	r1, #4
	bl 0x02009508
	movs	r1, #4
	movs	r0, #5
	bl 0x02009510
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #3
	movs	r0, #9
	bl 0x02009510
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009558
	movs	r0, #40
	bl 0x020094b0
	mov	r2, r8
	ldr	r3, [r2, #108]
	movs	r0, #128
	lsls	r0, r0, #9
	str	r5, [r3, r6]
	movs	r1, #0
	adds	r0, #2
	bl 0x02009590
	movs	r0, #40
	bl 0x020095a0
	bl 0x020095b0
	movs	r1, #3
	movs	r0, #9
	bl 0x02009510
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #13
	bl 0x02009560
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #13
	bl 0x02009558
	movs	r0, #50
	bl 0x020094b0
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #13
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x02009560
	movs	r1, #6
	movs	r2, #50
	adds	r1, #255
	movs	r0, #6
	bl 0x02009560
	movs	r0, #9
	movs	r1, #3
	bl 0x02009520
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x02009568
	movs	r0, #40
	bl 0x020094b0
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #0
	movs	r2, #0
	movs	r0, #13
	bl 0x02009558
	movs	r0, #35
	bl 0x020094b0
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02009558
	movs	r0, #35
	bl 0x020094b0
	movs	r1, #132
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x02009560
	movs	r0, #6
	movs	r1, #3
	bl 0x02009520
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x02009568
	movs	r0, #5
	movs	r1, #3
	bl 0x02009520
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #5
	bl 0x02009568
	movs	r0, #60
	bl 0x020094b0
	movs	r1, #3
	movs	r0, #9
	bl 0x02009510
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #130
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x02009560
	movs	r1, #4
	movs	r0, #5
	bl 0x02009510
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #8
	adds	r1, #255
	movs	r2, #60
	movs	r0, #6
	bl 0x02009560
	movs	r0, #4
	movs	r1, #6
	movs	r2, #15
	bl 0x02009518
	movs	r1, #6
	movs	r2, #23
	movs	r0, #4
	bl 0x02009518
	movs	r0, #15
	bl 0x020094b0
	movs	r1, #10
	movs	r2, #70
	adds	r1, #255
	movs	r0, #9
	bl 0x02009560
	movs	r0, #78
	bl 0x020095f8
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r0, #2
	movs	r1, #0
	bl 0x02009598
	movs	r1, #0
	movs	r0, #0
	bl 0x02009590
	movs	r0, #60
	bl 0x020095a0
	bl 0x020095b0
	movs	r0, #60
	bl 0x020094b0
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x02009590
	bl 0x020095a8
	bl 0x020095b0
	movs	r0, #20
	bl 0x020094b0
	bl 0x020095c0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	b.n	.L_02000a68
	.2byte 0x24e9
	.2byte 0x0000
.L_02000a68:
	adds	r1, #204
	adds	r2, #102
	bl 0x020094d8
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x020094d8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #11
	ldr	r1, [pc, #1016]
	adds	r2, #204
	bl 0x020094d8
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #1004]
	adds	r2, #204
	bl 0x020094d8
	movs	r0, #9
	movs	r1, #16
	movs	r2, #0
	bl 0x020095d0
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #8
	bl 0x02009560
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #0
	bl 0x020095d0
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x02009558
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #0
	movs	r2, #20
	movs	r0, #8
	bl 0x02009548
	movs	r0, #78
	bl 0x020095f8
	movs	r1, #172
	movs	r2, #176
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009500
	movs	r2, #16
	movs	r0, #11
	movs	r1, #0
	negs	r2, r2
	bl 0x020095d0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r1, #192
	movs	r0, #13
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #160
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #11
	adds	r1, #204
	adds	r2, #102
	bl 0x020094d8
	movs	r2, #24
	movs	r0, #11
	movs	r1, #16
	negs	r2, r2
	bl 0x020095d0
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02009558
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02009558
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #11
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x02009560
	movs	r1, #0
	movs	r2, #10
	movs	r0, #11
	bl 0x02009548
	movs	r0, #49
	bl 0x020095f8
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #13
	bl 0x02009560
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x02009560
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #7
	bl 0x02009560
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x02009560
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #6
	bl 0x02009560
	adds	r1, r5, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x02009560
	adds	r1, r5, #0
	movs	r2, #60
	movs	r0, #9
	bl 0x02009560
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x02009548
	movs	r0, #11
	movs	r1, #4
	bl 0x02009510
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x02009560
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #11
	bl 0x02009560
	movs	r1, #0
	movs	r2, #0
	movs	r0, #11
	bl 0x02009558
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #20
	bl 0x02009548
	movs	r1, #172
	movs	r2, #176
	movs	r0, #12
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02009500
	movs	r2, #16
	movs	r1, #0
	negs	r2, r2
	movs	r0, #12
	bl 0x020095d0
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x02009560
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #13
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x02009560
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	movs	r0, #9
	bl 0x02009560
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x02009560
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #11
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	adds	r1, r5, #0
	movs	r2, #50
	movs	r0, #12
	bl 0x02009560
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	movs	r0, #12
	bl 0x020095d0
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	adds	r1, r5, #0
	movs	r2, #40
	movs	r0, #12
	bl 0x02009560
	movs	r2, #10
	movs	r1, #0
	movs	r0, #12
	bl 0x02009548
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #2
	movs	r0, #8
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02009558
	movs	r2, #0
	movs	r1, #0
	movs	r0, #12
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #2
	movs	r0, #11
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #160
	movs	r0, #11
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009558
	b.n	.L_02000e88
	.2byte 0x9999
	.2byte 0x0001
.L_02000e88:
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #12
	bl 0x02009558
	movs	r0, #35
	bl 0x020094b0
	movs	r1, #2
	movs	r0, #12
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x02009548
	movs	r1, #3
	movs	r0, #11
	bl 0x02009510
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #3
	movs	r0, #12
	bl 0x02009510
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x02009560
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r0, #12
	movs	r1, #6
	movs	r2, #0
	bl 0x02009518
	movs	r0, #11
	movs	r1, #6
	movs	r2, #25
	bl 0x02009518
	movs	r0, #12
	movs	r1, #0
	movs	r2, #24
	bl 0x020095c8
	movs	r1, #16
	movs	r0, #11
	negs	r1, r1
	movs	r2, #16
	bl 0x020095d0
	movs	r1, #0
	movs	r2, #24
	movs	r0, #11
	bl 0x020095c8
	movs	r0, #12
	bl 0x020094f8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x02009500
	movs	r0, #11
	bl 0x020094f8
	movs	r2, #0
	movs	r1, #0
	movs	r0, #11
	bl 0x02009500
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #4
	movs	r0, #9
	bl 0x02009510
	movs	r0, #10
	bl 0x020094b0
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #0
	movs	r2, #0
	movs	r0, #13
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x020094d8
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #8
	lsls	r1, r1, #9
	bl 0x020094d8
	movs	r0, #4
	movs	r1, #9
	bl 0x020095d8
	movs	r0, #13
	movs	r1, #9
	bl 0x020095d8
	movs	r0, #7
	movs	r1, #9
	bl 0x020095d8
	movs	r0, #5
	movs	r1, #9
	bl 0x020095d8
	movs	r0, #6
	movs	r1, #9
	bl 0x020095d8
	movs	r0, #9
	movs	r1, #16
	movs	r2, #0
	bl 0x020095d0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #50
	bl 0x020095d0
	movs	r1, #32
	negs	r1, r1
	movs	r2, #4
	movs	r0, #9
	bl 0x020095d0
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #4
	bl 0x020094e0
	movs	r0, #13
	bl 0x020094e0
	movs	r0, #7
	bl 0x020094e0
	movs	r0, #5
	bl 0x020094e0
	movs	r0, #6
	bl 0x020094e0
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #13
	bl 0x02009558
	movs	r0, #50
	bl 0x020094b0
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x02009560
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009558
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #13
	bl 0x02009558
	movs	r0, #35
	bl 0x020094b0
	movs	r1, #2
	movs	r0, #9
	bl 0x02009528
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #32
	negs	r1, r1
	movs	r2, #0
	movs	r0, #8
	bl 0x020095d0
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #8
	movs	r1, #6
	movs	r2, #15
	bl 0x02009518
	movs	r0, #8
	movs	r1, #6
	movs	r2, #23
	bl 0x02009518
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #0
	movs	r2, #10
	movs	r0, #9
	bl 0x02009548
	movs	r0, #78
	bl 0x020095f8
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #3
	movs	r0, #8
	bl 0x02009510
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #24
	bl 0x020095c8
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #16
	bl 0x020095d0
	movs	r1, #0
	movs	r2, #24
	movs	r0, #8
	bl 0x020095c8
	movs	r0, #9
	bl 0x020094f8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x02009500
	movs	r0, #8
	bl 0x020094f8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x02009500
	movs	r0, #30
	bl 0x020094b0
	bl 0x020095c0
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x02009560
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x02009560
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009558
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x02009558
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #13
	bl 0x02009558
	movs	r0, #20
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #5
	bl 0x02009540
	movs	r0, #4
	movs	r1, #0
	bl 0x020094c8
	cmp	r0, #0
	bne.n	.L_02001286
	bl 0x020095f0
	movs	r0, #30
	bl 0x020094b0
	movs	r0, #6
	movs	r1, #4
	bl 0x02009510
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x02009548
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020012b6
.L_02001286:
	movs	r0, #40
	bl 0x020094b0
	movs	r1, #3
	movs	r0, #6
	bl 0x02009510
	movs	r0, #10
	bl 0x020094b0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #6
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
.L_020012b6:
	movs	r1, #3
	movs	r0, #13
	bl 0x02009510
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #13
	movs	r1, #0
	movs	r2, #10
	bl 0x02009548
	movs	r1, #224
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009558
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009558
	movs	r0, #30
	bl 0x020094b0
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #13
	bl 0x02009560
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #13
	movs	r1, #0
	bl 0x02009548
	movs	r1, #3
	movs	r0, #13
	bl 0x02009510
	movs	r0, #10
	bl 0x020094b0
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #13
	movs	r1, #0
	bl 0x02009548
	movs	r0, #4
	movs	r1, #3
	bl 0x02009508
	movs	r0, #6
	movs	r1, #3
	bl 0x02009508
	movs	r0, #5
	movs	r1, #3
	bl 0x02009508
	movs	r1, #3
	movs	r0, #7
	bl 0x02009510
	movs	r0, #25
	bl 0x020094b0
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #296]
	bl 0x020094d8
	movs	r0, #7
	movs	r1, #2
	bl 0x02009508
	ldr	r3, [pc, #288]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x020094d0
	cmp	r0, #0
	beq.n	.L_02001382
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x020094e8
.L_02001382:
	movs	r0, #7
	bl 0x020094f8
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x02009500
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #228]
	adds	r2, #153
	bl 0x020094d8
	movs	r0, #5
	movs	r1, #2
	bl 0x02009508
	ldr	r0, [r5, #0]
	bl 0x020094d0
	cmp	r0, #0
	beq.n	.L_020013c0
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x020094e8
.L_020013c0:
	movs	r0, #5
	bl 0x020094f8
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02009500
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #168]
	adds	r2, #153
	bl 0x020094d8
	movs	r0, #6
	movs	r1, #2
	bl 0x02009508
	ldr	r0, [r5, #0]
	bl 0x020094d0
	cmp	r0, #0
	beq.n	.L_020013fe
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x020094e8
.L_020013fe:
	movs	r0, #6
	bl 0x020094f8
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02009500
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #13
	ldr	r1, [pc, #104]
	adds	r2, #153
	bl 0x020094d8
	movs	r0, #13
	movs	r1, #2
	bl 0x02009508
	ldr	r0, [r5, #0]
	bl 0x020094d0
	cmp	r0, #0
	beq.n	.L_0200143c
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #13
	bl 0x020094e8
.L_0200143c:
	movs	r0, #13
	bl 0x020094f8
	movs	r2, #0
	movs	r0, #13
	movs	r1, #0
	bl 0x02009500
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x02009570
	bl 0x020094c0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #16
	str	r3, [r2, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r0, #48
	adds	r3, #93
	str	r3, [r2, #0]
	adds	r0, #255
	bl 0x020094a8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x020095e8
	pop	{pc}
	.irp EntryTarget, 0x080000c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80b1, 0x080c80c1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8269, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b9, 0x080c84e1, 0x080c8571, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8779, 0x080c8879, 0x080c8931, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0050
	.4byte 0x000002f8
	.4byte 0x40000140
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0051
	.4byte 0x00000138
	.4byte 0xe0000078
	.4byte 0x00e00000
	.4byte 0x01d00010
	.4byte 0x000000c0
	.4byte 0xffff0063
	.4byte 0x00000208
	.4byte 0xc0000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000061
	.4byte 0x1010e05f
	.4byte 0xffffffff
	.4byte 0x10207061
	.4byte 0xffffffff
	.4byte 0x10308061
	.4byte 0xffffffff
	.4byte 0x10409061
	.4byte 0xffffffff
	.4byte 0x1050a061
	.4byte 0xffffffff
	.4byte 0x1060f061
	.4byte 0xffffffff
	.4byte 0x10702061
	.4byte 0xffffffff
	.4byte 0x10803061
	.4byte 0xffffffff
	.4byte 0x10904061
	.4byte 0xffffffff
	.4byte 0x10a05061
	.4byte 0xffffffff
	.4byte 0x10b0c061
	.4byte 0xffffffff
	.4byte 0x10c0b061
	.4byte 0xffffffff
	.4byte 0x10d0e061
	.4byte 0xffffffff
	.4byte 0x10e0d061
	.4byte 0xffffffff
	.4byte 0x10f06061
	.4byte 0xffffffff
	.4byte 0x11011061
	.4byte 0xffffffff
	.4byte 0x11110061
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00010000
	.4byte 0xffff0018
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000e000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00002000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0001c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0000a000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0001a000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0xffff00ba
	.4byte 0x00000002
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00004000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00008000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00018000
	.4byte 0xffff0018
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00018000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00010000
	.4byte 0xffff00e7
	.4byte 0x00000002
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00020000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
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
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x18a7000a
	.4byte 0x00002511
	.4byte 0x00008d15
	.4byte 0x18a7000a
	.4byte 0x00002523
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001cab
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200806d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001caf
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001cb0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001cb1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001cb2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001cb3
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001cb4
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020080b9
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001cb8
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001cb9
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001cba
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008151
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001cbe
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001cbf
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001cc0
	.4byte 0x00000000
	.4byte 0x08ba0012
	.4byte 0x0200819d
	.4byte 0x00008d15
	.4byte 0x08ba0412
	.4byte 0x0200819d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001cc3
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001cc4
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001cc5
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001cc6
	.4byte 0x00000000
	.4byte 0x08bb0008
	.4byte 0x02008239
	.4byte 0x00008d15
	.4byte 0x08bb0408
	.4byte 0x02008239
	.4byte 0x00000000
	.4byte 0x08bc0009
	.4byte 0x02008371
	.4byte 0x00008d15
	.4byte 0x08bc0409
	.4byte 0x02008371
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001cd8
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001cd9
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001cda
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001cdb
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403055
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
