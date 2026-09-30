.syntax unified
	.thumb
	.global Func_0811bf18
	.thumb_func
Func_0811bf18:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r5, [r0]
	ldr r6, [r1]
	mov r10, r2
	ldr r3, [r6, #8]
	ldr r2, [r5, #8]
	movs r1, #100
	subs r3, r3, r2
	mov r0, r10
	muls r0, r3
	mov r8, r2
	bl __divsi3
	ldr r3, [r6, #16]
	ldr r6, [r5, #16]
	add r8, r0
	subs r3, r3, r6
	mov r0, r10
	muls r0, r3
	movs r1, #100
	bl __divsi3
	ldr r3, .L_0811bf88
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r5, #72]
	adds r2, r5, #0
	movs r3, #0
	str r3, [r5, #68]
	adds r6, r6, r0
	adds r2, #90
	movs r3, #1
	strb r3, [r2]
	adds r0, r5, #0
	mov r1, r8
	movs r2, #0
	adds r3, r6, #0
	b .L_0811bf8c
	.2byte 0x0000
.L_0811bf88:
	.4byte 0x00000000
.L_0811bf8c:
	bl Object_SetPosition
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetMode
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
