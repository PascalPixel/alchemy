.syntax unified
	.thumb
	.global Func_080dfb0c
	.thumb_func
Func_080dfb0c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #68
	mov r10, r0
	add r0, sp, #28
	movs r3, #0
	str r3, [r0, #4]
	ldr r3, .L_080dfb80
	mov r8, r0
	str r3, [r0, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r0, #8]
	str r3, [r0, #12]
	movs r7, #0
	add r6, sp, #16
.L_080dfb32:
	lsls r5, r7, #12
	adds r0, r5, #0
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6]
	adds r0, r5, #0
	movs r3, #0
	str r3, [r6, #4]
	bl Trig_Sin
	str r0, [r6, #8]
	mov r2, r10
	ldr r5, [r2, #8]
	ldr r1, [r2, #12]
	ldr r3, [r6]
	ldr r2, [r2, #16]
	ldr r4, [r6, #4]
	str r0, [sp, #4]
	ldr r0, .L_080dfb84
	adds r7, #1
	str r0, [sp, #8]
	mov r0, r8
	str r0, [sp, #12]
	adds r0, r5, #0
	str r4, [sp, #0]
	bl Func_080df8e0
	cmp r7, #16
	bls .L_080dfb32
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080dfb80:
	.4byte Func_080dfab4
.L_080dfb84:
	.4byte 0x01090001
