.syntax unified
	.thumb
	.global Func_080fecfc
	.thumb_func
Func_080fecfc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #139
	lsls r2, r2, #1
	mov r8, r3
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	sub sp, #56
	cmp r3, #1
	bls .L_080fed2a
	cmp r1, #1
	bne .L_080fed2e
	ldrb r3, [r2]
	subs r3, #1
	cmp r0, r3
	bne .L_080fed32
.L_080fed2a:
	movs r0, #0
	b .L_080fedfa
.L_080fed2e:
	cmp r0, #0
	beq .L_080fed2a
.L_080fed32:
	mov r7, sp
	movs r2, #0
	add r3, sp, #52
	mov r12, r7
.L_080fed3a:
	str r2, [r3]
	subs r3, #4
	cmp r3, r12
	bge .L_080fed3a
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	movs r6, #0
	cmp r6, r3
	bge .L_080fed70
	movs r5, #139
	lsls r5, r5, #1
	movs r2, #129
	adds r5, #255
	lsls r2, r2, #2
	add r5, r8
	adds r4, r7, #0
	add r2, r8
.L_080fed62:
	ldrh r3, [r2]
	adds r6, #1
	stmia r4!, {r3}
	adds r2, #2
	ldrb r3, [r5]
	cmp r6, r3
	blt .L_080fed62
.L_080fed70:
	cmp r1, #1
	bne .L_080fed7a
	lsls r3, r0, #2
	adds r1, r3, #4
	b .L_080fed7e
.L_080fed7a:
	lsls r3, r0, #2
	subs r1, r3, #4
.L_080fed7e:
	ldr r6, [r7, r3]
	ldr r2, [r7, r1]
	str r2, [r7, r3]
	str r6, [r7, r1]
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	movs r6, #0
	cmp r6, r3
	bge .L_080fedb8
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	movs r5, #129
	add r2, r8
	lsls r5, r5, #2
	mov r10, r2
	add r5, r8
.L_080feda6:
	ldrh r0, [r5]
	bl Party_RemoveActiveOwnerFar
	mov r2, r10
	ldrb r3, [r2]
	adds r6, #1
	adds r5, #2
	cmp r6, r3
	blt .L_080feda6
.L_080fedb8:
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	movs r6, #0
	cmp r6, r3
	bge .L_080fede4
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	mov r10, r3
	adds r5, r7, #0
.L_080fedd4:
	ldmia r5!, {r0}
	bl Party_AddActiveOwnerFar
	mov r2, r10
	ldrb r3, [r2]
	adds r6, #1
	cmp r6, r3
	blt .L_080fedd4
.L_080fede4:
	movs r0, #129
	lsls r0, r0, #2
	add r0, r8
	bl Party_ListActiveOwnersFar
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	strb r0, [r3]
	movs r0, #1
.L_080fedfa:
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
