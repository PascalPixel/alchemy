.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020087d5, 0x02008039, 0x02008045, 0x0200804d, 0x02008789, 0x02008041, 0x02008a19
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9dc0
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9df0
	.2byte 0x0200
	push	{lr}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_02000072
	movs	r0, #129
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009bb4
	cmp	r0, #0
	bne.n	.L_0200006e
	ldr	r0, [pc, #28]
	b.n	.L_02000086
.L_0200006e:
	ldr	r0, [pc, #28]
	b.n	.L_02000086
.L_02000072:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_02000084
	ldr	r0, [pc, #12]
	b.n	.L_02000086
.L_02000084:
	ldr	r0, [pc, #12]
.L_02000086:
	pop	{pc}
	.4byte 0x0200a5b0
	.4byte 0x0200a358
	.4byte 0x0200a100
	.2byte 0x9e48
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl 0x02009be4
	movs	r0, #0
	bl 0x02009cfc
	movs	r0, #158
	bl 0x02009d4c
	ldrh	r1, [r5, #4]
	ldrh	r2, [r5, #6]
	ldr	r0, [r5, #0]
	bl 0x02009bc4
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x02009bfc
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x02009c04
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009c44
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009bdc
	adds	r0, r6, #0
	bl 0x02009cd4
	bl 0x02009cdc
	bl 0x02009ce4
	bl 0x02009bec
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #16]
	adds	r1, r0, #0
	lsls	r0, r1, #3
	adds	r0, r0, r3
	movs	r2, #0
	bl 0x02008098
	pop	{pc}
	.2byte 0x0000
	.2byte 0xa77c
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #7
	bl 0x02009d44
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #8
	bl 0x02009d44
	pop	{pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #139
	lsls	r0, r0, #4
	bl 0x02009bb4
	cmp	r0, #0
	bne.n	.L_0200018a
	ldr	r5, [pc, #76]
	adds	r0, r5, #0
	bl 0x02009c74
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009c7c
	bl 0x02009d34
	movs	r1, #0
	bl 0x02009bf4
	cmp	r0, #0
	bne.n	.L_0200017a
	adds	r0, r5, #1
	bl 0x02009c74
	movs	r0, #139
	lsls	r0, r0, #4
	bl 0x02009bbc
	b.n	.L_02000180
.L_0200017a:
	adds	r0, r5, #2
	bl 0x02009c74
.L_02000180:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c8c
	b.n	.L_02000198
.L_0200018a:
	ldr	r0, [pc, #20]
	bl 0x02009c74
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c8c
.L_02000198:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001ac1
	.2byte 0x1ac4
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x02009c74
	movs	r1, #0
.L_020001b2:
	adds	r0, r6, #0
	bl 0x02009c7c
	bl 0x02009d34
	movs	r1, #0
	bl 0x02009bf4
	cmp	r0, #0
	bne.n	.L_020001d4
	movs	r0, #10
	bl 0x02009bdc
	adds	r0, r5, #1
	bl 0x02009c74
	b.n	.L_020001e0
.L_020001d4:
	movs	r0, #20
	bl 0x02009bdc
	adds	r0, r5, #2
	bl 0x02009c74
.L_020001e0:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c8c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1aec
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x02009c74
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009c7c
	bl 0x02009d34
	movs	r1, #0
	bl 0x02009bf4
	cmp	r0, #0
	bne.n	.L_02000220
	movs	r0, #10
	bl 0x02009bdc
	adds	r0, r5, #1
	bl 0x02009c74
	b.n	.L_0200022c
.L_02000220:
	movs	r0, #20
	bl 0x02009bdc
	adds	r0, r5, #2
	bl 0x02009c74
.L_0200022c:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c8c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1b44
	.2byte 0x0000
	push	{lr}
	movs	r0, #1
	.2byte 0xf001
	.2byte 0xfb8a
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl 0x02009958
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #25
	movs	r1, #2
	bl 0x02009c9c
	pop	{pc}
	push	{lr}
	movs	r0, #25
	movs	r1, #3
	bl 0x02009c9c
	pop	{pc}
	push	{r5, lr}
	adds	r5, r1, #0
	ldr	r0, [pc, #16]
	bl 0x02009d2c
	adds	r0, r5, #0
	bl 0x02009bfc
	movs	r3, #0
	adds	r0, #35
	strb	r3, [r0, #0]
	pop	{r5, pc}
	.2byte 0x9d54
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #100]
	bl 0x02009c74
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	ldr	r1, [pc, #88]
	movs	r2, #0
	movs	r0, #19
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #19
	bl 0x02009ca4
	movs	r2, #10
	movs	r0, #19
	movs	r1, #0
	bl 0x02009c84
	movs	r0, #24
	movs	r1, #3
	bl 0x02009c5c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #24
	bl 0x02009cac
	movs	r0, #40
	bl 0x02009bdc
	movs	r1, #128
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c84
	pop	{pc}
	.4byte 0x00001c20
	.2byte 0xff00
	.2byte 0xffff
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #181
	bl 0x02009bbc
	movs	r0, #202
	adds	r0, #255
	bl 0x02009bd4
	bl 0x02009be4
	movs	r0, #0
	bl 0x02009cfc
	ldr	r0, [pc, #120]
	bl 0x02009c74
	movs	r0, #10
	bl 0x02009bdc
	ldr	r3, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #27
	bl 0x02009c6c
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #27
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r0, #1
	movs	r2, #10
	negs	r0, r0
	movs	r1, #0
	bl 0x02009c84
	movs	r0, #27
	movs	r1, #1
	bl 0x02009c44
	movs	r0, #27
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c54
	movs	r0, #27
	movs	r1, #6
	movs	r2, #23
	bl 0x02009c54
	movs	r1, #0
	movs	r2, #0
	movs	r0, #27
	bl 0x02009c84
	movs	r0, #27
	bl 0x02008398
	movs	r1, #128
	movs	r0, #27
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	bl 0x02009bec
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001c42
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #181
	bl 0x02009bb4
	cmp	r0, #0
	bne.n	.L_020003be
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	ldr	r0, [pc, #180]
	strh	r3, [r2, #0]
	b.n	.L_020003ce
.L_020003be:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #182
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_020003dc
	ldr	r0, [pc, #164]
.L_020003ce:
	bl 0x02009c74
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009c8c
	b.n	.L_0200046c
.L_020003dc:
	ldr	r0, [pc, #152]
	bl 0x02009c74
	movs	r0, #10
	bl 0x02009bdc
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x02009c4c
	movs	r2, #10
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009c84
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02009c64
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02009c7c
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #4
	movs	r1, #0
	bl 0x02009bf4
	cmp	r0, #0
	bne.n	.L_0200044a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #182
	bl 0x02009bbc
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #10
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009c84
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200046c
.L_0200044a:
	movs	r0, #30
	bl 0x02009bdc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
.L_0200046c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001c3e
	.4byte 0x00001c49
	.2byte 0x1c45
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #247
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02009bbc
	bl 0x02009be4
	movs	r0, #0
	bl 0x02009cfc
	ldr	r0, [pc, #464]
	bl 0x02009c74
	movs	r0, #140
	movs	r1, #1
	movs	r2, #248
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x02009cc4
	bl 0x02009ccc
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #29
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #29
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #31
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r2, #0
	movs	r0, #30
	lsls	r1, r1, #7
	bl 0x02009c94
	movs	r1, #5
	movs	r0, #29
	bl 0x02009c44
	movs	r0, #89
	bl 0x02009d4c
	ldr	r5, [pc, #360]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r3, #29
	str	r3, [r5, #0]
	movs	r1, #3
	movs	r0, #0
	bl 0x02009cec
	movs	r3, #4
	str	r3, [r5, #0]
	movs	r0, #50
	bl 0x02009bdc
	movs	r0, #28
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c54
	movs	r0, #28
	movs	r1, #6
	movs	r2, #23
	bl 0x02009c54
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #29
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #29
	bl 0x02009c4c
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #30
	bl 0x02009ca4
	movs	r0, #30
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #29
	bl 0x02009ca4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #29
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #29
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #29
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #2
	movs	r0, #30
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #30
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r2, #0
	movs	r1, #0
	movs	r0, #29
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #29
	bl 0x02009c4c
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #31
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c54
	movs	r0, #31
	movs	r1, #6
	movs	r2, #23
	bl 0x02009c54
	movs	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r2, #0
	movs	r0, #30
	movs	r1, #0
	bl 0x02009c94
	movs	r1, #3
	movs	r0, #30
	bl 0x02009c4c
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #31
	lsls	r1, r1, #7
	bl 0x02009c94
	movs	r0, #31
	movs	r1, #6
	bl 0x02009c44
	movs	r1, #192
	movs	r2, #0
	movs	r0, #30
	lsls	r1, r1, #8
	bl 0x02009c94
	movs	r0, #30
	movs	r1, #6
	bl 0x02009c44
	movs	r1, #128
	movs	r0, #29
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r2, #0
	movs	r0, #28
	lsls	r1, r1, #8
	bl 0x02009c94
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x02009cb4
	bl 0x02009ccc
	bl 0x02009bec
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001c4c
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #185
	ldr	r5, [pc, #224]
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_02000690
	adds	r0, r5, #4
	bl 0x02009c74
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c7c
	b.n	.L_02000754
.L_02000690:
	adds	r0, r5, #0
	bl 0x02009c74
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02009c7c
	bl 0x02009d34
	movs	r1, #0
	bl 0x02009bf4
	cmp	r0, #0
	beq.n	.L_020006c2
	movs	r0, #20
	bl 0x02009bdc
	adds	r0, r5, #1
	bl 0x02009c74
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c8c
	b.n	.L_02000754
.L_020006c2:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #185
	bl 0x02009bbc
	movs	r0, #20
	bl 0x02009bdc
	adds	r0, r5, #2
	bl 0x02009c74
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c8c
	ldr	r5, [pc, #120]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #172
	ldr	r0, [r5, #0]
	movs	r1, #72
	lsls	r2, r2, #1
	bl 0x02009c24
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #128
	movs	r2, #128
	adds	r0, r6, #0
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r2, #164
	adds	r0, r6, #0
	movs	r1, #56
	lsls	r2, r2, #1
	bl 0x02009c24
	movs	r1, #160
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	adds	r0, r6, #0
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009c8c
.L_02000754:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001ca5
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r1, #184
	movs	r2, #138
	movs	r0, #64
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x02009cf4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r1, #1
	movs	r2, #1
	movs	r0, #64
	negs	r1, r1
	negs	r2, r2
	bl 0x02009cf4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_020007ae
	movs	r0, #129
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009bb4
	cmp	r0, #0
	bne.n	.L_020007aa
	ldr	r0, [pc, #28]
	b.n	.L_020007c2
.L_020007aa:
	ldr	r0, [pc, #28]
	b.n	.L_020007c2
.L_020007ae:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_020007c0
	ldr	r0, [pc, #12]
	b.n	.L_020007c2
.L_020007c0:
	ldr	r0, [pc, #12]
.L_020007c2:
	pop	{pc}
	.4byte 0x0200b154
	.4byte 0x0200ae48
	.4byte 0x0200aad0
	.2byte 0xa7f4
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	movs	r0, #245
	adds	r3, r3, r2
	lsls	r0, r0, #3
	subs	r2, #172
	str	r2, [r3, #0]
	adds	r0, #255
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_020008aa
	movs	r0, #129
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009bb4
	cmp	r0, #0
	bne.n	.L_02000836
	movs	r0, #24
	bl 0x02009bfc
	ldr	r5, [pc, #516]
	str	r5, [r0, #108]
	movs	r0, #19
	bl 0x02009bfc
	movs	r1, #2
	str	r5, [r0, #108]
	movs	r0, #20
	bl 0x02009c9c
	movs	r0, #14
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #16
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #10
	movs	r1, #1
	bl 0x02009c9c
	b.n	.L_02000a0a
.L_02000836:
	movs	r0, #14
	bl 0x02009bfc
	ldr	r5, [pc, #464]
	str	r5, [r0, #108]
	movs	r0, #20
	bl 0x02009bfc
	str	r5, [r0, #108]
	movs	r0, #22
	bl 0x02009bfc
	movs	r1, #2
	str	r5, [r0, #108]
	movs	r0, #13
	bl 0x02009c9c
	movs	r0, #19
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #23
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #24
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #27
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #28
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #29
	movs	r1, #1
	bl 0x02009c9c
	movs	r0, #30
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #31
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #12
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #10
	movs	r1, #3
	bl 0x02009c9c
	b.n	.L_02000a0a
.L_020008aa:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_0200098a
	movs	r0, #14
	bl 0x02009bfc
	ldr	r5, [pc, #336]
	str	r5, [r0, #108]
	movs	r0, #20
	bl 0x02009bfc
	movs	r1, #2
	str	r5, [r0, #108]
	movs	r0, #13
	bl 0x02009c9c
	movs	r0, #19
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #23
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #24
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #22
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #27
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #28
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #29
	movs	r1, #1
	bl 0x02009c9c
	movs	r0, #30
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #31
	movs	r1, #3
	bl 0x02009c9c
	movs	r1, #3
	movs	r0, #12
	bl 0x02009c9c
	movs	r0, #26
	bl 0x02009bfc
	movs	r1, #9
	bl 0x02009d1c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #181
	bl 0x02009bb4
	cmp	r0, #0
	bne.n	.L_02000944
	movs	r0, #27
	movs	r1, #5
	bl 0x02009c44
.L_02000944:
	movs	r0, #247
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_02000962
	movs	r0, #30
	movs	r1, #6
	bl 0x02009c44
	movs	r0, #31
	movs	r1, #6
	bl 0x02009c44
.L_02000962:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #185
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_02000a0a
	movs	r1, #224
	movs	r2, #164
	movs	r0, #12
	lsls	r1, r1, #14
	lsls	r2, r2, #17
	bl 0x02009c3c
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	b.n	.L_02000a0a
.L_0200098a:
	movs	r0, #13
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #23
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #10
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #24
	bl 0x02009bfc
	ldr	r3, [pc, #100]
	str	r3, [r0, #108]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #179
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_020009c8
	movs	r1, #134
	movs	r2, #190
	movs	r0, #14
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009c3c
.L_020009c8:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #180
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_020009e0
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
.L_020009e0:
	ldr	r0, [pc, #48]
	bl 0x02009d24
	movs	r0, #26
	bl 0x02009bfc
	movs	r1, #9
	bl 0x02009d1c
	movs	r0, #27
	bl 0x02009bfc
	movs	r1, #9
	bl 0x02009d1c
	movs	r0, #28
	bl 0x02009bfc
	movs	r1, #9
	bl 0x02009d1c
.L_02000a0a:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02008a99
	.2byte 0x9d54
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	sub	sp, #8
	bl 0x02009bb4
	cmp	r0, #0
	beq.n	.L_02000a8c
	ldr	r3, [pc, #104]
	ldrh	r1, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #246
	strh	r1, [r2, #0]
	adds	r3, #2
	ldrh	r1, [r3, #0]
	adds	r2, #2
	strh	r1, [r2, #0]
	adds	r3, #2
	ldrh	r1, [r3, #0]
	adds	r2, #2
	strh	r1, [r2, #0]
	ldrh	r2, [r3, #2]
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #252
	strh	r2, [r3, #0]
	movs	r3, #37
	movs	r2, #33
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #37
	movs	r1, #18
	movs	r2, #2
	movs	r3, #2
	bl 0x02009bcc
	movs	r3, #39
	movs	r2, #31
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #37
	movs	r1, #18
	movs	r2, #2
	movs	r3, #3
	bl 0x02009bcc
	movs	r3, #35
	movs	r2, #95
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #50
	movs	r1, #80
	movs	r2, #7
	movs	r3, #5
	bl 0x02009bcc
.L_02000a8c:
	movs	r0, #0
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0116
	.2byte 0x0500
	push	{lr}
	bl 0x02009d3c
	pop	{pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #179
	bl 0x02009bbc
	bl 0x02009be4
	movs	r0, #0
	bl 0x02009cfc
	ldr	r0, [pc, #200]
	bl 0x02009c74
	movs	r1, #128
	movs	r2, #128
	movs	r0, #29
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #30
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #31
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #32
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #33
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #34
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #35
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #14
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	cmp	r5, #20
	bne.n	.L_02000b4c
	movs	r2, #8
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d0c
.L_02000b4c:
	movs	r1, #204
	movs	r2, #218
	movs	r0, #31
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r1, #196
	movs	r2, #222
	movs	r0, #32
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	cmp	r5, #20
	bne.n	.L_02000b88
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	b.n	.L_02000b92
	.2byte 0x1aca
	.2byte 0x0000
.L_02000b88:
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
.L_02000b92:
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #200
	movs	r1, #1
	movs	r2, #202
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x02009cc4
	bl 0x02009ccc
	movs	r2, #96
	movs	r0, #31
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #96
	negs	r2, r2
	movs	r0, #32
	movs	r1, #0
	bl 0x02009d0c
	movs	r1, #1
	movs	r0, #31
	bl 0x02009c44
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #160
	movs	r0, #31
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #160
	movs	r0, #31
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #196
	movs	r2, #218
	movs	r0, #33
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r1, #204
	movs	r2, #218
	movs	r0, #35
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #128
	movs	r0, #31
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #48
	movs	r0, #35
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #48
	movs	r1, #0
	negs	r2, r2
	movs	r0, #33
	bl 0x02009d0c
	movs	r0, #35
	bl 0x02009c34
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #35
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #31
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #31
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x02009ca4
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #33
	bl 0x02009c94
	movs	r0, #40
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #33
	bl 0x02009c64
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #33
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #33
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #160
	movs	r0, #31
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #31
	movs	r1, #3
	bl 0x02009c44
	movs	r1, #3
	movs	r0, #32
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #48
	movs	r0, #35
	movs	r1, #32
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #48
	movs	r1, #32
	negs	r2, r2
	movs	r0, #33
	bl 0x02009d0c
	movs	r0, #35
	bl 0x02009c34
	movs	r1, #160
	movs	r0, #35
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	movs	r2, #0
	movs	r0, #33
	lsls	r1, r1, #7
	bl 0x02009c94
	movs	r1, #3
	movs	r0, #35
	bl 0x02009c9c
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #200
	movs	r2, #218
	movs	r0, #34
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r0, #34
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #128
	movs	r0, #31
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #32
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #33
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #32
	negs	r2, r2
	movs	r1, #0
	movs	r0, #34
	bl 0x02009d0c
	movs	r0, #1
	bl 0x02009bdc
	movs	r1, #5
	movs	r0, #34
	bl 0x02009c44
	movs	r0, #90
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #34
	bl 0x02009c64
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #34
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r2, #16
	negs	r2, r2
	movs	r1, #0
	movs	r0, #34
	bl 0x02009d0c
	movs	r0, #1
	bl 0x02009bdc
	movs	r1, #5
	movs	r0, #34
	bl 0x02009c44
	movs	r0, #60
	bl 0x02009bdc
	movs	r2, #32
	movs	r0, #34
	movs	r1, #8
	negs	r2, r2
	bl 0x02009d0c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #34
	bl 0x02009c94
	movs	r0, #1
	bl 0x02009bdc
	movs	r1, #5
	movs	r0, #34
	bl 0x02009c44
	movs	r0, #30
	bl 0x02009bdc
	movs	r1, #196
	movs	r2, #218
	movs	r0, #30
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r1, #204
	movs	r2, #218
	movs	r0, #29
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r2, #32
	movs	r0, #30
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #32
	negs	r2, r2
	movs	r0, #29
	movs	r1, #0
	bl 0x02009d0c
	movs	r1, #1
	movs	r0, #30
	bl 0x02009c44
	movs	r0, #1
	bl 0x02009bdc
	movs	r0, #29
	movs	r1, #5
	bl 0x02009c44
	movs	r0, #31
	movs	r1, #30
	bl 0x02009d14
	movs	r0, #32
	movs	r1, #30
	bl 0x02009d14
	movs	r0, #33
	movs	r1, #30
	bl 0x02009d14
	movs	r0, #34
	movs	r1, #30
	bl 0x02009d14
	movs	r1, #30
	movs	r0, #35
	bl 0x02009d14
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #30
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #30
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #30
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #30
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c54
	movs	r2, #23
	movs	r0, #30
	movs	r1, #6
	bl 0x02009c54
	movs	r0, #34
	movs	r1, #1
	bl 0x02009c44
	movs	r2, #10
	movs	r0, #30
	movs	r1, #0
	bl 0x02009c84
	ldr	r1, [pc, #1020]
	movs	r0, #29
	bl 0x02009c0c
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #32
	movs	r0, #30
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d0c
	movs	r1, #32
	movs	r2, #0
	negs	r1, r1
	movs	r0, #30
	bl 0x02009d0c
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #30
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #30
	movs	r1, #0
	bl 0x02009c84
	movs	r0, #29
	movs	r1, #4
	bl 0x02009c4c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #30
	bl 0x02009ca4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #30
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #29
	bl 0x02009c64
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #29
	movs	r1, #0
	bl 0x02009c84
	movs	r0, #29
	movs	r1, #5
	bl 0x02009c44
	movs	r0, #30
	movs	r1, #64
	movs	r2, #0
	bl 0x02009d0c
	movs	r1, #160
	movs	r0, #29
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #30
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #30
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #1
	movs	r0, #29
	bl 0x02009c44
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #29
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #10
	movs	r1, #0
	movs	r0, #29
	bl 0x02009c84
	movs	r0, #31
	bl 0x02009c1c
	movs	r0, #32
	bl 0x02009c1c
	movs	r0, #33
	bl 0x02009c1c
	movs	r0, #34
	bl 0x02009c1c
	movs	r0, #35
	bl 0x02009c1c
	movs	r1, #5
	movs	r0, #29
	bl 0x02009c44
	movs	r0, #50
	bl 0x02009bdc
	movs	r1, #160
	movs	r0, #31
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #40
	bl 0x02009bdc
	movs	r1, #192
	movs	r0, #31
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	ldr	r1, [pc, #680]
	movs	r0, #14
	bl 0x02009c0c
	movs	r0, #31
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c54
	movs	r0, #31
	movs	r1, #6
	movs	r2, #23
	bl 0x02009c54
	movs	r1, #0
	movs	r2, #10
	movs	r0, #31
	bl 0x02009c84
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #128
	lsls	r1, r1, #5
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #32
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #1
	movs	r0, #29
	bl 0x02009c44
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #29
	bl 0x02009ca4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #29
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #29
	movs	r1, #0
	bl 0x02009c84
	movs	r0, #29
	movs	r1, #1
	bl 0x02009cb4
	bl 0x02009ccc
	movs	r1, #244
	movs	r2, #202
	movs	r0, #29
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c24
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r0, #32
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #35
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c94
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #29
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #29
	movs	r1, #4
	bl 0x02009c4c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x02009cbc
	movs	r0, #162
	movs	r1, #1
	movs	r2, #134
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #18
	bl 0x02009cc4
	bl 0x02009ccc
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #29
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r0, #220
	movs	r1, #1
	movs	r2, #202
	movs	r3, #1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x02009cc4
	bl 0x02009ccc
	movs	r0, #128
	movs	r1, #128
	lsls	r1, r1, #6
	lsls	r0, r0, #9
	bl 0x02009cbc
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #33
	movs	r1, #32
	movs	r2, #36
	bl 0x02009d0c
	movs	r0, #33
	movs	r1, #32
	movs	r2, #0
	bl 0x02009d0c
	movs	r1, #252
	movs	r2, #202
	movs	r0, #33
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c24
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #33
	bl 0x02009c94
	movs	r0, #30
	bl 0x02009bdc
	movs	r0, #33
	movs	r1, #3
	bl 0x02009c5c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #33
	bl 0x02009cac
	movs	r0, #50
	bl 0x02009bdc
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #33
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #33
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #33
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #2
	movs	r0, #29
	bl 0x02009c64
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #29
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #29
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #34
	bl 0x02009ca4
	movs	r0, #34
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #29
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #29
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #29
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #2
	movs	r0, #30
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #30
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #160
	movs	r0, #31
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #32
	bl 0x02009c94
	b.n	.L_0200133c
	.2byte 0x0000
	.4byte 0x0200b3a0
	.2byte 0xb3f8
	.2byte 0x0200
.L_0200133c:
	movs	r0, #30
	bl 0x02009bdc
	movs	r0, #31
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c54
	movs	r0, #31
	movs	r1, #6
	movs	r2, #23
	bl 0x02009c54
	movs	r2, #10
	movs	r0, #31
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #3
	movs	r0, #32
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #32
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #2
	movs	r0, #29
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #192
	movs	r0, #31
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	movs	r0, #32
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c94
	movs	r2, #10
	movs	r0, #29
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #3
	movs	r0, #30
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #30
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #204
	movs	r2, #174
	movs	r0, #17
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r1, #204
	movs	r2, #190
	movs	r0, #17
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c24
	movs	r1, #224
	movs	r2, #198
	movs	r0, #17
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c24
	movs	r1, #224
	movs	r2, #202
	movs	r0, #17
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c24
	movs	r0, #30
	movs	r1, #8
	movs	r2, #0
	bl 0x02009d0c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #30
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #17
	bl 0x02009ca4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #30
	bl 0x02009ca4
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #30
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #2
	movs	r0, #17
	bl 0x02009c64
	movs	r0, #10
	bl 0x02009bdc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #220
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r3, #0]
	movs	r0, #17
	movs	r2, #10
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #3
	movs	r0, #30
	bl 0x02009c4c
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #29
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #30
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #31
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #32
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #33
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #34
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #35
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r1, #128
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009c04
	movs	r0, #30
	movs	r1, #1
	bl 0x02009cb4
	bl 0x02009ccc
	cmp	r5, #21
	.2byte 0xd170
	movs	r0, #30
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #31
	movs	r1, #29
	bl 0x02009d14
	movs	r0, #32
	movs	r1, #29
	bl 0x02009d14
	movs	r0, #34
	movs	r1, #29
	bl 0x02009d14
	ldr	r1, [pc, #160]
	movs	r0, #17
	bl 0x02009c0c
	ldr	r1, [pc, #156]
	movs	r0, #29
	bl 0x02009c0c
	ldr	r1, [pc, #152]
	movs	r0, #33
	bl 0x02009c0c
	movs	r0, #80
	bl 0x02009bdc
	movs	r0, #29
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #30
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #31
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #32
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #33
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #34
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #35
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #17
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #35
	movs	r1, #29
	bl 0x02009d14
	ldr	r1, [pc, #68]
	movs	r0, #30
	bl 0x02009c0c
	movs	r0, #170
	bl 0x02009bdc
	ldr	r1, [pc, #60]
	movs	r0, #32
	bl 0x02009c0c
	movs	r0, #10
	bl 0x02009bdc
	ldr	r1, [pc, #48]
	movs	r0, #31
	bl 0x02009c0c
	ldr	r1, [pc, #44]
	movs	r0, #34
	bl 0x02009c0c
	movs	r0, #60
	bl 0x02009bdc
	ldr	r1, [pc, #36]
	movs	r0, #35
	bl 0x02009c0c
	.2byte 0xe076
	.2byte 0xb434
	.2byte 0x0200
	push	{r4, r6, lr}
	lsls	r0, r0, #8
	push	{r3, r5, r6, r7, lr}
	lsls	r0, r0, #8
	push	{r2, r3, r6, r7}
	lsls	r0, r0, #8
	.2byte 0xb74c
	lsls	r0, r0, #8
	.2byte 0xb66c
	lsls	r0, r0, #8
	.2byte 0xb6c8
	lsls	r0, r0, #8
	.2byte 0xb7f8
	lsls	r0, r0, #8
.L_020015e0:
	movs	r0, #30
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #30
	movs	r1, #17
	bl 0x02009d14
	movs	r0, #31
	movs	r1, #30
	bl 0x02009d14
	movs	r0, #32
	movs	r1, #30
	bl 0x02009d14
	movs	r0, #34
	movs	r1, #30
	bl 0x02009d14
	ldr	r1, [pc, #808]
	movs	r0, #17
	bl 0x02009c0c
	movs	r0, #60
	bl 0x02009bdc
	ldr	r1, [pc, #800]
	movs	r0, #29
	bl 0x02009c0c
	ldr	r1, [pc, #796]
	movs	r0, #33
	bl 0x02009c0c
	movs	r0, #70
	bl 0x02009bdc
	movs	r0, #35
	movs	r1, #29
	bl 0x02009d14
	ldr	r1, [pc, #776]
	movs	r0, #30
	bl 0x02009c0c
	movs	r0, #29
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #30
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #31
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #32
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #33
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #34
	movs	r1, #3
	bl 0x02009c9c
	movs	r0, #35
	movs	r1, #3
	bl 0x02009c9c
	movs	r1, #3
	movs	r0, #17
	bl 0x02009c9c
	movs	r0, #120
	bl 0x02009bdc
	ldr	r1, [pc, #704]
	movs	r0, #34
	bl 0x02009c0c
	movs	r0, #40
	bl 0x02009bdc
	ldr	r1, [pc, #692]
	movs	r0, #31
	bl 0x02009c0c
	ldr	r1, [pc, #688]
	movs	r0, #35
	bl 0x02009c0c
	movs	r0, #30
	bl 0x02009bdc
	ldr	r1, [pc, #680]
	movs	r0, #32
	bl 0x02009c0c
.L_020016ae:
	movs	r0, #150
	lsls	r0, r0, #1
	bl 0x02009bdc
	movs	r0, #34
	bl 0x02009c14
	movs	r0, #50
	bl 0x02009bdc
	movs	r0, #248
	movs	r1, #1
	movs	r2, #166
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x02009cc4
	bl 0x02009ccc
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #17
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #17
	bl 0x02009c4c
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #30
	bl 0x02009c4c
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #16
	movs	r0, #17
	negs	r1, r1
	movs	r2, #0
	bl 0x02009d0c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #17
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #128
	movs	r0, #29
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	movs	r0, #31
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #128
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
	movs	r2, #0
	movs	r1, #0
	movs	r0, #34
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #30
	movs	r1, #3
	bl 0x02009c44
	movs	r0, #29
	movs	r1, #3
	bl 0x02009c44
	movs	r0, #33
	movs	r1, #3
	bl 0x02009c44
	movs	r0, #31
	movs	r1, #3
	bl 0x02009c44
	movs	r0, #32
	movs	r1, #3
	bl 0x02009c44
	movs	r0, #35
	movs	r1, #3
	bl 0x02009c44
	movs	r1, #3
	movs	r0, #34
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #192
	movs	r0, #29
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #31
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #30
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #33
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #192
	movs	r0, #34
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c94
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #29
	adds	r1, #102
	adds	r2, #51
	bl 0x02009c04
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #30
	adds	r1, #102
	adds	r2, #51
	bl 0x02009c04
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #31
	adds	r1, #102
	adds	r2, #51
	bl 0x02009c04
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #32
	adds	r1, #102
	adds	r2, #51
	bl 0x02009c04
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #33
	adds	r1, #102
	adds	r2, #51
	bl 0x02009c04
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #34
	adds	r1, #102
	adds	r2, #51
	bl 0x02009c04
	movs	r1, #243
	movs	r2, #243
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #35
	adds	r1, #102
	adds	r2, #51
	bl 0x02009c04
	movs	r2, #150
	movs	r0, #29
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #150
	movs	r0, #30
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #150
	movs	r0, #31
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #150
	movs	r0, #32
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #150
	movs	r0, #33
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #150
	movs	r0, #35
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d04
	movs	r2, #150
	movs	r0, #34
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d0c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
	movs	r0, #31
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
	movs	r0, #32
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #35
	bl 0x02009c3c
	movs	r0, #15
	bl 0x02009bdc
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x02009cb4
	bl 0x02009ccc
	bl 0x02009bec
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200b890
	.4byte 0x0200b970
	.4byte 0x0200b9cc
	.4byte 0x0200b914
	.4byte 0x0200ba28
	.4byte 0x0200bb08
	.4byte 0x0200bb78
	.4byte 0x0200ba98
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #180
	bl 0x02009bbc
	bl 0x02009be4
	movs	r0, #0
	bl 0x02009cfc
	ldr	r0, [pc, #568]
	bl 0x02009c74
	cmp	r6, #1
	bne.n	.L_0200199e
	movs	r0, #10
	bl 0x02009bdc
	ldr	r5, [pc, #556]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #184
	ldr	r0, [r5, #0]
	movs	r1, #168
	lsls	r2, r2, #2
	bl 0x02009c24
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c94
.L_0200199e:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x02009ca4
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #3
	movs	r0, #25
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x02009c84
	movs	r0, #25
	movs	r1, #4
	bl 0x02009c4c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #2
	movs	r0, #25
	bl 0x02009c64
	movs	r0, #20
	bl 0x02009bdc
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	cmp	r6, #1
	beq.n	.L_020019fc
	b.n	.L_02001b56
.L_020019fc:
	movs	r1, #8
	movs	r2, #0
	negs	r1, r1
	movs	r0, #25
	bl 0x02009d0c
	movs	r0, #15
	bl 0x02009bdc
	movs	r0, #25
	movs	r1, #3
	bl 0x02009c5c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x02009cac
	movs	r0, #40
	bl 0x02009bdc
	movs	r0, #25
	bl 0x02009bfc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #184
	strb	r3, [r0, #0]
	movs	r1, #192
	lsls	r2, r2, #2
	movs	r0, #25
	bl 0x02009c24
	movs	r0, #1
	bl 0x02009bdc
	movs	r0, #25
	bl 0x02009bfc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x02009bdc
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x02009ca4
	movs	r1, #0
	movs	r0, #25
	bl 0x02009c7c
	movs	r0, #4
	movs	r1, #0
	bl 0x02009bf4
	cmp	r0, #0
	bne.n	.L_02001aba
	movs	r0, #30
	bl 0x02009bdc
	movs	r1, #2
	movs	r0, #25
	bl 0x02009c64
	movs	r0, #20
	bl 0x02009bdc
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x02009c84
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001adc
.L_02001aba:
	movs	r0, #30
	bl 0x02009bdc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #25
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
.L_02001adc:
	movs	r0, #4
	bl 0x02009bfc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #188
	strb	r3, [r0, #0]
	movs	r1, #152
	lsls	r2, r2, #2
	movs	r0, #4
	bl 0x02009c24
	movs	r0, #1
	bl 0x02009bdc
	movs	r0, #4
	bl 0x02009bfc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #224
	strb	r3, [r0, #0]
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x02009c94
	movs	r0, #20
	bl 0x02009bdc
	movs	r1, #3
	movs	r0, #25
	bl 0x02009c4c
	movs	r0, #10
	bl 0x02009bdc
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x02009c94
	movs	r0, #70
	bl 0x02009bdc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #25
	bl 0x02009ca4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c84
.L_02001b56:
	movs	r2, #182
	movs	r0, #25
	movs	r1, #184
	lsls	r2, r2, #2
	bl 0x02009c24
	movs	r2, #110
	movs	r0, #25
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d0c
	cmp	r6, #0
	bne.n	.L_02001b9a
	movs	r0, #50
	bl 0x02009bdc
	movs	r1, #132
	movs	r2, #162
	movs	r0, #25
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c3c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #32
	bl 0x02009d0c
	movs	r0, #25
	movs	r1, #150
	movs	r2, #0
	bl 0x02009d0c
.L_02001b9a:
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c3c
	bl 0x02009bec
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001afa
	.4byte 0x02000240
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x08020171, 0x080201e9, 0x080ad041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80d9, 0x080c80e1, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c83e1, 0x080c8409, 0x080c84e1, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8681, 0x080c8701, 0x080c8709, 0x080c8779, 0x080c8879, 0x08108009, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x0200001a
	.4byte 0x0201001b
	.4byte 0x0202001c
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
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
	.4byte 0x0000005f
	.4byte 0x10101060
	.4byte 0xffffffff
	.4byte 0x10202060
	.4byte 0xffffffff
	.4byte 0x10505060
	.4byte 0xffffffff
	.4byte 0x10606060
	.4byte 0xffffffff
	.4byte 0x10707060
	.4byte 0xffffffff
	.4byte 0x10808065
	.4byte 0xffffffff
	.4byte 0x10914002
	.4byte 0xffffffff
	.4byte 0x10b01062
	.4byte 0xffffffff
	.4byte 0x10c01064
	.4byte 0xffffffff
	.4byte 0x10e01061
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00002000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0001c000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x0001c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x0001c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0001a000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00002000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00012000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00010000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00010000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0xffff00e1
	.4byte 0x00000002
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x02e00000
	.4byte 0x0002e000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00014000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00003000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0001e000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00004000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x0000c000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00004000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0000a000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00018000
	.4byte 0xffff00ba
	.4byte 0x00000002
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00010000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x0000e000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00010000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x0001e000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0000c000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00004000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0001c000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00010000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0000e000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x0001c000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00004000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0000a000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00018000
	.4byte 0xffff00ba
	.4byte 0x00000002
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00010000
	.4byte 0xffff00d7
	.4byte 0x00000002
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00004000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x0000e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x0000e000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001c000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0000c000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0000d000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00003000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00003000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00010000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00015000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00010000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x02009d64
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x0001c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x003fffff
	.4byte 0x00010002
	.4byte 0x00060002
	.4byte 0x003effff
	.4byte 0x00010002
	.4byte 0x00060002
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200a762
	.4byte 0x0025001b
	.4byte 0x0200a762
	.4byte 0x002b001b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200a762
	.4byte 0x00240015
	.4byte 0x0200a762
	.4byte 0x002e0026
	.4byte 0x0200a760
	.4byte 0x00000000
	.4byte 0x0200a76e
	.4byte 0x0021000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200a76e
	.4byte 0x00090005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0200a762
	.4byte 0x00090010
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200810d
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008761
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008775
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x02008125
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte 0x02008131
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte 0x02008255
	.4byte 0x00000002
	.4byte 0xffff0013
	.4byte 0x02008261
	.4byte 0x00000002
	.4byte 0x08b30014
	.4byte 0x02008aa1
	.4byte 0x00000002
	.4byte 0x08b30015
	.4byte 0x02008aa1
	.4byte 0x00000002
	.4byte 0x08b40016
	.4byte 0x02008249
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001b14
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001b15
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001b16
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001b17
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001abf
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001ac0
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200813d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001ac5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001ac6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001ac7
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001ac8
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001ac9
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001ae7
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001ae8
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001ae9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001aea
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001aeb
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020081a5
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001aef
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001af0
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001af1
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001af2
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001af3
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001af4
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001af5
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001af6
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001af7
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001af8
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001af9
	.4byte 0x00008d15
	.4byte 0xffff0419
	.4byte 0x0200823d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001b47
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001b48
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001b49
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001b4a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001b4b
	.4byte 0x00001815
	.4byte 0x0200001a
	.4byte 0x0200826d
	.4byte 0x00001815
	.4byte 0x0201001b
	.4byte 0x0200826d
	.4byte 0x00001815
	.4byte 0x0202001c
	.4byte 0x0200826d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200810d
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008761
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008775
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x02008125
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte 0x02008131
	.4byte 0x00000002
	.4byte 0x08b70019
	.4byte 0x0200847d
	.4byte 0x00000000
	.4byte 0x18ff0012
	.4byte 0x00001ceb
	.4byte 0x00008d15
	.4byte 0x18ff0012
	.4byte 0x00001cec
	.4byte 0x00000000
	.4byte 0x18ff0011
	.4byte 0x00001ced
	.4byte 0x00008d15
	.4byte 0x18ff0011
	.4byte 0x00001cee
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001c59
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c5a
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001c5b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001c5c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001c15
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001c16
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001c17
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c18
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001c19
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001c1a
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001c1b
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001c1c
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001c1d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001c1e
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001c1f
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008289
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001c23
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001c24
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001c25
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001c26
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001c27
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001c28
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001c29
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001c2a
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001c2b
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001c2c
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001c2d
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001c2e
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001c3c
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001c3d
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x02008399
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001c3f
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001c40
	.4byte 0x00008d15
	.4byte 0x08b5001b
	.4byte 0x00001c41
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001c4a
	.4byte 0x0001c914
	.4byte 0xffff001b
	.4byte 0x020082f9
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001c4b
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00001c52
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00001c54
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x00001c53
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001c55
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00001c56
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00001c58
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x00001c57
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c9f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001ca0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001ca1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001ca2
	.4byte 0x00000000
	.4byte 0x08b6000c
	.4byte 0x00001ca3
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200866d
	.4byte 0x00008d15
	.4byte 0x08b9000c
	.4byte 0x00001ca4
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001caa
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200810d
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008761
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008775
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x02008125
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte 0x02008131
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025a2
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025a3
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025a4
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025a5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002570
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002571
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002572
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002573
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002574
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002575
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002576
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002577
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002578
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002579
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000257a
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000257b
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000257c
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000257d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000257e
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000257f
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002580
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002587
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002581
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002582
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002583
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002584
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002585
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002586
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002594
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002595
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002596
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00002597
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00002598
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00002599
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x0000259a
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x0000259b
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x0000259c
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0000259d
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x0000259e
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x0000259f
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x000025a0
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x000025a1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025bc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000025bd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025be
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000025bf
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025c0
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025c1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte 0x0200810d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x0200810d
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008761
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008775
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002512
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002513
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002514
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002515
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002516
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002517
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002518
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002519
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000251a
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000251b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000251c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000251d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0000251e
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000251f
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002520
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002521
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002522
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002524
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002525
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002526
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002527
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002528
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002529
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000252a
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000252b
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000252c
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000252d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000252e
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000252f
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002530
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002531
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002532
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002533
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002534
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xfff00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xfff00000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0xffec0000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
