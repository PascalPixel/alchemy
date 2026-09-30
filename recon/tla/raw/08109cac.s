.syntax unified
	.thumb
	.global Func_08109cac
	.thumb_func
Func_08109cac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r0, [sp, #24]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r11, r3
	bl Owner_GetState
	movs r1, #0
	movs r2, #1
	movs r3, #2
	str r0, [sp, #12]
	str r2, [sp, #8]
	str r3, [sp, #0]
	mov r10, r1
	movs r0, #15
	movs r1, #8
	movs r2, #15
	movs r3, #4
	bl UiWindow_CreateFar
	str r0, [sp, #16]
.L_08109ce8:
	movs r3, #2
	str r3, [sp, #0]
	movs r2, #30
	movs r3, #3
	movs r0, #0
	movs r1, #5
	bl UiWindow_CreateFar
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #3
	adds r3, #220
	add r3, r11
	ldr r2, [r3]
	movs r3, #18
	strb r3, [r2, #5]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	movs r3, #12
	add r2, r11
	strb r3, [r2]
	movs r3, #1
	mov r9, r3
.L_08109d18:
	mov r1, r9
	cmp r1, #0
	beq .L_08109dda
	movs r2, #0
	ldr r0, [sp, #24]
	mov r9, r2
	bl Item_AdjustCounterFar + 0x8
	adds r3, r0, #0
	subs r3, #1
	str r0, [sp, #8]
	cmp r10, r3
	ble .L_08109d34
	mov r10, r3
.L_08109d34:
	mov r1, r10
	lsls r3, r1, #1
	adds r7, r3, #0
	ldr r3, [sp, #12]
	adds r7, #216
	ldrh r2, [r3, r7]
	ldr r3, .L_08109d80
	mov r0, r10
	adds r1, r3, #0
	ands r1, r2
	mov r8, r1
	mov r2, r11
	movs r1, #5
	ldr r6, [r2, #36]
	bl Math_Mod
	movs r1, #5
	adds r5, r0, #0
	mov r0, r10
	bl __divsi3
	adds r2, r0, #0
	lsls r5, r5, #4
	lsls r2, r2, #4
	adds r2, #8
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_08108af0
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	movs r3, #3
	add r2, r11
	strb r3, [r2]
	movs r3, #129
	b .L_08109d84
	.2byte 0x0000
.L_08109d80:
	.4byte 0x000001ff
.L_08109d84:
	lsls r3, r3, #3
	adds r3, #255
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_08109da4
	ldr r3, [sp, #12]
	ldrh r0, [r3, r7]
	bl Shop_GetSellPrice
	mov r1, r8
	adds r2, r0, #0
	movs r3, #1
	b .L_08109db6
.L_08109da4:
	cmp r3, #3
	bne .L_08109dbe
	ldr r1, [sp, #12]
	ldrh r0, [r1, r7]
	bl Func_0810a2ac
	mov r1, r8
	adds r2, r0, #0
	movs r3, #2
.L_08109db6:
	ldr r0, [sp, #16]
	bl Func_081091cc
	b .L_08109dd0
.L_08109dbe:
	mov r0, r8
	bl Func_0810a748
	mov r1, r8
	adds r2, r0, #0
	movs r3, #3
	ldr r0, [sp, #16]
	bl Func_081091cc
.L_08109dd0:
	ldr r1, .L_08109ff0
	ldr r0, [sp, #20]
	add r1, r8
	bl Func_08109270
.L_08109dda:
	ldr r4, .L_08109ff4
	movs r3, #1
	ldr r2, [r4, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_08109de8
	b .L_08109f9a
.L_08109de8:
	ldr r6, [r4, #4]
	movs r7, #2
	ands r6, r7
	cmp r6, #0
	beq .L_08109df4
	b .L_08109f8e
.L_08109df4:
	ldr r3, [r4]
	movs r2, #4
	ands r3, r2
	mov r8, r2
	cmp r3, #0
	beq .L_08109e82
	ldr r2, [sp, #12]
	mov r1, r10
	lsls r3, r1, #1
	adds r3, #216
	movs r0, #126
	ldrh r5, [r2, r3]
	str r4, [sp, #4]
	bl Audio_PlayCue
	movs r3, #10
	movs r1, #9
	movs r2, #16
	movs r0, #0
	str r7, [sp, #0]
	bl UiWindow_CreateFar
	adds r1, r5, #0
	adds r7, r0, #0
	bl Func_080f8038
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #9
	add r3, r11
	strb r6, [r3]
	ldr r4, [sp, #4]
	mov r1, r8
	ldr r3, [r4]
	ands r3, r1
	cmp r3, #0
	beq .L_08109e50
	adds r6, r4, #0
	movs r5, #4
.L_08109e42:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6]
	ands r3, r5
	cmp r3, #0
	bne .L_08109e42
.L_08109e50:
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #9
	add r2, r11
	movs r3, #1
	strb r3, [r2]
	movs r1, #2
	adds r0, r7, #0
	bl UiWork_FinalizeFar
	bl Func_08109188
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #232
	add r3, r11
	ldr r0, [r3]
	bl RenderOutput_PrepareForRedrawFar
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	mov r9, r2
	b .L_08109d18
.L_08109e82:
	ldr r3, [r4, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08109eac
	movs r0, #111
	str r4, [sp, #4]
	bl Audio_PlayCue
	movs r3, #1
	ldr r0, [sp, #8]
	negs r3, r3
	add r10, r3
	add r0, r10
	ldr r1, [sp, #8]
	bl Math_Mod
	ldr r4, [sp, #4]
	movs r1, #1
	mov r10, r0
	mov r9, r1
.L_08109eac:
	ldr r3, [r4, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_08109ed4
	movs r0, #111
	str r4, [sp, #4]
	bl Audio_PlayCue
	ldr r0, [sp, #8]
	movs r2, #1
	add r10, r2
	add r0, r10
	ldr r1, [sp, #8]
	bl Math_Mod
	ldr r4, [sp, #4]
	movs r3, #1
	mov r10, r0
	mov r9, r3
.L_08109ed4:
	ldr r3, [r4, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08109f0a
	movs r1, #5
	negs r1, r1
	add r10, r1
	mov r2, r10
	cmp r2, #0
	bge .L_08109eee
	movs r3, #15
	add r10, r3
.L_08109eee:
	ldr r1, [sp, #8]
	cmp r10, r1
	blt .L_08109f00
.L_08109ef4:
	movs r2, #5
	ldr r3, [sp, #8]
	negs r2, r2
	add r10, r2
	cmp r10, r3
	bge .L_08109ef4
.L_08109f00:
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #1
	mov r9, r1
.L_08109f0a:
	ldr r3, .L_08109ff4
	movs r2, #128
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08109f40
	ldr r3, [sp, #8]
	movs r2, #5
	add r10, r2
	cmp r10, r3
	blt .L_08109f26
	movs r1, #15
	negs r1, r1
	add r10, r1
.L_08109f26:
	mov r2, r10
	cmp r2, #0
	bge .L_08109f36
.L_08109f2c:
	movs r3, #5
	add r10, r3
	mov r1, r10
	cmp r1, #0
	blt .L_08109f2c
.L_08109f36:
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #1
	mov r9, r2
.L_08109f40:
	movs r0, #1
	bl WaitFrames
	b .L_08109d18
.L_08109f48:
	ldr r0, [sp, #20]
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bne .L_08109fd8
	movs r3, #129
	lsls r3, r3, #3
	adds r3, #255
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_08109fa4
	ldr r0, [sp, #24]
	mov r1, r10
	bl Func_0810a03c
	movs r3, #1
	adds r2, r0, #0
	negs r3, r3
	cmp r2, r3
	beq .L_08109f86
	ldr r0, [sp, #24]
	mov r1, r10
	bl Func_0810a108
.L_08109f86:
	ldr r0, .L_08109ff8
	bl Func_081084f4
	b .L_08109fcc
.L_08109f8e:
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_08109f48
.L_08109f9a:
	movs r0, #112
	bl Audio_PlayCue
	movs r5, #0
	b .L_08109f48
.L_08109fa4:
	cmp r3, #3
	bne .L_08109fb8
	ldr r0, [sp, #24]
	mov r1, r10
	bl Func_0810a2d8
	ldr r0, .L_08109ffc
	bl Func_081084f4
	b .L_08109fcc
.L_08109fb8:
	ldr r0, [sp, #24]
	mov r1, r10
	bl Func_0810b6cc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08109fd8
	ldr r0, .L_0810a000
	bl Func_081084f4
.L_08109fcc:
	ldr r0, [sp, #24]
	bl Item_AdjustCounterFar + 0x8
	cmp r0, #0
	beq .L_08109fd8
	b .L_08109ce8
.L_08109fd8:
	ldr r0, [sp, #16]
	movs r1, #2
	bl UiWork_FinalizeFar
	adds r0, r5, #0
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08109ff0:
	.4byte 0x00000092
.L_08109ff4:
	.4byte gInput
.L_08109ff8:
	.4byte 0x0000125b
.L_08109ffc:
	.4byte 0x00001273
.L_0810a000:
	.4byte 0x000012fe
