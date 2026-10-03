.syntax unified
	.thumb
	.global Func_080d7788
	.thumb_func
Func_080d7788:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r2, #0
	mov r8, r0
	adds r5, r1, #0
	cmp r6, #19
	bne .L_080d77aa
	movs r1, #200
	ldr r2, .L_080d7844
	lsls r1, r1, #5
	adds r1, #80
	adds r3, r5, r1
	ldrsb r1, [r2, r3]
	cmp r1, #0
	beq .L_080d783e
	subs r6, r1, #1
.L_080d77aa:
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d77d0
	lsls r0, r5, #2
	ldr r3, .L_080d7848
	adds r0, r0, r5
	movs r2, #133
	lsls r0, r0, #2
	lsls r2, r2, #2
	adds r0, r0, r6
	adds r3, r3, r2
	adds r0, #48
	ldr r7, [r3]
	bl GameFlag_SetBit
	b .L_080d77da
.L_080d77d0:
	adds r0, r5, #0
	adds r1, r6, #0
	bl Djinn_AddToLeastLoadedOwnerFar
	adds r7, r0, #0
.L_080d77da:
	cmp r7, #0
	blt .L_080d783e
	bl EventRuntime_Begin
	movs r3, #1
	negs r3, r3
	cmp r8, r3
	beq .L_080d7818
	cmp r5, #0
	bne .L_080d77f6
	mov r0, r8
	bl Func_080d82e0
	b .L_080d7818
.L_080d77f6:
	cmp r5, #1
	bne .L_080d7802
	mov r0, r8
	bl Func_080d8740
	b .L_080d7818
.L_080d7802:
	cmp r5, #2
	bne .L_080d780e
	mov r0, r8
	bl Func_080d7f80
	b .L_080d7818
.L_080d780e:
	cmp r5, #3
	bne .L_080d7818
	mov r0, r8
	bl Func_080d7c04
.L_080d7818:
	bl Func_080cb82c
	adds r1, r5, #0
	adds r2, r6, #0
	adds r0, r7, #0
	bl Func_08038328
	bl Func_080cb8a4
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	mov r2, r8
	lsls r3, r2, #2
	adds r3, #20
	movs r2, #0
	str r2, [r1, r3]
	bl EventRuntime_End
.L_080d783e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080d7844:
	.4byte Data_02001000
.L_080d7848:
	.4byte gPartyState
