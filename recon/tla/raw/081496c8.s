.syntax unified
	.thumb
	.global Func_081496c8
	.thumb_func
Func_081496c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	lsls r5, r5, #10
	adds r1, r5, r1
	adds r0, r1, #0
	sub sp, #128
	adds r6, r2, #0
	mov r10, r3
	bl Trig_Sin
	mov r8, r0
	mov r3, r8
	adds r6, r5, r6
	lsls r3, r3, #4
	asrs r3, r3, #15
	adds r0, r6, #0
	mov r8, r3
	bl Trig_Sin
	add r5, r10
	adds r6, r0, #0
	adds r0, r5, #0
	bl Trig_Sin
	movs r3, #0
	mov r2, sp
	lsls r6, r6, #4
	lsls r0, r0, #4
	strh r3, [r2]
	asrs r6, r6, #15
	asrs r0, r0, #15
	mov r7, r8
	mov r5, sp
	movs r4, #1
	adds r7, #16
	adds r6, #16
	adds r0, #16
	adds r5, #2
.L_0814971a:
	adds r3, r4, #0
	muls r3, r7
	cmp r3, #0
	bge .L_08149724
	adds r3, #63
.L_08149724:
	adds r2, r4, #0
	muls r2, r6
	asrs r1, r3, #6
	cmp r2, #0
	bge .L_08149730
	adds r2, #63
.L_08149730:
	adds r3, r4, #0
	muls r3, r0
	asrs r2, r2, #6
	cmp r3, #0
	bge .L_0814973c
	adds r3, #63
.L_0814973c:
	asrs r3, r3, #6
	cmp r1, #0
	bge .L_08149744
	movs r1, #0
.L_08149744:
	cmp r1, #31
	ble .L_0814974a
	movs r1, #31
.L_0814974a:
	cmp r2, #0
	bge .L_08149750
	movs r2, #0
.L_08149750:
	cmp r2, #31
	ble .L_08149756
	movs r2, #31
.L_08149756:
	cmp r3, #0
	bge .L_0814975c
	movs r3, #0
.L_0814975c:
	cmp r3, #31
	ble .L_08149762
	movs r3, #31
.L_08149762:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r1
	adds r4, #1
	strh r3, [r5]
	adds r5, #2
	cmp r4, #64
	bne .L_0814971a
	movs r0, #160
	lsls r0, r0, #19
	mov r1, sp
	ldr r3, .L_08149790
	movs r2, #128
	adds r0, #2
	mov lr, r3
	.2byte 0xf800
	add sp, #128
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08149790:
	.4byte IwramCopyWords
