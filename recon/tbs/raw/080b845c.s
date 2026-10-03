@ Uncredited: complete owned-English-ROM extent, including its literal pool.
@ The prior C reserved an unread 12-byte frame; ordinary draft retained.
.syntax unified
.thumb
.text
.balign 4
.global BattleMotion_ProjectScaledPosition
.type BattleMotion_ProjectScaledPosition, %function
.thumb_func
BattleMotion_ProjectScaledPosition:
    push {r5, r6, lr}
    mov r6, r10
    mov r5, r8
    push {r5, r6}
    sub sp, #12
    mov r8, r1
    mov r10, r0
    bl GetBattleObjectSlot
    ldr r5, [r0]
    movs r1, #0
    adds r0, r5, #0
    bl GetMotionRecord
    adds r5, #8
    adds r6, r0, #0
    bl Camera_ApplyTransformByFlag
    mov r1, r8
    adds r0, r5, #0
    bl Render_ProjectPoint
    ldr r5, .L_multiply
    ldr r1, [r6, #24]
    mov r12, pc
    bx r5
    adds r6, r0, #0
    mov r0, r10
    bl Battle_GetObjectTableValue
    adds r1, r0, #0
    asrs r1, r1, #16
    adds r0, r6, #0
    movs r0, r0
    mov r12, pc
    bx r5
    mov r2, r8
    ldr r3, [r2, #4]
    subs r3, r3, r0
    str r3, [r2, #4]
    movs r0, #0
    add sp, #12
    pop {r3, r5}
    mov r8, r3
    mov r10, r5
    pop {r5, r6}
    pop {r1}
    bx r1
.L_multiply:
    .word IwramMulQ16ReturnIp
.size BattleMotion_ProjectScaledPosition, .-BattleMotion_ProjectScaledPosition
