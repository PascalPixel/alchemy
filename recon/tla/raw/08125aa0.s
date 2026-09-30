.syntax unified
	.thumb
	.global Func_08125aa0
	.thumb_func
Func_08125aa0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #128
	lsls r2, r2, #19
	sub sp, #8
	adds r2, #8
	movs r0, #6
	ldrh r3, [r2]
	add r0, sp
	mov r9, r0
	mov r1, r9
	strh r3, [r1]
	ldr r1, .L_08125af4
	mov r7, sp
	orrs r3, r1
	strh r3, [r2]
	add r3, sp, #4
	adds r2, #2
	mov r10, r3
	ldrh r3, [r2]
	mov r0, r10
	strh r3, [r0]
	orrs r3, r1
	strh r3, [r2]
	movs r3, #2
	add r3, sp
	adds r2, #2
	mov r8, r3
	ldrh r3, [r2]
	mov r0, r8
	strh r3, [r0]
	orrs r3, r1
	strh r3, [r2]
	adds r2, #2
	ldrh r3, [r2]
	movs r0, #16
	strh r3, [r7]
	orrs r3, r1
	b .L_08125af8
.L_08125af4:
	.4byte 0x00000040
.L_08125af8:
	strh r3, [r2]
	ldr r3, .L_08125b34
	adds r2, #66
	strh r3, [r2]
	bl Func_08013e70
	movs r6, #128
	lsls r6, r6, #19
	movs r5, #0
	adds r6, #76
.L_08125b0c:
	bl Random16
	bl Random16
	bl Random16
	bl Random16
	lsls r3, r5, #8
	orrs r3, r5
	strh r3, [r6]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #15
	ble .L_08125b0c
	ldr r3, .L_08125b38
	b .L_08125b3c
	.2byte 0x0000
.L_08125b34:
	.4byte 0x00003eee
.L_08125b38:
	.4byte 0x00000001
.L_08125b3c:
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #4
	bl WaitFrames
	mov r1, r9
	ldrh r3, [r1]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #8
	strh r3, [r2]
	mov r0, r10
	ldrh r3, [r0]
	adds r2, #2
	strh r3, [r2]
	mov r1, r8
	ldrh r3, [r1]
	adds r2, #2
	strh r3, [r2]
	ldrh r3, [r7]
	adds r2, #2
	movs r0, #0
	strh r3, [r2]
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
