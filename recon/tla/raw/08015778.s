.syntax unified
	.thumb
	.global Render_ProjectPoint
	.thumb_func
Render_ProjectPoint:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	ldr r3, .L_0801581c
	adds r7, r1, #0
	mov r1, sp
	mov lr, r3
	.2byte 0xf800
	mov r6, sp
	ldr r3, [r6, #8]
	ldr r0, .L_08015820
	negs r1, r3
	ldr r3, [r0, #4]
	mov r10, r0
	cmp r1, r3
	blt .L_08015806
	ldr r3, [r0, #8]
	cmp r1, r3
	bgt .L_08015806
	asrs r3, r1, #16
	str r3, [r7, #8]
	ldr r0, [r0]
	cmp r0, #0
	beq .L_080157bc
	lsrs r1, r1, #11
	lsls r0, r0, #5
	ldr r3, .L_08015824
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	b .L_080157be
.L_080157bc:
	ldr r5, .L_08015828
.L_080157be:
	ldr r1, .L_0801582c
	ldr r0, [r6]
	mov r9, r1
	adds r1, r5, #0
	mov lr, r9
	.2byte 0xf800
	cmp r0, #0
	bge .L_080157d6
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r0, r0, r2
.L_080157d6:
	mov r4, r10
	ldr r2, [r4, #12]
	asrs r3, r0, #16
	adds r2, r2, r3
	ldr r0, [r6, #4]
	adds r1, r5, #0
	mov r8, r2
	mov lr, r9
	.2byte 0xf800
	cmp r0, #0
	bge .L_080157f4
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_080157f4:
	mov r4, r10
	ldr r3, [r4, #16]
	asrs r2, r0, #16
	subs r3, r3, r2
	mov r0, r8
	str r0, [r7]
	str r3, [r7, #4]
	adds r0, r5, #0
	b .L_08015810
.L_08015806:
	adds r2, r7, #0
	adds r3, r6, #0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r0, #0
.L_08015810:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0801581c:
	.4byte IwramTransformVector
.L_08015820:
	.4byte gCameraSceneParameters
.L_08015824:
	.4byte IwramUnsignedDivide
.L_08015828:
	.4byte 0x000151eb
.L_0801582c:
	.4byte IwramMulQ16
