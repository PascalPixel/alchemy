.syntax unified
	.thumb
	.global Func_0810a760
	.thumb_func
Func_0810a760:
	push {r5, r6, r7, lr}
	bl Func_0810a70c
	adds r5, r0, #0
	movs r6, #0
	movs r7, #0
	cmp r5, #0
	beq .L_0810a7d8
	movs r2, #4
	ldrsh r3, [r5, r2]
	movs r0, #0
	cmp r3, #0
	beq .L_0810a794
	adds r2, r5, #4
	movs r1, #20
.L_0810a77e:
	ldrsh r3, [r1, r5]
	adds r0, #1
	adds r6, r6, r3
	adds r1, #2
	cmp r0, #7
	bgt .L_0810a794
	adds r2, #2
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #0
	bne .L_0810a77e
.L_0810a794:
	bl Random16
	adds r3, r6, #0
	muls r3, r0
	lsrs r1, r3, #16
	movs r2, #4
	ldrsh r3, [r5, r2]
	movs r0, #0
	cmp r3, #0
	beq .L_0810a7ce
	movs r4, #20
	ldrsh r3, [r5, r4]
	subs r1, r1, r3
	cmp r1, #0
	blt .L_0810a7ce
	adds r2, r5, #4
.L_0810a7b4:
	adds r0, #1
	adds r2, #2
	cmp r0, #7
	bgt .L_0810a7ce
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #0
	beq .L_0810a7ce
	movs r4, #16
	ldrsh r3, [r2, r4]
	subs r1, r1, r3
	cmp r1, #0
	bge .L_0810a7b4
.L_0810a7ce:
	cmp r0, #8
	beq .L_0810a7d8
	lsls r3, r0, #1
	adds r3, #4
	ldrsh r7, [r5, r3]
.L_0810a7d8:
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
