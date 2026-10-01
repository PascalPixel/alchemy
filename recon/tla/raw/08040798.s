.syntax unified
	.thumb
	.global Func_08040798
	.thumb_func
Func_08040798:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #1
	mov r8, r0
	movs r0, #191
	sub sp, #20
	negs r1, r1
	movs r2, #3
	lsls r0, r0, #1
	str r1, [sp, #12]
	mov r11, r2
	bl GameFlag_Test
	movs r3, #0
	str r3, [sp, #4]
	adds r5, r0, #0
	bl Func_080405dc
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	movs r0, #1
	mov r10, r3
	bl WaitFrames
	bl Func_08040628
	movs r3, #16
	str r0, [sp, #16]
	negs r3, r3
	movs r0, #7
	ldr r1, [sp, #16]
	movs r2, #40
	bl Func_08044f88
	str r0, [sp, #8]
	cmp r5, #0
	beq .L_080407f8
	movs r1, #2
	str r1, [sp, #4]
	movs r0, #1
	mov r11, r0
.L_080407f8:
	ldr r3, .L_08040980
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08040804
	movs r2, #3
	add r11, r2
.L_08040804:
	ldr r3, [sp, #12]
	cmp r3, #0
	beq .L_080408b2
	mov r0, r8
	add r0, r11
	mov r1, r11
	bl Math_Mod
	movs r3, #160
	lsls r3, r3, #3
	mov r8, r0
	adds r3, #116
	add r3, r10
	mov r0, r8
	movs r7, #0
	strh r0, [r3]
	cmp r7, r11
	bge .L_0804086c
	ldr r1, .L_08040984
	movs r4, #194
	ldr r6, [sp, #4]
	lsls r4, r4, #3
	mov r9, r1
	add r4, r10
.L_08040834:
	ldmia r4!, {r5}
	movs r3, #251
	strb r3, [r5, #15]
	adds r0, r5, #0
	str r4, [sp, #0]
	bl UiIcon_PrepareObjectFar
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #116
	add r3, r10
	ldrh r3, [r3]
	ldrb r1, [r5, #14]
	movs r2, #0
	ldr r4, [sp, #0]
	cmp r7, r3
	beq .L_08040858
	movs r2, #1
.L_08040858:
	mov r3, r9
	ldrsb r0, [r6, r3]
	str r4, [sp, #0]
	bl RenderResource_LoadFrame
	adds r7, #1
	adds r6, #1
	ldr r4, [sp, #0]
	cmp r7, r11
	blt .L_08040834
.L_0804086c:
	ldr r1, [sp, #16]
	movs r0, #12
	ldrsh r3, [r1, r0]
	ldr r0, [sp, #16]
	lsls r1, r3, #3
	movs r3, #14
	ldrsh r2, [r0, r3]
	mov r0, r8
	lsls r3, r0, #1
	add r3, r8
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r2, r3, #0
	ldr r3, [sp, #12]
	movs r0, #1
	negs r0, r0
	adds r2, #16
	cmp r3, r0
	bne .L_080408a0
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r10
	bl Func_08108048
	b .L_080408ae
.L_080408a0:
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r10
	movs r3, #3
	bl Func_08108040
.L_080408ae:
	movs r1, #0
	str r1, [sp, #12]
.L_080408b2:
	ldr r0, [sp, #8]
	bl Func_08045018
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_08040988
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_080408d4
	movs r0, #112
	mov r5, r8
	bl Audio_PlayCue
	b .L_08040954
.L_080408d4:
	ldr r2, [r1, #4]
	movs r3, #10
	ands r2, r3
	cmp r2, #0
	beq .L_080408ea
	movs r5, #1
	movs r0, #113
	negs r5, r5
	bl Audio_PlayCue
	b .L_08040954
.L_080408ea:
	ldr r2, [r1, #12]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_08040906
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #1
	negs r2, r2
	movs r3, #1
	add r8, r2
	str r3, [sp, #12]
	b .L_08040804
.L_08040906:
	ldr r2, [r1, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_0804091e
	movs r0, #111
	bl Audio_PlayCue
	movs r0, #1
	add r8, r0
	str r0, [sp, #12]
	b .L_08040804
.L_0804091e:
	ldr r3, .L_08040980
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08040928
	b .L_08040804
.L_08040928:
	ldr r3, [r1, #4]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804093e
	movs r0, #112
	movs r5, #20
	bl Audio_PlayCue
	b .L_08040954
.L_0804093e:
	ldr r3, [r1, #4]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0804094c
	b .L_08040804
.L_0804094c:
	movs r0, #112
	movs r5, #21
	bl Audio_PlayCue
.L_08040954:
	ldr r0, [sp, #16]
	movs r1, #2
	bl UiWork_Finalize
	bl Func_08040614
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_0804096e
	ldr r1, [sp, #4]
	adds r5, r5, r1
.L_0804096e:
	adds r0, r5, #0
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08040980:
	.4byte Data_03001238
.L_08040984:
	.4byte Data_0805ea8f
.L_08040988:
	.4byte gInput
