.syntax unified
	.thumb
	.global Func_080dbcd8
	.thumb_func
Func_080dbcd8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	adds r2, r0, #0
	movs r0, #197
	lsls r0, r0, #1
	adds r3, r5, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #12
	cmp r3, #3
	bne .L_080dbd0e
	mov r0, sp
	movs r3, #0
	str r2, [r0]
	str r3, [r0, #4]
	str r1, [r0, #8]
	bl GetWorldMapCollisionFar
	subs r0, #5
	movs r2, #0
	cmp r0, #1
	bhi .L_080dbd26
	movs r2, #1
	b .L_080dbd26
.L_080dbd0e:
	adds r0, r2, #0
	movs r2, #2
	bl Func_080dbb78
	ldrb r2, [r0, #2]
	movs r3, #232
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r2, r3, #31
	movs r3, #1
	subs r2, r3, r2
.L_080dbd26:
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #55
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080dbd42
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	movs r2, #1
	subs r2, r2, r3
.L_080dbd42:
	adds r0, r2, #0
	add sp, #12
	pop {r5, pc}
