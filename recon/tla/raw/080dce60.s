.syntax unified
	.thumb
	.global Func_080dce60
	.thumb_func
Func_080dce60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #88]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #142
	adds r6, r7, r0
	movs r1, #0
	ldrsh r0, [r6, r1]
	bl Func_080dce20
	adds r5, r0, #0
	ldrh r0, [r6]
	lsls r5, r5, #16
	adds r0, #120
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Func_080dce20
	lsls r0, r0, #16
	asrs r0, r0, #16
	mov r8, r0
	ldrh r0, [r6]
	asrs r5, r5, #16
	adds r0, #240
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Func_080dce20
	lsls r0, r0, #16
	asrs r4, r0, #16
	cmp r5, #0
	bge .L_080dceaa
	adds r5, #3
.L_080dceaa:
	movs r0, #198
	asrs r3, r5, #2
	lsls r0, r0, #1
	adds r0, #255
	adds r3, #4
	movs r1, #31
	adds r2, r7, r0
	ands r3, r1
	mov r0, r8
	strb r3, [r2]
	cmp r0, #0
	bge .L_080dcec4
	adds r0, #3
.L_080dcec4:
	asrs r3, r0, #2
	movs r0, #163
	lsls r0, r0, #2
	adds r3, #4
	adds r2, r7, r0
	ands r3, r1
	adds r0, r4, #0
	strb r3, [r2]
	cmp r0, #0
	bge .L_080dceda
	adds r0, #3
.L_080dceda:
	asrs r3, r0, #2
	adds r3, #4
	ands r3, r1
	movs r1, #199
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r7, r1
	strb r3, [r2]
	ldr r2, .L_080dcf04
	ldrh r3, [r6]
	adds r3, #4
	strh r3, [r6]
	lsls r3, r3, #16
	cmp r3, r2
	bls .L_080dcefc
	movs r3, #0
	strh r3, [r6]
.L_080dcefc:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dcf04:
	.4byte 0x01670000
