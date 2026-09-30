.syntax unified
	.thumb
	.global Func_08041350
	.thumb_func
Func_08041350:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	mov r8, r0
	bl Func_0804121c
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	movs r0, #1
	mov r9, r3
	bl WaitFrames
	bl Func_0804128c
	movs r6, #1
	negs r6, r6
	movs r7, #0
	mov r10, r0
.L_0804137e:
	cmp r6, #0
	beq .L_080413e0
	adds r0, r7, #4
	movs r1, #4
	bl __modsi3
	adds r7, r0, #0
	mov r0, r8
	movs r1, #9
	adds r0, #9
	bl __modsi3
	mov r3, r10
	movs r1, #12
	ldrsh r2, [r3, r1]
	lsls r3, r7, #3
	subs r3, r3, r7
	adds r3, r3, r2
	lsls r3, r3, #3
	mov r2, r10
	mov r8, r0
	subs r1, r3, #4
	movs r0, #14
	ldrsh r3, [r2, r0]
	mov r0, r8
	lsls r2, r0, #4
	lsls r3, r3, #3
	adds r3, r3, r2
	adds r2, r3, #0
	movs r3, #1
	negs r3, r3
	adds r2, #12
	cmp r6, r3
	bne .L_080413d0
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r9
	bl Func_08108048
	b .L_080413de
.L_080413d0:
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r9
	movs r3, #3
	bl Func_08108040
.L_080413de:
	movs r6, #0
.L_080413e0:
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_08041594
	movs r2, #2
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080413f4
	b .L_08041500
.L_080413f4:
	ldr r3, [r5, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080414f6
	ldr r3, [r5, #4]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_080414ec
	ldr r3, [r5, #4]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080414e2
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804142a
	movs r0, #111
	subs r7, #1
	movs r6, #1
	bl Audio_PlayCue
.L_0804142a:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804143e
	movs r0, #111
	adds r7, #1
	movs r6, #1
	bl Audio_PlayCue
.L_0804143e:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08041456
	movs r0, #1
	negs r0, r0
	add r8, r0
	movs r0, #111
	movs r6, #1
	bl Audio_PlayCue
.L_08041456:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0804137e
	movs r1, #1
	movs r0, #111
	add r8, r1
	movs r6, #1
	bl Audio_PlayCue
	b .L_0804137e
.L_0804146e:
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	bl Func_08041254
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	negs r2, r2
	mov r9, r2
	cmp r6, r9
	beq .L_08041586
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r2, .L_08041598
	mov r10, r3
	lsls r3, r7, #3
	adds r3, r3, r7
	add r3, r8
	lsls r3, r3, #1
	ldrsh r5, [r2, r3]
	adds r0, r5, #0
	bl Func_080ad2e8
	cmp r0, r9
	bne .L_080414b2
	ldr r3, .L_0804159c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
.L_080414b2:
	cmp r5, #0
	bne .L_0804150c
	cmp r6, #0
	bne .L_080414c2
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	strh r6, [r3]
.L_080414c2:
	cmp r6, #1
	bne .L_080414d0
	ldr r3, .L_0804159c
	movs r2, #144
	lsls r2, r2, #2
	adds r3, r3, r2
	strh r5, [r3]
.L_080414d0:
	cmp r6, #2
	bne .L_08041542
	ldr r3, .L_0804159c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #66
	adds r3, r3, r0
	strh r5, [r3]
	b .L_08041542
.L_080414e2:
	movs r0, #112
	bl Audio_PlayCue
	movs r6, #2
	b .L_0804146e
.L_080414ec:
	movs r0, #112
	bl Audio_PlayCue
	movs r6, #1
	b .L_0804146e
.L_080414f6:
	movs r0, #112
	bl Audio_PlayCue
	movs r6, #0
	b .L_0804146e
.L_08041500:
	movs r0, #113
	movs r6, #1
	bl Audio_PlayCue
	negs r6, r6
	b .L_0804146e
.L_0804150c:
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	lsls r0, r0, #10
	ands r5, r3
	orrs r0, r5
	cmp r6, #0
	bne .L_08041524
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	strh r0, [r3]
.L_08041524:
	cmp r6, #1
	bne .L_08041532
	ldr r3, .L_0804159c
	movs r1, #144
	lsls r1, r1, #2
	adds r3, r3, r1
	strh r0, [r3]
.L_08041532:
	cmp r6, #2
	bne .L_08041542
	ldr r3, .L_0804159c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #66
	adds r3, r3, r2
	strh r0, [r3]
.L_08041542:
	movs r0, #4
	bl Owner_GetState
	movs r5, #250
	lsls r5, r5, #1
	strh r5, [r0, #58]
	movs r0, #5
	bl Owner_GetState
	strh r5, [r0, #58]
	movs r0, #6
	bl Owner_GetState
	strh r5, [r0, #58]
	movs r0, #7
	bl Owner_GetState
	strh r5, [r0, #58]
	movs r0, #4
	bl Owner_GetState
	strh r5, [r0, #54]
	movs r0, #5
	bl Owner_GetState
	strh r5, [r0, #54]
	movs r0, #6
	bl Owner_GetState
	strh r5, [r0, #54]
	movs r0, #7
	bl Owner_GetState
	strh r5, [r0, #54]
.L_08041586:
	adds r0, r6, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08041594:
	.4byte gInput
.L_08041598:
	.4byte Data_080aa170
.L_0804159c:
	.4byte gPartyState
