.syntax unified
	.thumb
	.global Func_0811d2f8
	.thumb_func
Func_0811d2f8:
	push {r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r1, r9
	sub sp, #4
	mov r8, r1
	mov r3, sp
	mov r7, r8
	str r1, [r3]
	subs r7, #4
	ldr r0, [r7]
	bl Party_Check
	movs r2, #1
	movs r5, #150
	negs r2, r2
	movs r6, #0
	lsls r5, r5, #1
	cmp r0, r2
	bne .L_0811d34e
	b .L_0811d400
.L_0811d324:
	ldr r3, .L_0811d40c
	ldrh r3, [r3]
	cmp r3, #20
	bhi .L_0811d3f8
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_0811d3f8
	ldr r3, .L_0811d410
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0811d34c
	adds r6, #1
	cmp r6, #24
	ble .L_0811d34e
	b .L_0811d3f8
.L_0811d34c:
	movs r6, #0
.L_0811d34e:
	bl Func_0801692c
	cmp r0, #0
	bne .L_0811d324
	ldr r3, .L_0811d40c
	ldrh r3, [r3]
	cmp r3, #20
	bne .L_0811d3f8
	movs r3, #16
	negs r3, r3
	add r3, r8
	mov r9, r3
	ldr r3, [r7]
	mov r1, r9
	ldr r2, [r3]
	str r2, [r1]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0811d3fe
	mov r3, r8
	mov r2, r8
	subs r3, #20
	subs r2, #12
	ldr r3, [r3]
	ldr r0, [r2]
	lsls r3, r3, #4
	adds r0, r0, r3
	bl Party_Check
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_0811d3d2
	b .L_0811d400
.L_0811d392:
	ldr r3, .L_0811d40c
	movs r1, #20
	ldrh r3, [r3]
	mov r8, r3
	mov r3, r9
	ldr r0, [r3]
	lsls r0, r0, #4
	adds r0, #19
	bl Math_DivU
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	cmp r8, r3
	bhi .L_0811d3f8
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_0811d3f8
	ldr r3, .L_0811d410
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0811d3d0
	adds r6, #1
	cmp r6, #24
	ble .L_0811d3d2
	b .L_0811d3f8
.L_0811d3d0:
	movs r6, #0
.L_0811d3d2:
	bl Func_0801692c
	cmp r0, #0
	bne .L_0811d392
	mov r1, r9
	ldr r3, .L_0811d40c
	ldr r0, [r1]
	ldrh r3, [r3]
	lsls r0, r0, #4
	adds r0, #19
	movs r1, #20
	mov r8, r3
	bl Math_DivU
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	cmp r8, r3
	beq .L_0811d3fe
.L_0811d3f8:
	movs r0, #1
	negs r0, r0
	b .L_0811d400
.L_0811d3fe:
	movs r0, #0
.L_0811d400:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r9, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811d40c:
	.4byte Data_02005354
.L_0811d410:
	.4byte gLinkStatus
