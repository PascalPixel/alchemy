@ Text_CopyFiveCharsPaddingUnderscore. Unresolved C allocation; no authorship claim.
.syntax unified
.thumb
.global Text_CopyFiveCharsPaddingUnderscore
.thumb_func
Text_CopyFiveCharsPaddingUnderscore:
    push {r5, r6, lr}
    sub sp, #8
    mov r5, sp
    movs r6, #95
    adds r4, r5, #0
    adds r0, r5, #0
    movs r1, #3
.Lcopy_character:
    ldrh r3, [r2]
    strb r3, [r0]
    lsls r3, r3, #24
    adds r2, #2
    adds r0, #1
    cmp r3, #0
    bne .Lnext_character
    strb r6, [r4]
.Lnext_character:
    subs r1, #1
    adds r4, #1
    cmp r1, #0
    bge .Lcopy_character
    movs r3, #0
    strb r3, [r5, #4]
    add sp, #8
    pop {r5, r6}
    pop {r1}
    bx r1
