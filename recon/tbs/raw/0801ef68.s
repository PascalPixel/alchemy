.syntax unified
	.thumb
	.global UiWindow_DrawColumnBorders
	.thumb_func
UiWindow_DrawColumnBorders:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0801eff4
	ldr r3, [r3]
	adds r6, r0, #0
	sub sp, #4
	mov r9, r3
	movs r3, #0
	str r3, [sp, #0]
	ldrh r3, [r6, #8]
	subs r3, #1
	movs r2, #1
	mov r11, r3
	adds r3, r1, #0
	ands r3, r2
	ldrh r7, [r6, #10]
	cmp r3, #0
	bne .L_0801ef9c
	movs r3, #3
	negs r3, r3
	ands r1, r3
.L_0801ef9c:
	movs r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_0801efaa
	movs r2, #5
	str r2, [sp, #0]
	movs r2, #0
.L_0801efaa:
	ldr r1, .L_0801eff8
	adds r5, r2, #0
	b .L_0801f012
.L_0801efb0:
	ldrsb r3, [r2, r5]
	ldr r2, [sp, #0]
	adds r0, r3, r2
	cmp r0, r11
	bcs .L_0801f010
	movs r4, #0
	cmp r7, #0
	beq .L_0801f010
	ldr r3, .L_0801effc
	subs r2, r7, #1
	mov r12, r2
	ldr r2, .L_0801f000
	mov r10, r3
	adds r3, #1
	mov r8, r3
	mov lr, r2
.L_0801efd0:
	ldrh r2, [r6, #14]
	ldrh r3, [r6, #12]
	adds r2, r2, r4
	adds r3, r3, r0
	lsls r2, r2, #5
	adds r2, r2, r3
	lsls r2, r2, #1
	mov r3, r9
	adds r1, r2, r3
	cmp r4, #0
	bne .L_0801efea
	mov r2, r10
	b .L_0801f006
.L_0801efea:
	cmp r4, r12
	bne .L_0801f004
	mov r3, r8
	strh r3, [r1]
	b .L_0801f008
.L_0801eff4:
	.4byte gWindowWork
.L_0801eff8:
	.4byte Data_080371c4
.L_0801effc:
	.4byte 0x0000f018
.L_0801f000:
	.4byte 0x0000f00f
.L_0801f004:
	mov r2, lr
.L_0801f006:
	strh r2, [r1]
.L_0801f008:
	adds r4, #1
	cmp r4, r7
	bne .L_0801efd0
	ldr r1, .L_0801f06c
.L_0801f010:
	adds r5, #1
.L_0801f012:
	adds r2, r1, #0
	ldrsb r3, [r2, r5]
	cmp r3, #0
	bge .L_0801efb0
	ldr r3, .L_0801f070
	add r3, r9
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0801f054
	ldrh r3, [r6, #10]
	ldrh r2, [r6, #14]
	adds r2, r2, r3
	ldrh r3, [r6, #12]
	lsls r2, r2, #6
	lsls r3, r3, #1
	add r2, r9
	adds r2, r2, r3
	adds r1, r2, #0
	ldr r3, .L_0801f060
	subs r1, #64
	movs r0, #1
	strh r3, [r1]
	adds r1, #2
	cmp r0, r11
	bcs .L_0801f050
	ldr r3, .L_0801f064
.L_0801f046:
	adds r0, #1
	strh r3, [r1]
	adds r1, #2
	cmp r0, r11
	bcc .L_0801f046
.L_0801f050:
	ldr r3, .L_0801f068
	strh r3, [r1]
.L_0801f054:
	ldr r2, .L_0801f074
	movs r3, #1
	add r2, r9
	strb r3, [r2]
	add sp, #4
	b .L_0801f078
.L_0801f060:
	.4byte 0x0000f080
.L_0801f064:
	.4byte 0x0000f081
.L_0801f068:
	.4byte 0x0000f082
.L_0801f06c:
	.4byte Data_080371c4
.L_0801f070:
	.4byte 0x00000ea5
.L_0801f074:
	.4byte 0x00000ea3
.L_0801f078:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
