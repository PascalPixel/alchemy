.syntax unified
	.thumb
	.global UpdateRisingParticleBurst
	.thumb_func
UpdateRisingParticleBurst:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	ldr r5, .L_080dd740
	movs r0, #154
	bl Audio_PlayCue
	movs r2, #30
	mov r8, r2
.L_080dd67e:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	ldr r3, [r7, #24]
	movs r0, #1
	adds r3, r3, r5
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	adds r3, r3, r5
	str r3, [r7, #28]
	bl WaitFrames
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	cmp r2, #0
	bge .L_080dd67e
	movs r2, #128
	movs r3, #7
	lsls r2, r2, #9
	mov r8, r3
	mov r10, r2
.L_080dd6ba:
	movs r0, #209
	lsls r0, r0, #1
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_080dc10c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080dd71e
	ldr r1, .L_080dd744
	bl Object_SetCallback
	bl Random16
	mov r3, r10
	adds r2, r6, #0
	adds r2, #85
	str r3, [r6, #52]
	add r0, r10
	movs r3, #2
	str r0, [r6, #48]
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #61
	str r3, [r6, #72]
	bl Random16
	adds r5, r0, #0
	bl Random16
	subs r5, r5, r0
	str r5, [r6, #40]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r2, #128
	lsls r2, r2, #12
	lsls r5, r5, #3
	adds r5, r5, r2
	bl Random16
	adds r1, r5, #0
	adds r2, r0, #0
	adds r0, r6, #0
	bl Func_080db974
.L_080dd71e:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	cmp r2, #0
	bge .L_080dd6ba
	movs r0, #131
	bl Audio_PlayCue
	adds r0, r7, #0
	bl Func_080200c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dd740:
	.4byte 0xfffff800
.L_080dd744:
	.4byte Data_080f0e78
