.syntax unified
	.thumb
	.global Func_080d7c04
	.thumb_func
Func_080d7c04:
	push {r5, r6, r7, lr}
	sub sp, #12
	adds r6, r0, #0
	bl Object_GetById
	adds r7, r0, #0
	cmp r7, #0
	bne .L_080d7c16
	b .L_080d7d32
.L_080d7c16:
	bl Func_080d7a78
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r0, .L_080d7d38
	ldr r5, [r3]
	bl Func_08108058
	movs r0, #173
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #174
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #175
	bl Audio_PlayCue
	movs r1, #1
	adds r0, r6, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl WaitFrames
	movs r0, #140
	bl Audio_PlayCue
	ldr r3, .L_080d7d3c
	movs r0, #40
	str r3, [r7, #108]
	bl WaitFrames
	movs r0, #153
	bl Audio_PlayCue
	movs r1, #12
	movs r2, #22
	adds r0, r6, #0
	bl ObjectMotion_Launch
	ldr r3, [r7, #8]
	mov r6, sp
	str r3, [r6]
	ldr r3, [r7, #12]
	adds r0, r6, #0
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	adds r5, #80
	str r3, [r6, #8]
	bl Func_080dc390
	adds r0, r7, #0
	bl Func_080200c8
	movs r0, #164
	bl Audio_PlayCue
	movs r7, #23
.L_080d7c9c:
	movs r1, #168
	ldr r2, [r6]
	ldr r3, [r6, #8]
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Func_080ebec8
	adds r0, r5, #0
	ldr r1, .L_080d7d40
	bl Func_080ebeb4
	adds r0, r5, #0
	movs r1, #7
	bl Func_080ebea8
	bl Random16
	lsls r1, r0, #3
	subs r1, r1, r0
	lsrs r1, r1, #16
	ldr r0, [r5]
	bl Animation_ApplyChildValuesToRecordFar
	bl Random16
	movs r1, #3
	bl Math_DivU
	movs r3, #128
	lsls r3, r3, #9
	adds r0, r0, r3
	str r0, [r5, #44]
	str r0, [r5, #40]
	subs r7, #1
	movs r0, #1
	bl WaitFrames
	adds r5, #72
	cmp r7, #0
	bge .L_080d7c9c
	movs r0, #60
	bl WaitFrames
	ldr r5, .L_080d7d44
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	ldr r0, [r5]
	bl Func_080d3838
	movs r0, #20
	bl WaitFrames
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #28
	bl Object_SetMode
	movs r0, #40
	bl WaitFrames
	movs r0, #164
	bl Audio_PlayCue
	movs r0, #100
	bl WaitFrames
	bl Func_08108060
	bl Func_080d7ab4
.L_080d7d32:
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d7d38:
	.4byte 0x0020118c
.L_080d7d3c:
	.4byte Func_080d7af8
.L_080d7d40:
	.4byte Func_080d7b04
.L_080d7d44:
	.4byte gPartyState
