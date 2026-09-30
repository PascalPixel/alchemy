.syntax unified
	.thumb
	.global Func_08108690
	.thumb_func
Func_08108690:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_08108758
	movs r3, #0
	mov r10, r3
	ldr r3, [r5, #12]
	ldr r6, [r0]
	adds r7, r2, #0
	movs r2, #32
	ands r3, r2
	mov r11, r0
	mov r8, r1
	mov r9, r6
	cmp r3, #0
	beq .L_081086c2
	subs r6, #1
	adds r0, r6, r1
	bl __modsi3
	adds r6, r0, #0
.L_081086c2:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_081086da
	adds r6, #1
	mov r3, r8
	adds r0, r6, r3
	mov r1, r8
	bl __modsi3
	adds r6, r0, #0
.L_081086da:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_081086ec
	subs r3, r6, r7
	cmp r3, #0
	blt .L_081086ec
	adds r6, r3, #0
.L_081086ec:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08108718
	mov r3, r8
	adds r0, r7, r3
	subs r0, #1
	adds r1, r7, #0
	bl Math_Div
	adds r3, r7, #0
	muls r3, r0
	adds r5, r6, r7
	cmp r5, r3
	bge .L_0810870e
	adds r6, r5, #0
.L_0810870e:
	mov r1, r8
	subs r1, #1
	cmp r6, r1
	ble .L_08108718
	adds r6, r1, #0
.L_08108718:
	cmp r9, r6
	beq .L_08108726
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	mov r10, r3
.L_08108726:
	adds r1, r7, #0
	mov r0, r9
	bl Math_Div
	adds r1, r7, #0
	adds r5, r7, #0
	muls r5, r0
	adds r0, r6, #0
	bl Math_Div
	adds r3, r7, #0
	muls r3, r0
	cmp r5, r3
	beq .L_08108746
	movs r3, #2
	mov r10, r3
.L_08108746:
	mov r3, r11
	str r6, [r3]
	mov r0, r10
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08108758:
	.4byte gInput
