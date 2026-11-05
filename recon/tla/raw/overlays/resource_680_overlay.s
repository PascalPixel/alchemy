.syntax unified
	.thumb
	.section .text.x02008528,"ax",%progbits
	.global Func_02000528
	.thumb_func
Func_02000528:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r1, #0
	movs r2, #20
	sub sp, #12
	adds r7, r0, #0
	mov r8, r1
	mov r10, r2
.L_0200853e:
	ldr r3, [r7, #8]
	mov r5, sp
	str r3, [r5]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	mov r2, r8
	muls r2, r3
	ldr r3, [r7, #12]
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r2, #192
	adds r1, r0, #0
	lsls r2, r2, #10
	lsls r0, r6, #2
	adds r0, r0, r6
	mov r9, r2
	add r0, r9
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	movs r0, #154
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	lsls r0, r0, #1
	bl Actor_Spawn
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020085d0
	ldr r3, .L_020085cc
	mov r1, r10
	str r3, [r6, #108]
	adds r3, r6, #0
	adds r3, #100
	strh r1, [r3]
	movs r2, #0
	adds r3, #2
	strh r2, [r3]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	mov r2, r9
	str r3, [r6, #72]
	str r2, [r6, #40]
	bl Random16Far
	ldr r5, .L_020085c8
	adds r3, r6, #0
	adds r3, #35
	strh r0, [r6, #6]
	movs r1, #2
	strb r5, [r3]
	adds r0, r6, #0
	bl Func_02000e5c
	b .L_020085d0
	.2byte 0x0000
.L_020085c8:
	.4byte 0x00000000
.L_020085cc:
	.4byte Func_02000474
.L_020085d0:
	movs r1, #1
	movs r3, #2
	add r8, r1
	negs r3, r3
	mov r2, r8
	add r10, r3
	cmp r2, #15
	ble .L_0200853e
	movs r0, #0
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
