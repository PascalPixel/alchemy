.syntax unified
	.thumb
	.global Func_0816442c
	.thumb_func
Func_0816442c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r10, r1
	ldr r1, [r3, #92]
	ldr r3, .L_08164640
	sub sp, #40
	ldr r4, [r3, #4]
	ldr r3, [r3]
	add r6, sp, #24
	str r3, [sp, #16]
	str r4, [sp, #20]
	ldr r3, .L_08164644
	mov r8, r2
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #8]
	str r4, [sp, #12]
	movs r3, #0
	str r3, [r6, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r6, #4]
	cmp r0, #6
	bls .L_08164468
	b .L_08164632
.L_08164468:
	ldr r2, .L_08164648
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08164470:
	.4byte .L_0816448c
	.4byte .L_0816450e
	.4byte .L_0816454e
	.4byte .L_081645c4
	.4byte .L_081645fc
	.4byte .L_081644d0
	.4byte .L_0816458c
.L_0816448c:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #220
	adds r2, r1, r2
	str r2, [sp, #4]
	movs r5, #0
	add r7, sp, #16
.L_0816449a:
	adds r0, r5, #0
	movs r1, #3
	bl __modsi3
	lsls r0, r0, #21
	add r0, r10
	str r0, [r6]
	movs r1, #3
	adds r0, r5, #0
	bl __divsi3
	lsls r0, r0, #21
	add r0, r8
	str r0, [r6, #8]
	ldr r2, [sp, #4]
	adds r1, r6, #0
	ldmia r2!, {r0}
	adds r5, #1
	adds r3, r2, #0
	str r3, [sp, #4]
	adds r2, r7, #0
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	cmp r5, #9
	bne .L_0816449a
	b .L_08164632
.L_081644d0:
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	movs r5, #0
	add r7, sp, #16
	adds r4, r1, r3
.L_081644dc:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_081644e4
	adds r3, r5, #3
.L_081644e4:
	asrs r3, r3, #2
	lsls r2, r3, #2
	subs r2, r5, r2
	ldmia r4!, {r0}
	lsls r2, r2, #21
	lsls r3, r3, #21
	add r2, r10
	add r3, r8
	str r2, [r6]
	str r3, [r6, #8]
	adds r1, r6, #0
	adds r2, r7, #0
	movs r3, #0
	str r4, [sp, #0]
	bl Render_ApplyProjectedPlacementFar
	adds r5, #1
	ldr r4, [sp, #0]
	cmp r5, #12
	bne .L_081644dc
	b .L_08164632
.L_0816450e:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #220
	movs r5, #0
	add r7, sp, #16
	adds r4, r1, r2
.L_0816451a:
	ldr r3, .L_0816464c
	ldr r2, .L_08164650
	ldrb r3, [r3, r5]
	ldmia r4!, {r0}
	lsls r3, r3, #16
	add r3, r10
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, .L_08164654
	ldr r2, .L_08164658
	ldrb r3, [r3, r5]
	adds r1, r6, #0
	lsls r3, r3, #16
	add r3, r8
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r2, r7, #0
	movs r3, #0
	str r4, [sp, #0]
	bl Render_ApplyProjectedPlacementFar
	adds r5, #1
	ldr r4, [sp, #0]
	cmp r5, #12
	bne .L_0816451a
	b .L_08164632
.L_0816454e:
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	movs r5, #0
	add r7, sp, #16
	adds r4, r1, r3
.L_0816455a:
	ldr r3, .L_0816465c
	movs r2, #128
	ldrb r3, [r3, r5]
	lsls r2, r2, #13
	lsls r3, r3, #16
	add r3, r10
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, .L_08164660
	ldmia r4!, {r0}
	ldrb r3, [r3, r5]
	adds r1, r6, #0
	lsls r3, r3, #16
	add r3, r8
	str r3, [r6, #8]
	adds r2, r7, #0
	movs r3, #0
	str r4, [sp, #0]
	bl Render_ApplyProjectedPlacementFar
	adds r5, #1
	ldr r4, [sp, #0]
	cmp r5, #8
	bne .L_0816455a
	b .L_08164632
.L_0816458c:
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	movs r5, #0
	add r7, sp, #16
	adds r4, r1, r3
.L_08164598:
	ldr r3, .L_08164664
	ldmia r4!, {r0}
	ldrb r3, [r3, r5]
	adds r1, r6, #0
	lsls r3, r3, #16
	add r3, r10
	str r3, [r6]
	ldr r3, .L_08164668
	adds r2, r7, #0
	ldrb r3, [r3, r5]
	str r4, [sp, #0]
	lsls r3, r3, #16
	add r3, r8
	str r3, [r6, #8]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	adds r5, #1
	ldr r4, [sp, #0]
	cmp r5, #8
	bne .L_08164598
	b .L_08164632
.L_081645c4:
	movs r3, #238
	movs r2, #8
	lsls r3, r3, #7
	add r2, sp
	adds r3, #220
	movs r5, #0
	mov r9, r2
	adds r7, r1, r3
.L_081645d4:
	ldr r3, .L_0816466c
	ldmia r7!, {r0}
	ldrb r3, [r3, r5]
	adds r1, r6, #0
	lsls r3, r3, #16
	add r3, r10
	str r3, [r6]
	ldr r3, .L_08164670
	mov r2, r9
	ldrb r3, [r3, r5]
	adds r5, #1
	lsls r3, r3, #16
	add r3, r8
	str r3, [r6, #8]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	cmp r5, #8
	bne .L_081645d4
	b .L_08164632
.L_081645fc:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #220
	movs r5, #0
	add r7, sp, #16
	adds r4, r1, r2
.L_08164608:
	ldr r3, .L_08164674
	ldmia r4!, {r0}
	ldrsb r3, [r3, r5]
	adds r1, r6, #0
	lsls r3, r3, #16
	add r3, r10
	str r3, [r6]
	ldr r3, .L_08164678
	adds r2, r7, #0
	ldrb r3, [r3, r5]
	str r4, [sp, #0]
	lsls r3, r3, #16
	add r3, r8
	str r3, [r6, #8]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	adds r5, #1
	ldr r4, [sp, #0]
	cmp r5, #11
	bne .L_08164608
.L_08164632:
	add sp, #40
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08164640:
	.4byte Data_08196e5c
.L_08164644:
	.4byte Data_08196e64
.L_08164648:
	.4byte .L_08164470
.L_0816464c:
	.4byte Data_08198984
.L_08164650:
	.4byte 0xfff00000
.L_08164654:
	.4byte Data_08198990
.L_08164658:
	.4byte 0xffe00000
.L_0816465c:
	.4byte Data_0819899c
.L_08164660:
	.4byte Data_081989a4
.L_08164664:
	.4byte Data_081989ac
.L_08164668:
	.4byte Data_081989b4
.L_0816466c:
	.4byte Data_081989bc
.L_08164670:
	.4byte Data_081989c4
.L_08164674:
	.4byte Data_081989cc
.L_08164678:
	.4byte Data_081989d7
