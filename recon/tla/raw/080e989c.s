.syntax unified
	.thumb
	.global Func_080e989c
	.thumb_func
Func_080e989c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r1, #0
	adds r0, #12
	movs r1, #24
	bl Math_Mod
	adds r5, r0, #0
	ldr r2, .L_080e993c
	lsls r5, r5, #18
	asrs r5, r5, #16
	adds r0, r5, #0
	movs r1, #96
	mov r8, r2
	bl Math_Mod
	lsls r0, r0, #16
	mov r2, r8
	asrs r0, r0, #16
	ldrb r3, [r2, r0]
	subs r6, #7
	lsls r6, r6, #16
	asrs r6, r6, #16
	adds r3, r3, r6
	adds r0, r5, #0
	lsls r3, r3, #16
	asrs r3, r3, #16
	movs r1, #96
	adds r0, #32
	mov r10, r3
	bl Math_Mod
	mov r2, r8
	ldrb r3, [r2, r0]
	adds r5, #64
	adds r3, r3, r6
	lsls r3, r3, #16
	adds r0, r5, #0
	movs r1, #96
	asrs r7, r3, #16
	bl Math_Mod
	mov r2, r8
	ldrb r3, [r2, r0]
	adds r3, r3, r6
	lsls r3, r3, #16
	asrs r0, r3, #16
	mov r3, r10
	cmp r3, #0
	bge .L_080e9908
	movs r2, #0
	mov r10, r2
.L_080e9908:
	mov r3, r10
	cmp r3, #31
	ble .L_080e9912
	movs r2, #31
	mov r10, r2
.L_080e9912:
	cmp r7, #0
	bge .L_080e9918
	movs r7, #0
.L_080e9918:
	cmp r7, #31
	ble .L_080e991e
	movs r7, #31
.L_080e991e:
	cmp r0, #0
	bge .L_080e9924
	movs r0, #0
.L_080e9924:
	cmp r0, #31
	ble .L_080e992a
	movs r0, #31
.L_080e992a:
	lsls r3, r7, #5
	lsls r0, r0, #10
	orrs r0, r3
	mov r3, r10
	orrs r0, r3
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080e993c:
	.4byte Data_080f0fe0
