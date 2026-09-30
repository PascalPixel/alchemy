.syntax unified
	.thumb
	.global Func_080fe49c
	.thumb_func
Func_080fe49c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r5, #0
	mov r10, r3
	ldr r3, .L_080fe57c
	movs r7, #0
	movs r6, #0
	mov r8, r3
	b .L_080fe552
.L_080fe4ba:
	cmp r5, #1
	beq .L_080fe508
	cmp r5, #1
	bgt .L_080fe4c8
	cmp r5, #0
	beq .L_080fe4d2
	b .L_080fe550
.L_080fe4c8:
	cmp r5, #2
	beq .L_080fe520
	cmp r5, #3
	beq .L_080fe53a
	b .L_080fe550
.L_080fe4d2:
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r1, #144
	strh r7, [r3]
	lsls r1, r1, #3
	mov r0, r8
	bl Func_080145a8
	movs r0, #1
	bl Func_081054cc
	movs r0, #0
	bl Func_080fe580
	adds r5, r0, #0
	mov r0, r8
	bl Func_08014644
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	bne .L_080fe504
	adds r6, r5, #0
	movs r7, #1
.L_080fe504:
	movs r5, #1
	b .L_080fe552
.L_080fe508:
	movs r0, #1
	bl Func_081054cc
	bl Func_080feec0
	adds r6, r0, #0
	mvns r2, r6
	negs r3, r2
	orrs r3, r2
	lsrs r5, r3, #31
	lsls r5, r5, #1
	b .L_080fe552
.L_080fe520:
	movs r0, #0
	bl Func_081054cc
	bl Func_080ffcd4
	movs r3, #1
	adds r6, r0, #0
	negs r3, r3
	movs r5, #0
	cmp r6, r3
	beq .L_080fe552
	movs r5, #3
	b .L_080fe552
.L_080fe53a:
	movs r0, #0
	bl Func_081054cc
	bl Func_0810021c
	adds r6, r0, #0
	mvns r2, r6
	negs r3, r2
	orrs r3, r2
	lsrs r5, r3, #31
	b .L_080fe552
.L_080fe550:
	movs r7, #1
.L_080fe552:
	cmp r7, #0
	bne .L_080fe562
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fe4ba
.L_080fe562:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fe572
	movs r6, #1
	negs r6, r6
.L_080fe572:
	adds r0, r6, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080fe57c:
	.4byte Func_08104da8
