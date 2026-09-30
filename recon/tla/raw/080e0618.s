.syntax unified
	.thumb
	.global Func_080e0618
	.thumb_func
Func_080e0618:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #224
	ldr r2, [r2]
	sub sp, #24
	ldr r0, [r3, #92]
	str r2, [sp, #8]
	mov r8, r0
	ldr r3, [r3, #108]
	movs r2, #132
	str r3, [sp, #4]
	movs r3, #132
	lsls r3, r3, #6
	add r3, r8
	ldr r3, [r3]
	lsls r2, r2, #6
	adds r2, #16
	mov r10, r3
	add r2, r8
	movs r3, #0
	strh r3, [r2]
	mov r9, r3
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #8
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r9, r3
	bge .L_080e070c
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #10
	add r2, r8
	mov r11, r2
.L_080e066e:
	mov r1, r11
	movs r0, #0
	ldrsh r3, [r1, r0]
	mov r0, r10
	lsls r6, r3, #3
	subs r6, r6, r3
	lsls r6, r6, #2
	ldr r3, [r0, #8]
	movs r2, #208
	lsls r2, r2, #5
	add r6, r8
	adds r7, r6, r2
	str r3, [r7]
	ldr r3, [r0, #12]
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	str r3, [r7, #8]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #128
	lsls r1, r1, #11
	lsls r5, r5, #2
	adds r5, r5, r1
	bl Random16
	adds r2, r7, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	movs r2, #0
	str r2, [r7, #12]
	bl Random16
	movs r3, #128
	lsls r0, r0, #1
	lsls r3, r3, #10
	subs r3, r3, r0
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #20]
	bl Random16
	adds r5, r0, #0
	movs r0, #192
	lsls r0, r0, #9
	adds r5, r5, r0
	bl Random16
	movs r2, #208
	lsls r2, r2, #5
	adds r2, #12
	adds r6, r6, r2
	adds r1, r0, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	movs r3, #0
	str r3, [r7, #24]
	mov r0, r11
	ldrh r3, [r0]
	movs r2, #63
	adds r3, #1
	ands r3, r2
	mov r1, r11
	strh r3, [r1]
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #8
	add r3, r8
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r2, #1
	add r9, r2
	cmp r9, r3
	blt .L_080e066e
.L_080e070c:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #12
	add r2, r8
	movs r7, #208
	movs r5, #128
	str r2, [sp, #0]
	ldr r6, .L_080e074c
	movs r1, #0
	lsls r7, r7, #5
	lsls r5, r5, #5
	movs r3, #3
	mov r9, r1
	add r7, r8
	add r5, r8
	mov r11, r3
.L_080e072c:
	ldr r1, [r7, #24]
	cmp r1, #0
	blt .L_080e0798
	movs r3, #1
	mov r2, r9
	ands r3, r2
	adds r0, r5, #0
	cmp r3, #0
	beq .L_080e0750
	mov r3, r11
	ands r1, r3
	lsls r3, r1, #1
	ldr r1, [sp, #0]
	ldrh r2, [r1]
	b .L_080e0760
	.2byte 0x0000
.L_080e074c:
	.4byte 0xfffffc00
.L_080e0750:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #14
	add r2, r8
	mov r3, r11
	ldrh r2, [r2]
	ands r1, r3
	lsls r3, r1, #1
.L_080e0760:
	adds r2, r2, r3
	ldr r3, .L_080e0790
	ands r2, r3
	ldrh r3, [r5, #8]
	ands r3, r6
	orrs r3, r2
	strh r3, [r5, #8]
	adds r1, r7, #0
	bl Func_080eb298
	adds r0, r7, #0
	movs r1, #63
	ldr r2, .L_080e0794
	bl BattleFx_IntegrateVector3
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
	cmp r3, #32
	bne .L_080e0798
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
	b .L_080e0798
.L_080e0790:
	.4byte 0x000003ff
.L_080e0794:
	.4byte 0xffffec00
.L_080e0798:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r5, #40
	adds r7, #28
	cmp r1, #63
	ble .L_080e072c
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #4
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bls .L_080e07b8
	b .L_080e0954
.L_080e07b8:
	ldr r2, .L_080e0970
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080e07c0:
	.4byte .L_080e07d4
	.4byte .L_080e082e
	.4byte .L_080e0850
	.4byte .L_080e08da
	.4byte .L_080e0924
.L_080e07d4:
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r1, [sp, #8]
	lsls r3, r5, #1
	ldr r2, [r1, #4]
	adds r3, r3, r5
	adds r2, r2, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	add r6, sp, #12
	subs r2, r2, r3
	str r2, [r6]
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r1, [sp, #8]
	lsls r3, r5, #1
	ldr r2, [r1, #8]
	adds r3, r3, r5
	adds r2, r2, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	subs r2, r2, r3
	str r2, [r6, #4]
	mov r0, r10
	ldr r3, [r1, #12]
	ldr r1, [r6]
	str r3, [r6, #8]
	bl Object_SetPositionAndResetMotionFar
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #6
	add r1, r8
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #20
	beq .L_080e082c
	b .L_080e0954
.L_080e082c:
	b .L_080e090c
.L_080e082e:
	ldr r3, [sp, #8]
	mov r0, r10
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	ldr r3, [r3, #12]
	bl Object_SetPositionAndResetMotionFar
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #6
	add r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #6
	beq .L_080e084e
	b .L_080e0954
.L_080e084e:
	b .L_080e090c
.L_080e0850:
	movs r6, #132
	lsls r6, r6, #6
	adds r6, #6
	add r6, r8
	movs r1, #0
	ldrsh r3, [r6, r1]
	cmp r3, #0
	bne .L_080e086c
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #16
	add r2, r8
	movs r3, #1
	strh r3, [r2]
.L_080e086c:
	ldr r2, [sp, #4]
	movs r0, #208
	lsls r0, r0, #4
	adds r0, #49
	add r5, sp, #12
	adds r3, r2, r0
	movs r0, #0
	ldrsb r0, [r3, r0]
	adds r1, r5, #0
	bl Func_080d92a4
	ldr r3, [r5]
	mov r1, r10
	str r3, [r1, #8]
	ldr r2, .L_080e0974
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r5, #8]
	str r3, [r1, #16]
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #18
	add r3, r8
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_080e08ba
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #4
	add r3, r8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r6]
.L_080e08ba:
	movs r1, #0
	ldrsh r3, [r6, r1]
	cmp r3, #90
	bne .L_080e0954
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #4
	add r3, r8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r6]
	b .L_080e0954
.L_080e08da:
	mov r0, r10
	movs r1, #3
	bl Object_SetMode
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #8
	movs r3, #3
	add r2, r8
	movs r1, #132
	strh r3, [r2]
	lsls r1, r1, #6
	adds r1, #6
	add r1, r8
	movs r3, #0
	ldrsh r2, [r1, r3]
	mov r0, r10
	ldr r3, [r0, #12]
	lsls r2, r2, #17
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #10
	bne .L_080e0954
.L_080e090c:
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #4
	add r3, r8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_080e0954
.L_080e0924:
	movs r2, #0
	mov r3, r10
	str r2, [r3, #24]
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #8
	add r3, r8
	strh r2, [r3]
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #6
	add r3, r8
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #30
	bne .L_080e0954
	movs r2, #132
	lsls r2, r2, #6
	movs r3, #186
	adds r2, #4
	lsls r3, r3, #2
	add r2, r8
	adds r3, #255
	strh r3, [r2]
.L_080e0954:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #6
	add r2, r8
	ldrh r3, [r2]
	add sp, #24
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e0970:
	.4byte .L_080e07c0
.L_080e0974:
	.4byte 0xfffc0000
