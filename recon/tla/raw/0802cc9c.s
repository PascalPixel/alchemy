.syntax unified
	.thumb
	.global Func_0802cc9c
	.thumb_func
Func_0802cc9c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #32]
	adds r5, r0, #0
	adds r0, r6, #0
	movs r1, #192
	adds r0, #24
	ldr r3, .L_0802cd44
	mov lr, r3
	.2byte 0xf800
	ldrh r1, [r5]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	movs r7, #0
	adds r5, #2
	cmp r1, r2
	beq .L_0802cd1c
	movs r3, #255
	movs r2, #253
	lsls r3, r3, #8
	lsls r2, r2, #8
	mov r10, r3
	mov r8, r2
	movs r3, #15
	movs r2, #128
	mov lr, r3
	mov r12, r2
	movs r4, #0
.L_0802ccde:
	adds r3, r1, #0
	mov r2, r10
	ands r3, r2
	cmp r3, r8
	bne .L_0802cd0e
	mov r3, lr
	adds r2, r1, #0
	ands r2, r3
	mov r3, r12
	ands r3, r1
	movs r0, #0
	cmp r3, #0
	beq .L_0802ccfa
	movs r0, #1
.L_0802ccfa:
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r6, r3
	adds r3, #24
	str r5, [r3]
	str r5, [r3, #4]
	strh r4, [r3, #8]
	strh r0, [r3, #10]
	adds r7, #1
.L_0802cd0e:
	ldrh r1, [r5]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r5, #2
	cmp r1, r2
	bne .L_0802ccde
.L_0802cd1c:
	cmp r7, #0
	beq .L_0802cd3a
	ldr r5, .L_0802cd48
	adds r0, r5, #0
	bl Func_0801456c
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0802cd3a
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Func_080145a8
.L_0802cd3a:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802cd44:
	.4byte IwramClearWords
.L_0802cd48:
	.4byte Func_0802cb64
