.syntax unified
	.thumb
	.global Func_080ddfd4
	.thumb_func
Func_080ddfd4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #138
	adds r6, r1, #0
	mov r8, r2
	adds r7, r3, #0
	bl Audio_PlayCue
	movs r0, #139
	adds r1, r5, #0
	lsls r0, r0, #1
	adds r2, r6, #0
	mov r3, r8
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080de056
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	str r3, [r5, #52]
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #9]
	movs r1, #3
	bl Object_SetMode
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_080de054
	ldr r6, .L_080de050
.L_080de02a:
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #28]
	str r3, [r5, #24]
	ldrh r3, [r5, #6]
	movs r0, #1
	adds r3, r3, r6
	strh r3, [r5, #6]
	bl WaitFrames
	movs r2, #255
	ldr r3, [r5, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	ble .L_080de02a
	b .L_080de054
	.2byte 0x0000
.L_080de050:
	.4byte 0x00002000
.L_080de054:
	strh r7, [r5, #6]
.L_080de056:
	adds r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
