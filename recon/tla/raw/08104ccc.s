.syntax unified
	.thumb
	.global Func_08104ccc
	.thumb_func
Func_08104ccc:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r5, r1, #0
	mov r12, r3
	cmp r0, #0
	bge .L_08104ce0
	adds r0, #3
.L_08104ce0:
	asrs r3, r0, #2
	lsls r0, r3, #2
	movs r3, #188
	lsls r3, r3, #1
	add r3, r12
	ldr r2, [r3]
	movs r4, #13
	movs r1, #0
	strb r4, [r2, #5]
	cmp r0, #0
	beq .L_08104cfc
	movs r3, #17
	strb r3, [r2, #5]
	strh r1, [r2, #12]
.L_08104cfc:
	movs r3, #190
	lsls r3, r3, #1
	add r3, r12
	ldr r2, [r3]
	adds r3, r0, #4
	strb r4, [r2, #5]
	cmp r3, r5
	bge .L_08104d12
	movs r3, #15
	strb r3, [r2, #5]
	strh r1, [r2, #12]
.L_08104d12:
	pop {r5, pc}
