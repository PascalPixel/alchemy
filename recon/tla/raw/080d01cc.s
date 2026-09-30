.syntax unified
	.thumb
	.global Func_080d01cc
	.thumb_func
Func_080d01cc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #32]
	asrs r2, r0, #8
	movs r3, #255
	adds r6, r3, #0
	ands r2, r3
	mov r10, r1
	ands r6, r0
	cmp r2, #4
	bls .L_080d01ee
	b .L_080d04d2
.L_080d01ee:
	lsls r3, r2, #2
	ldr r2, .L_080d0278
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080d01f8:
	.4byte .L_080d020c
	.4byte .L_080d0220
	.4byte .L_080d0284
	.4byte .L_080d036c
	.4byte .L_080d0408
.L_080d020c:
	movs r0, #0
	bl Func_08013e70
	mov r0, r10
	bl Func_08013eb4
	movs r0, #1
	bl WaitFrames
	b .L_080d04d2
.L_080d0220:
	movs r3, #160
	lsls r3, r3, #19
	movs r0, #128
	ldrh r1, [r3]
	lsls r0, r0, #8
	bl Func_080d172c
	mov r0, r10
	bl Func_080d17ac
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080d027c
	ldr r4, .L_080d0280
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_080d026c
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_080d026c:
	strh r5, [r4]
	movs r0, #0
	bl Func_080d174c
	b .L_080d0506
	.2byte 0x0000
.L_080d0278:
	.4byte .L_080d01f8
.L_080d027c:
	.4byte Data_020038e0
.L_080d0280:
	.4byte 0x04000208
.L_080d0284:
	bl Func_080d019c
	movs r1, #0
	adds r5, r0, #0
	movs r2, #160
	movs r0, #165
	mov r9, r1
	lsls r0, r0, #3
	lsls r2, r2, #3
	movs r1, #160
	adds r3, r5, r0
	adds r2, #42
	lsls r1, r1, #3
	strh r6, [r3]
	mov r0, r9
	adds r3, r5, r2
	adds r1, #52
	strh r0, [r3]
	adds r2, r5, r1
	movs r3, #63
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #74
	ldrh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	ands r3, r2
	ldr r1, .L_080d02f8
	ldr r2, .L_080d02fc
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #54
	orrs r3, r1
	mov r8, r2
	movs r1, #144
	adds r2, r5, r0
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d0300
	bl Func_080145a8
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #118
	ldr r0, .L_080d0304
	bl Func_080145a8
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080d0308
	ldr r4, .L_080d030c
	ldrh r3, [r4]
	adds r6, r3, #0
	strh r4, [r4]
	b .L_080d0310
	.2byte 0x0000
.L_080d02f8:
	.4byte 0x00000001
.L_080d02fc:
	.4byte 0x00000000
.L_080d0300:
	.4byte Func_080cf78c
.L_080d0304:
	.4byte Func_080cf6fc
.L_080d0308:
	.4byte Data_020038e0
.L_080d030c:
	.4byte 0x04000208
.L_080d0310:
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_080d0338
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_080d0338:
	strh r6, [r4]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #58
	adds r3, r5, r1
	mov r2, r9
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #59
	adds r2, r5, r3
	movs r0, #160
	movs r3, #32
	strb r3, [r2]
	lsls r0, r0, #3
	movs r2, #160
	adds r0, #60
	lsls r2, r2, #3
	adds r3, r5, r0
	mov r1, r10
	adds r2, #61
	strb r1, [r3]
	mov r0, r8
	adds r3, r5, r2
	strb r0, [r3]
	b .L_080d0506
.L_080d036c:
	bl Func_080d019c
	movs r1, #165
	movs r2, #160
	adds r5, r0, #0
	lsls r1, r1, #3
	movs r0, #32
	lsls r2, r2, #3
	adds r2, #42
	adds r3, r5, r1
	mov r8, r0
	mov r1, r8
	strh r6, [r3]
	adds r3, r5, r2
	strh r1, [r3]
	movs r0, #15
	bl Func_080d0b7c
	movs r0, #1
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d03fc
	bl Func_080145a8
	ldr r1, .L_080d0400
	ldr r4, .L_080d0404
	ldrh r3, [r4]
	adds r6, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_080d03d2
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_080d03d2:
	strh r6, [r4]
	movs r2, #160
	lsls r2, r2, #3
	movs r0, #160
	adds r2, #58
	lsls r0, r0, #3
	adds r3, r5, r2
	adds r0, #59
	movs r2, #0
	strb r2, [r3]
	mov r1, r8
	adds r3, r5, r0
	adds r0, #1
	strb r1, [r3]
	adds r3, r5, r0
	mov r1, r10
	adds r0, #1
	strb r1, [r3]
	adds r3, r5, r0
	strb r2, [r3]
	b .L_080d0506
.L_080d03fc:
	.4byte Func_080d0a28
.L_080d0400:
	.4byte Data_020038e0
.L_080d0404:
	.4byte 0x04000208
.L_080d0408:
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #32]
	bl Func_080d019c
	ldr r3, .L_080d0448
	movs r1, #130
	lsls r1, r1, #1
	adds r5, r0, #0
	ldr r0, .L_080d044c
	adds r2, r7, r1
	mov r8, r3
	adds r1, #2
	movs r3, #80
	strh r3, [r2]
	adds r2, r7, r1
	mov r9, r0
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	cmp r6, #0
	bne .L_080d048e
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d0450
	bl Func_080145a8
	ldr r2, .L_080d0454
	movs r0, #1
	movs r1, #0
	b .L_080d0458
.L_080d0448:
	.4byte 0x00000000
.L_080d044c:
	.4byte 0x00000050
.L_080d0450:
	.4byte Func_080d0788
.L_080d0454:
	.4byte Func_080d0954
.L_080d0458:
	bl Func_08013438
	movs r2, #160
	lsls r2, r2, #3
	movs r1, #160
	adds r2, #58
	lsls r1, r1, #3
	adds r3, r5, r2
	mov r0, r9
	adds r1, #59
	strb r0, [r3]
	mov r2, r8
	adds r3, r5, r1
	movs r0, #160
	strb r2, [r3]
	lsls r0, r0, #3
	movs r2, #160
	adds r0, #60
	lsls r2, r2, #3
	adds r3, r5, r0
	mov r1, r10
	adds r2, #61
	strb r1, [r3]
	mov r0, r8
	adds r3, r5, r2
	strb r0, [r3]
	b .L_080d04d2
.L_080d048e:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d0510
	bl Func_080145a8
	ldr r2, .L_080d0514
	movs r0, #1
	movs r1, #0
	bl Func_08013438
	movs r1, #160
	lsls r1, r1, #3
	movs r0, #160
	adds r1, #58
	lsls r0, r0, #3
	adds r3, r5, r1
	mov r2, r9
	adds r0, #59
	strb r2, [r3]
	mov r1, r8
	adds r3, r5, r0
	movs r2, #160
	strb r1, [r3]
	lsls r2, r2, #3
	movs r1, #160
	adds r2, #60
	lsls r1, r1, #3
	adds r3, r5, r2
	mov r0, r10
	adds r1, #61
	strb r0, [r3]
	mov r2, r8
	adds r3, r5, r1
	strb r2, [r3]
.L_080d04d2:
	ldr r1, .L_080d0518
	ldr r4, .L_080d051c
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_080d0504
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_080d0504:
	strh r5, [r4]
.L_080d0506:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080d0510:
	.4byte Func_080d085c
.L_080d0514:
	.4byte Func_080d0954
.L_080d0518:
	.4byte Data_020038e0
.L_080d051c:
	.4byte 0x04000208
