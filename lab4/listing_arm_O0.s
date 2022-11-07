CalcSin(int, double):
        push    {r7}
        sub     sp, sp, #52
        add     r7, sp, #0
        str     r0, [r7, #12]
        vstr.64 d0, [r7]
        mov     r2, #0
        mov     r3, #0
        strd    r2, [r7, #40]
        movs    r3, #1
        str     r3, [r7, #36]
        mov     r2, #0
        mov     r3, #0
        movt    r3, 16368
        strd    r2, [r7, #24]
        movs    r3, #1
        str     r3, [r7, #20]
        movs    r3, #0
        str     r3, [r7, #16]
        b       .L2
.L3:
        ldr     r3, [r7, #20]
        vmov    s15, r3 @ int
        vcvt.f64.s32    d17, s15
        vldr.64 d18, [r7]
        vdiv.f64        d16, d18, d17
        vldr.64 d17, [r7, #24]
        vmul.f64        d16, d17, d16
        vstr.64 d16, [r7, #24]
        ldr     r3, [r7, #20]
        adds    r3, r3, #1
        str     r3, [r7, #20]
        ldr     r3, [r7, #36]
        vmov    s15, r3 @ int
        vcvt.f64.s32    d17, s15
        vldr.64 d16, [r7, #24]
        vmul.f64        d16, d17, d16
        vldr.64 d17, [r7, #40]
        vadd.f64        d16, d17, d16
        vstr.64 d16, [r7, #40]
        ldr     r3, [r7, #20]
        vmov    s15, r3 @ int
        vcvt.f64.s32    d17, s15
        vldr.64 d18, [r7]
        vdiv.f64        d16, d18, d17
        vldr.64 d17, [r7, #24]
        vmul.f64        d16, d17, d16
        vstr.64 d16, [r7, #24]
        ldr     r3, [r7, #20]
        adds    r3, r3, #1
        str     r3, [r7, #20]
        ldr     r3, [r7, #36]
        rsbs    r3, r3, #0
        str     r3, [r7, #36]
        ldr     r3, [r7, #16]
        adds    r3, r3, #1
        str     r3, [r7, #16]
.L2:
        ldr     r2, [r7, #16]
        ldr     r3, [r7, #12]
        cmp     r2, r3
        blt     .L3
        ldrd    r2, [r7, #40]
        vmov    d16, r2, r3
        vmov.f64        d0, d16
        adds    r7, r7, #52
        mov     sp, r7
        ldr     r7, [sp], #4
        bx      lr
.LC0:
        .ascii  "too few arguments. try again\000"
.LC1:
        .ascii  "too many arguments. try again\000"
.LC2:
        .ascii  "%lf\012\000"
.LC3:
        .ascii  "(clock_gettime) total time: %lf sec.\012\000"
main:
        push    {r7, lr} // sp -= 8; помещение lr и r7 на стек.
        sub     sp, sp, #40 // sp = sp - 40.
        add     r7, sp, #0 // r7 = sp + 0.
        str     r0, [r7, #4] // по адресу (r7 + 4) помещается значение, лежащее в r0 [argc].
        str     r1, [r7] // по адресу (r7) помещается значение, лежащее в r1.
        ldr     r3, [r7, #4] // в регистр r3 загружается значение, лежащее по адресу (r7 + 4) [argc].
        cmp     r3, #2 // сравнение значения из r3 [argc] со значением 2 [обновление флагов].
        bgt     .L6 // если argc > 2, то
        movw    r0, #:lower16:.LC0
        movt    r0, #:upper16:.LC0
        bl      puts
        movs    r3, #0
        b       .L9
.L6:
        ldr     r3, [r7, #4]
        cmp     r3, #3
        ble     .L8
        movw    r0, #:lower16:.LC1
        movt    r0, #:upper16:.LC1
        bl      puts
        movs    r3, #0
        b       .L9
.L8:
        ldr     r3, [r7]
        adds    r3, r3, #4
        ldr     r3, [r3]
        movs    r2, #10
        movs    r1, #0
        mov     r0, r3
        bl      strtol
        str     r0, [r7, #36]
        ldr     r3, [r7]
        adds    r3, r3, #8
        ldr     r3, [r3]
        movs    r1, #0
        mov     r0, r3
        bl      strtod
        vstr.64 d0, [r7, #24]
        add     r3, r7, #16
        mov     r1, r3
        movs    r0, #4
        bl      clock_gettime
        vldr.64 d16, [r7, #24]
        vldr.64 d17, .L10
        vmul.f64        d16, d16, d17
        vmov.f64        d0, d16
        ldr     r0, [r7, #36]
        bl      CalcSin(int, double)
        vmov    r2, r3, d0
        movw    r0, #:lower16:.LC2
        movt    r0, #:upper16:.LC2
        bl      printf
        add     r3, r7, #8
        mov     r1, r3
        movs    r0, #4
        bl      clock_gettime
        ldr     r2, [r7, #8]
        ldr     r3, [r7, #16]
        subs    r3, r2, r3
        vmov    s15, r3 @ int
        vcvt.f64.s32    d17, s15
        ldr     r2, [r7, #12]
        ldr     r3, [r7, #20]
        subs    r3, r2, r3
        vmov    s15, r3 @ int
        vcvt.f64.s32    d16, s15
        vldr.64 d18, .L10+8
        vmul.f64        d16, d16, d18
        vadd.f64        d16, d17, d16
        vmov    r2, r3, d16
        movw    r0, #:lower16:.LC3
        movt    r0, #:upper16:.LC3
        bl      printf
        movs    r3, #0
.L9:
        mov     r0, r3 // копирует значение из r3 в r0.
        adds    r7, r7, #40 // r7 = r7 + 40.
        mov     sp, r7 // копирует значение из r7 в sp.
        pop     {r7, pc} // извлекает со стека два значения и помещает их в r7 и pc; sp += 8.
.L10:
        .word   14568529
        .word   1066524467
        .word   -400107883
        .word   1041313291
