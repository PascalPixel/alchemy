.syntax unified
	.thumb
	.global Func_080d0954
	.thumb_func
Func_080d0954:
	push {lr}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #6
	ldrh r3, [r3]
	adds r4, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	mov r12, r3
.L_080d0968:
	movs r0, #134
	lsls r0, r0, #1
	add r0, r12
	ldrh r3, [r0]
	cmp r3, #1
	beq .L_080d09cc
	cmp r3, #1
	bgt .L_080d097e
	cmp r3, #0
	beq .L_080d0a1a
	b .L_080d0a24
.L_080d097e:
	cmp r3, #2
	beq .L_080d09a6
	cmp r3, #3
	bne .L_080d0a24
	movs r3, #132
	lsls r3, r3, #1
	add r3, r12
	ldrh r3, [r3]
	cmp r4, r3
	bcc .L_080d0a24
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	ands r3, r2
	ldr r2, .L_080d09c8
	orrs r3, r2
	b .L_080d09c0
.L_080d09a6:
	movs r3, #133
	lsls r3, r3, #1
	add r3, r12
	ldrh r3, [r3]
	cmp r4, r3
	bcc .L_080d0a24
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	ands r3, r2
.L_080d09c0:
	strh r3, [r1]
	movs r3, #9
	strh r3, [r0]
	b .L_080d0a24
.L_080d09c8:
	.4byte 0x00000002
.L_080d09cc:
	movs r3, #132
	lsls r3, r3, #1
	add r3, r12
	ldrh r3, [r3]
	cmp r4, r3
	bcc .L_080d09f8
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	ands r3, r2
	ldr r2, .L_080d09f4
	orrs r3, r2
	strh r3, [r1]
	ldrh r3, [r0]
	adds r3, #1
	strh r3, [r0]
	b .L_080d0968
.L_080d09f4:
	.4byte 0x00000002
.L_080d09f8:
	movs r3, #133
	lsls r3, r3, #1
	add r3, r12
	ldrh r3, [r3]
	cmp r4, r3
	bcc .L_080d0a24
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	ands r3, r2
	strh r3, [r1]
	movs r3, #3
	strh r3, [r0]
	b .L_080d0968
.L_080d0a1a:
	cmp r4, #158
	bhi .L_080d0a24
	movs r3, #1
	strh r3, [r0]
	b .L_080d0968
.L_080d0a24:
	pop {pc}
	.2byte 0x0000
