.syntax unified
	.thumb
	.global Func_08145c68
	.thumb_func
Func_08145c68:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #160
	lsls r0, r0, #19
	adds r0, #64
	movs r5, #0
.L_08145c7a:
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r5, #10
	lsls r1, r3, #5
	orrs r2, r1
	orrs r2, r3
	adds r5, #1
	strh r2, [r0]
	adds r0, #2
	cmp r5, #32
	bne .L_08145c7a
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #100]
	movs r2, #16
	movs r1, #0
	mov r10, r2
	movs r2, #240
	mov r9, r3
	mov r8, r1
	ldr r3, .L_08145d08
	ldr r0, .L_08145d0c
	ldr r1, .L_08145d10
	lsls r2, r2, #7
	mov lr, r3
	.2byte 0xf800
	movs r6, #128
	lsls r6, r6, #1
	movs r7, #63
	mov r5, r9
	add r6, r9
.L_08145cba:
	bl Random16
	ands r0, r7
	strb r0, [r5]
	adds r5, #1
	cmp r5, r6
	bne .L_08145cba
	movs r6, #0
	movs r5, #0
.L_08145ccc:
	mov r3, r10
	cmp r3, #0
	bge .L_08145cd4
	adds r3, #3
.L_08145cd4:
	asrs r3, r3, #2
	add r8, r3
	movs r3, #1
	add r10, r3
	cmp r6, r8
	beq .L_08145d76
	ldr r7, .L_08145d04
.L_08145ce2:
	movs r0, #0
.L_08145ce4:
	mov r1, r9
	ldrb r3, [r1, r0]
	subs r1, r6, r3
	cmp r1, #0
	blt .L_08145d66
	cmp r1, #119
	bgt .L_08145d66
	movs r4, #7
	adds r3, r0, #0
	ands r3, r4
	adds r2, r0, #0
	cmp r0, #0
	bge .L_08145d14
	adds r2, r0, #7
	b .L_08145d14
	.2byte 0x0000
.L_08145d04:
	.4byte 0x0000001f
.L_08145d08:
	.4byte IwramCopyWords
.L_08145d0c:
	.4byte gMapCellBuffer
.L_08145d10:
	.4byte 0x06008000
.L_08145d14:
	asrs r2, r2, #3
	lsls r2, r2, #6
	adds r2, r3, r2
	adds r3, r1, #0
	ands r3, r4
	lsls r3, r3, #3
	adds r2, r2, r3
	adds r3, r1, #0
	cmp r3, #0
	bge .L_08145d2a
	adds r3, #7
.L_08145d2a:
	asrs r3, r3, #3
	lsls r3, r3, #11
	adds r3, r2, r3
	ldr r2, .L_08145dec
	movs r1, #160
	adds r4, r3, r2
	ldrb r3, [r4]
	lsls r1, r1, #19
	lsls r3, r3, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r3, #248
	lsls r2, r2, #16
	lsls r3, r3, #13
	ands r3, r2
	lsrs r1, r3, #16
	lsrs r3, r2, #21
	ands r3, r7
	lsrs r2, r2, #26
	ands r2, r7
	cmp r1, r3
	bge .L_08145d5a
	adds r1, r3, #0
.L_08145d5a:
	cmp r1, r2
	bge .L_08145d60
	adds r1, r2, #0
.L_08145d60:
	movs r3, #63
	subs r3, r3, r1
	strb r3, [r4]
.L_08145d66:
	movs r2, #128
	adds r0, #1
	lsls r2, r2, #1
	cmp r0, r2
	bne .L_08145ce4
	adds r6, #1
	cmp r6, r8
	bne .L_08145ce2
.L_08145d76:
	movs r2, #240
	ldr r3, .L_08145df0
	ldr r1, .L_08145dec
	lsls r2, r2, #7
	ldr r0, .L_08145df4
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	mov r3, r8
	cmp r3, #248
	bgt .L_08145d96
	adds r5, #1
	cmp r5, #27
	bne .L_08145ccc
.L_08145d96:
	movs r0, #160
	lsls r0, r0, #19
	adds r0, #192
	movs r5, #0
.L_08145d9e:
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r5, #10
	lsls r1, r3, #5
	orrs r2, r1
	orrs r2, r3
	adds r5, #1
	strh r2, [r0]
	adds r0, #2
	cmp r5, #32
	bne .L_08145d9e
	ldr r2, .L_08145dec
	movs r1, #240
	movs r5, #0
	lsls r1, r1, #7
.L_08145dbe:
	ldrb r3, [r2]
	adds r5, #1
	adds r3, #64
	strb r3, [r2]
	adds r2, #1
	cmp r5, r1
	bne .L_08145dbe
	movs r2, #240
	ldr r3, .L_08145df0
	ldr r1, .L_08145dec
	lsls r2, r2, #7
	ldr r0, .L_08145df4
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08145dec:
	.4byte gMapCellBuffer
.L_08145df0:
	.4byte IwramCopyWords
.L_08145df4:
	.4byte 0x06008000
