.syntax unified
	.thumb
	.global Func_080dc164
	.thumb_func
Func_080dc164:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r1, #0
	ldr r1, .L_080dc1ac
	mov r8, r0
	adds r6, r2, #0
	movs r5, #0
	mov r10, r1
.L_080dc178:
	adds r0, r5, #0
	bl ObjectTable_Get
	mov r1, r10
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r5, r3
	beq .L_080dc19c
	cmp r0, #0
	beq .L_080dc19c
	cmp r0, r8
	beq .L_080dc19c
	adds r3, r0, #0
	adds r3, #91
	strb r7, [r3]
	adds r1, r6, #0
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_080dc19c:
	adds r5, #1
	cmp r5, #80
	ble .L_080dc178
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dc1ac:
	.4byte Data_020004aa
