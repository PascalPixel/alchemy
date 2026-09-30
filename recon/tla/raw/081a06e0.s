.syntax unified
	.thumb
	.global Func_081a06e0
	.thumb_func
Func_081a06e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_081a0744
	movs r2, #0
	strb r2, [r3]
	ldr r3, .L_081a0748
	ldr r7, .L_081a074c
	strb r2, [r3]
	ldr r3, .L_081a0750
	sub sp, #8
	strb r2, [r3]
	ldr r2, .L_081a0740
	ldr r3, .L_081a0754
	strh r2, [r7]
	strb r2, [r3]
	adds r6, r0, #0
	bl Func_080144c0
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_081a0758
	bl Scheduler_AddOrUpdateCallback
	movs r3, #64
	movs r5, #128
	lsls r5, r5, #19
	strh r3, [r5]
	ldr r0, .L_081a075c
	bl Func_081a0674
	ldr r0, .L_081a0760
	bl Func_081a0674
	movs r0, #0
	bl Func_081a04d0
	movs r0, #1
	bl Func_081a04d0
	movs r2, #248
	movs r3, #128
	b .L_081a0764
	.2byte 0x0000
.L_081a0740:
	.4byte 0x00000000
.L_081a0744:
	.4byte Data_0300120c
.L_081a0748:
	.4byte Data_0300123c
.L_081a074c:
	.4byte Data_02007500
.L_081a0750:
	.4byte Data_03001110
.L_081a0754:
	.4byte Data_03001200
.L_081a0758:
	.4byte Func_081a06b4
.L_081a075c:
	.4byte 0x06007800
.L_081a0760:
	.4byte 0x0600f800
.L_081a0764:
	lsls r2, r2, #5
	lsls r3, r3, #19
	adds r2, #138
	adds r3, #12
	strh r2, [r3]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #131
	adds r3, #2
	strh r2, [r3]
	movs r3, #226
	lsls r3, r3, #5
	strh r3, [r5]
	movs r2, #160
	movs r3, #128
	lsls r2, r2, #6
	lsls r3, r3, #19
	adds r2, #68
	adds r3, #80
	strh r2, [r3]
	cmp r6, #1
	bne .L_081a0852
	bl Func_081a0a70
	movs r0, #60
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #19
	movs r0, #0
	adds r1, #82
	mov r10, r0
	mov r11, r7
	mov r9, r1
.L_081a07a8:
	ldr r2, .L_081a08ac
	mov r0, r10
	movs r1, #1
	mov r5, r10
	lsls r3, r0, #2
	ands r5, r1
	ldr r0, [r2, r3]
	adds r1, r5, #0
	movs r2, #1
	eors r1, r2
	bl Func_081a052c
	movs r3, #240
	lsls r3, r3, #4
	movs r6, #1
	adds r1, r5, #0
	mov r8, r3
.L_081a07ca:
	cmp r1, #0
	beq .L_081a07dc
	movs r0, #16
	subs r2, r0, r6
	lsls r3, r6, #8
	orrs r3, r2
	mov r2, r9
	strh r3, [r2]
	b .L_081a07e4
.L_081a07dc:
	mov r3, r8
	orrs r3, r6
	mov r0, r9
	strh r3, [r0]
.L_081a07e4:
	ldr r2, .L_081a08b0
	movs r5, #0
	movs r7, #8
.L_081a07ea:
	ldr r3, [r2]
	ands r3, r7
	cmp r3, #0
	bne .L_081a087c
	movs r0, #1
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl WaitFrames
	adds r5, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0]
	cmp r5, #3
	ble .L_081a07ea
	ldr r2, .L_081a08b4
	adds r6, #1
	add r8, r2
	cmp r6, #16
	ble .L_081a07ca
	mov r1, r11
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_081a0842
	ldr r5, .L_081a08b0
	ldr r6, .L_081a08b8
.L_081a081e:
	ldr r3, [r5, #4]
	ldr r3, [r5]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	bne .L_081a087c
	ldr r3, .L_081a08bc
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	beq .L_081a087c
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	ldrsh r3, [r6, r0]
	cmp r3, #0
	beq .L_081a081e
.L_081a0842:
	mov r1, r11
	ldrh r3, [r1]
	mov r2, r11
	subs r3, #1
	strh r3, [r2]
	movs r3, #1
	add r10, r3
	b .L_081a07a8
.L_081a0852:
	subs r0, r6, #2
	bl Func_081a0e0c
	ldr r1, .L_081a08bc
	movs r2, #120
	movs r0, #0
	ldrsh r3, [r1, r0]
	adds r2, #255
	cmp r3, r2
	bgt .L_081a0878
	adds r5, r1, #0
	mov r8, r2
.L_081a086a:
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	ble .L_081a086a
.L_081a0878:
	bl Func_081a0fb8
.L_081a087c:
	movs r3, #128
	lsls r3, r3, #19
	movs r2, #0
	adds r3, #80
	strh r2, [r3]
	movs r2, #130
	lsls r2, r2, #5
	subs r3, #80
	strh r2, [r3]
	bl Func_08014bac
	bl Func_08014b70
	ldr r2, .L_081a08c0
	movs r3, #1
	movs r0, #0
	strb r3, [r2]
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081a08ac:
	.4byte Data_081a19a8
.L_081a08b0:
	.4byte gInput
.L_081a08b4:
	.4byte 0xffffff00
.L_081a08b8:
	.4byte Data_02007500
.L_081a08bc:
	.4byte Data_02007508
.L_081a08c0:
	.4byte Data_0300120c
