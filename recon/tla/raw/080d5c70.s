.syntax unified
	.thumb
	.global Func_080d5c70
	.thumb_func
Func_080d5c70:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	movs r1, #128
	ldr r3, .L_080d5d64
	lsls r1, r1, #2
	adds r1, #14
	adds r3, r3, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	sub sp, #8
	bl Audio_PlayCue
	movs r0, #149
	lsls r0, r0, #1
	bl Audio_PlayCue
	movs r0, #147
	bl Audio_PlayCue
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080d5d04
	ldr r3, .L_080d5d68
	movs r2, #254
	lsls r2, r2, #7
	adds r2, #255
	strh r2, [r3]
	movs r0, #128
	lsls r0, r0, #3
	movs r1, #16
	adds r0, #1
	bl Func_080d0520
	movs r3, #217
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	strh r3, [r2]
	movs r0, #16
	bl WaitFrames
	movs r7, #240
	movs r1, #240
	movs r5, #0
	lsls r1, r1, #2
	lsls r7, r7, #7
	movs r6, #30
.L_080d5ce0:
	adds r3, r7, #0
	ldr r2, .L_080d5d68
	orrs r3, r1
	orrs r3, r6
	strh r3, [r2]
	movs r0, #1
	str r1, [sp, #4]
	bl WaitFrames
	ldr r2, .L_080d5d6c
	ldr r1, [sp, #4]
	adds r5, #1
	subs r1, #64
	adds r7, r7, r2
	subs r6, #2
	cmp r5, #15
	ble .L_080d5ce0
	b .L_080d5d5c
.L_080d5d04:
	movs r3, #254
	lsls r3, r3, #7
	movs r5, #160
	adds r3, #255
	lsls r5, r5, #19
	strh r3, [r5]
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	movs r1, #16
	bl Func_080d0520
	movs r3, #217
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	strh r3, [r2]
	movs r0, #16
	bl WaitFrames
	movs r7, #240
	movs r2, #240
	mov r8, r5
	lsls r2, r2, #2
	lsls r7, r7, #7
	movs r6, #30
	movs r5, #15
.L_080d5d3a:
	adds r3, r7, #0
	orrs r3, r2
	orrs r3, r6
	mov r1, r8
	strh r3, [r1]
	movs r0, #1
	str r2, [sp, #0]
	bl WaitFrames
	ldr r3, .L_080d5d6c
	ldr r2, [sp, #0]
	subs r5, #1
	subs r2, #64
	adds r7, r7, r3
	subs r6, #2
	cmp r5, #0
	bge .L_080d5d3a
.L_080d5d5c:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080d5d64:
	.4byte gPartyState
.L_080d5d68:
	.4byte 0x050001e6
.L_080d5d6c:
	.4byte 0xfffff800
