.syntax unified
	.thumb
	.global Func_081082c4
	.thumb_func
Func_081082c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r1, #0
	movs r1, #0
	mov r8, r0
	sub sp, #4
	mov r10, r1
	bl Func_0810a66c
	cmp r8, r0
	bge .L_081082e6
	mov r2, r8
	cmp r2, #0
	bge .L_081082ea
.L_081082e6:
	movs r3, #0
	mov r8, r3
.L_081082ea:
	mov r0, r8
	bl Func_0810a670
	bl Func_08108148
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	mov r0, r8
	ldr r7, [r3]
	bl Func_0810a6f8
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #6
	adds r3, r7, r1
	strb r0, [r3]
	adds r0, r5, #0
	bl Object_GetByIdFar
	ldr r3, [r0, #80]
	movs r1, #128
	ldr r3, [r3, #40]
	lsls r1, r1, #3
	ldrh r2, [r3]
	adds r1, #250
	adds r3, r7, r1
	strh r2, [r3]
	movs r1, #0
	ldrh r0, [r3]
	movs r2, #0
	movs r3, #0
	bl Func_080380f8
	mov r9, r0
	cmp r0, #0
	bne .L_08108344
	movs r3, #2
	movs r0, #30
	movs r1, #0
	movs r2, #2
	str r3, [sp, #0]
	bl UiWindow_CreateFar
	mov r9, r0
.L_08108344:
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #236
	adds r3, r7, r2
	movs r1, #128
	ldrh r0, [r3]
	lsls r1, r1, #23
	movs r3, #0
	mov r2, r9
	str r3, [sp, #0]
	bl RenderOutput_CreateFar
	movs r3, #255
	adds r5, r0, #0
	strb r3, [r5, #15]
	movs r3, #1
	strb r3, [r5, #5]
	ldr r3, .L_08108388
	movs r1, #32
	strb r3, [r5, #4]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	adds r6, r7, r3
	adds r0, r6, #0
	negs r1, r1
	movs r2, #112
	bl Func_08108aa8
	str r5, [r6]
	ldr r0, .L_0810838c
	bl Func_081084f4
	b .L_08108390
.L_08108388:
	.4byte 0x00000000
.L_0810838c:
	.4byte 0x0000124c
.L_08108390:
	mov r0, r10
	bl Menu_SelectEntry11To14Far
	movs r1, #129
	lsls r1, r1, #3
	mov r10, r0
	adds r1, #255
	adds r3, r7, r1
	mov r2, r10
	strb r2, [r3]
	mov r3, r10
	cmp r3, #0
	bne .L_081083c4
	movs r2, #156
	lsls r2, r2, #2
	adds r1, r7, r2
	mov r0, r8
	bl Func_0810a6b8
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #2
	adds r3, r7, r1
	strh r0, [r3]
	ldr r0, .L_08108448
	b .L_081083e6
.L_081083c4:
	mov r2, r10
	cmp r2, #1
	bne .L_081083d6
	ldr r0, .L_0810844c
	bl Func_081084f4
	bl Func_08109ad8
	b .L_0810840e
.L_081083d6:
	mov r3, r10
	cmp r3, #2
	bne .L_081083fe
	bl Func_081080a8
	cmp r0, #0
	beq .L_081083f0
	ldr r0, .L_08108450
.L_081083e6:
	bl Func_081084f4
	bl Func_08108b70
	b .L_0810840e
.L_081083f0:
	ldr r0, .L_08108454
	bl Func_081084f4
	movs r0, #1
	bl WaitFrames
	b .L_0810840e
.L_081083fe:
	mov r1, r10
	cmp r1, #3
	bne .L_08108428
	ldr r0, .L_08108458
	bl Func_081084f4
	bl Func_0810a2c8
.L_0810840e:
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #220
	movs r1, #32
	adds r0, r7, r2
	negs r1, r1
	movs r2, #112
	bl Func_08108aa8
	ldr r0, .L_0810845c
	bl Func_081084f4
	b .L_08108390
.L_08108428:
	ldr r0, .L_08108460
	bl Func_081084f4
	mov r0, r9
	movs r1, #2
	bl UiWork_FinalizeFar
	bl Func_0810824c
	movs r0, #0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08108448:
	.4byte 0x00001258
.L_0810844c:
	.4byte 0x0000125a
.L_08108450:
	.4byte 0x00001269
.L_08108454:
	.4byte 0x00001268
.L_08108458:
	.4byte 0x0000126a
.L_0810845c:
	.4byte 0x00001255
.L_08108460:
	.4byte 0x00001256
