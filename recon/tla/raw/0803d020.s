.syntax unified
	.thumb
	.global Func_0803d020
	.thumb_func
Func_0803d020:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #66
	adds r3, r6, r2
	movs r7, #0
	strh r7, [r3]
	mov r10, r1
	movs r1, #0
	sub sp, #8
	mov r8, r1
	bl UiText_BuildRenderEntries
	movs r3, #0
	adds r5, r0, #0
	add r1, sp, #4
	mov r2, sp
	bl UiText_MeasureEntryDimensions
	lsls r5, r5, #1
	movs r3, #244
	lsls r3, r3, #4
	adds r0, r6, r5
	adds r5, r0, r3
	ldrh r3, [r5]
	cmp r3, #45
	bne .L_0803d086
	ldrh r3, [r5, #2]
	movs r0, #0
	cmp r3, #45
	beq .L_0803d06c
	b .L_0803d168
.L_0803d06c:
	ldrh r2, [r5, #4]
	cmp r2, #45
	bne .L_0803d082
	ldrh r3, [r5, #6]
	movs r0, #3
	eors r3, r2
	negs r2, r3
	orrs r2, r3
	lsrs r2, r2, #31
	subs r0, r0, r2
	b .L_0803d168
.L_0803d082:
	movs r0, #1
	b .L_0803d168
.L_0803d086:
	ldr r3, [sp, #4]
	movs r2, #192
	subs r2, r2, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r7, r2, #1
	ldrh r1, [r5]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #66
	adds r5, r0, r2
	cmp r1, #0
	beq .L_0803d166
.L_0803d0a0:
	cmp r1, #30
	bhi .L_0803d152
	subs r1, #3
	cmp r1, #25
	bhi .L_0803d15e
	ldr r2, .L_0803d174
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0803d0b4:
	.4byte .L_0803d146
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d140
	.4byte .L_0803d11c
	.4byte .L_0803d126
	.4byte .L_0803d130
	.4byte .L_0803d13c
	.4byte .L_0803d13c
	.4byte .L_0803d15e
	.4byte .L_0803d14e
	.4byte .L_0803d14e
	.4byte .L_0803d15e
	.4byte .L_0803d13c
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d15e
	.4byte .L_0803d14e
.L_0803d11c:
	movs r1, #240
	ldrh r2, [r5]
	lsls r1, r1, #4
	adds r1, #62
	b .L_0803d138
.L_0803d126:
	movs r1, #240
	ldrh r2, [r5]
	lsls r1, r1, #4
	adds r1, #60
	b .L_0803d138
.L_0803d130:
	movs r1, #240
	ldrh r2, [r5]
	lsls r1, r1, #4
	adds r1, #56
.L_0803d138:
	adds r3, r6, r1
	strh r2, [r3]
.L_0803d13c:
	adds r5, #2
	b .L_0803d15e
.L_0803d140:
	bl Func_0803a404
	b .L_0803d15e
.L_0803d146:
	movs r2, #15
	movs r7, #0
	add r8, r2
	b .L_0803d15e
.L_0803d14e:
	adds r5, #2
	b .L_0803d13c
.L_0803d152:
	adds r2, r7, #0
	mov r0, r10
	mov r3, r8
	bl Func_0803cfd0
	adds r7, r7, r0
.L_0803d15e:
	ldrh r1, [r5]
	adds r5, #2
	cmp r1, #0
	bne .L_0803d0a0
.L_0803d166:
	movs r0, #0
.L_0803d168:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803d174:
	.4byte .L_0803d0b4
