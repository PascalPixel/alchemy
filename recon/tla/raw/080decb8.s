.syntax unified
	.thumb
	.global Func_080decb8
	.thumb_func
Func_080decb8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	sub sp, #4
	str r2, [sp, #0]
	adds r3, #224
	ldr r3, [r3]
	movs r0, #130
	ldr r5, [r3, #16]
	ldr r7, [r5, #80]
	ldr r3, [r7, #40]
	mov r10, r3
	bl Audio_PlayCue
	adds r0, r5, #0
	movs r1, #0
	bl Object_SetMode
	movs r3, #0
	str r3, [r5, #108]
	movs r2, #1
	movs r5, #0
	movs r3, #7
	mov r11, r5
	mov r8, r2
	mov r9, r3
.L_080decfa:
	mov r3, r10
	mov r2, r9
	strb r2, [r3, #5]
	movs r6, #2
	mov r2, r8
	strb r2, [r7, #25]
	movs r0, #2
	strb r6, [r7, #26]
	bl WaitFrames
	mov r3, r8
	mov r2, r11
	strb r3, [r7, #25]
	strb r2, [r7, #26]
	movs r0, #2
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	bls .L_080decfa
	ldr r5, .L_080ded7c
	movs r3, #0
	mov r8, r3
	mov r2, r8
	mov r3, r10
	strb r6, [r7, #26]
	movs r1, #144
	movs r6, #1
	strb r2, [r3, #5]
	lsls r1, r1, #3
	adds r0, r5, #0
	strb r6, [r7, #25]
	bl Func_080145a8
	ldr r3, .L_080ded80
	movs r2, #183
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	strb r6, [r3]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [sp, #0]
	movs r2, #179
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	movs r2, #128
	lsls r2, r2, #6
	adds r2, #146
	cmp r3, r2
	bne .L_080ded6c
	bl Func_080debd8
	mov r3, r8
	strh r3, [r5]
.L_080ded6c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ded7c:
	.4byte Func_080deba4
.L_080ded80:
	.4byte gPartyState
