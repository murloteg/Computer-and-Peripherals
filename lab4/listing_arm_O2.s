CalcSin(int, double):
        vmov.f64        d18, d0 // в регистр d18 копируется d0 [argument].
        cmp     r0, #0 // сравнение r0 [number] и значения 0 [number - 0 = 0].
        ble     .L4 // если number <= 0, то переход на метку .L4.
        movs    r3, #1 // в r3 копируется значение 1 [factorial = 1].
        lsls    r0, r0, #1 // логический сдвиг влево: r0 = r0 * 2 [number * 2].
        vmov.i64        d0, #0  @ float // в d0 копируется значение 0 [result = 0].
        add     r0, r0, r3 // r0 = r0 + r3 [number * 2 + 1].
        mov     r1, r3 // в r1 копируется значение r3 [signStatus = 1].
        vmov.f64        d16, #1.0e+0 // в d16 копируется значение 1 * 10^(0) [nextMultiplier].
.L3:
        vmov    s15, r3 @ int // в s15 копируется r3 [factorial].
        adds    r2, r3, #1 // r2 = r3 + 1 [factorial + 1] + обновление флагов.
        adds    r3, r3, #2 // r3 = r3 + 2 [factorial + 2].
        vcvt.f64.s32    d19, s15 // в d19 записывается (double) s15 (преобразование одинарной точности в двойную) [factorial].
        vmov    s15, r2 @ int // в s15 копируется r2 [factorial + 1].
        cmp     r0, r3 // сравнение r0 [number * 2 + 1] и r3 [factorial + 2].
        vcvt.f64.s32    d17, s15 // в d17 записывается (double) s15 (преобразование одинарной точности в двойную) [factorial + 1].
        vmov    s15, r1 @ int // в s15 копируется значение r1 [signStatus].
        rsb     r1, r1, #0 // r1 = 0 - r1 [reverse substract].
        vdiv.f64        d20, d18, d19 // d20 = d18 / d19 [argument / factorial].
        vdiv.f64        d19, d18, d17 // d19 = d18 / d17 [argument / (factorial + 1)]
        vcvt.f64.s32    d17, s15 // в d17 записывается (double) s15 (преобразование одинарной точности в двойную) [signStatus].
        vmul.f64        d16, d20, d16 // d16 = d20 * d16 [nextMultiplier * (argument / factorial)].
        vmla.f64        d0, d17, d16 // перемножает регистры d17 и d16, добавляя результат в d0 [result += signStatus * nextMultiplier * (argument / factorial)].
        vmul.f64        d16, d19, d16 // d16 = d19 * d16 [nextMultiplier * (argument / factorial) *= argument / (factorial + 1)].
        bne     .L3 // если (factorial + 2) != (number * 2 + 1), то следующая итерация цикла.
        bx      lr // иначе: возврат из функции.
.L4:
        vmov.i64        d0, #0  @ float // в d0 [result] копируется значение 0.
        bx      lr // возврат из функции.
.LC0:
        .ascii  "too few arguments. try again\000"
.LC1:
        .ascii  "too many arguments. try again\000"
.LC2:
        .ascii  "%lf\012\000"
.LC3:
        .ascii  "(clock_gettime) total time: %lf sec.\012\000"
main:
        push    {r4, lr} // сначала sp -= 8, затем помещение lr и r4 на стек.
        cmp     r0, #2 // сравнение r0 [argc] и 2, обновление флагов [argc - 2 = 0].
        vpush.64        {d8} // помещение d8 на стек.
        sub     sp, sp, #16 // sp = sp - 16.
        ble     .L12 // если argc <= 2, то переход на метку .L12.
        cmp     r0, #3 // сравнение r0 [argc] и 3, обновление флагов [argc - 3 = 0].
        bne     .L13 // если argc != 3, то переход на метку .L13.
        mov     r4, r1 // в регистр r4 копируется значение r1 [argv].
        movs    r2, #10 // в регистр r2 копируется значение 10.
        movs    r1, #0 // в регистр r1 копируется значение 0 [NULL].
        ldr     r0, [r4, #4] // в регистр r0 загружается значение, лежащее по адресу (r4 + 4) [argv[1]].
        bl      strtol // вызов функции strtol.
        movs    r1, #0 // в регистр r1 копируется значение 0 [NULL].
        mov     r3, r0 // в регистр r3 копируется значение r0 [результат функции strtol -> number].
        ldr     r0, [r4, #8] // в регистр r0 загружается значение, лежащее по адресу (r4 + 8) [argv[2]].
        mov     r4, r3 // в регистр r4 копируется значение r3 [number].
        bl      strtod // вызов функции strtod.
        mov     r1, sp // в регистр r1 копируется sp.
        vmov.f64        d8, d0 // в регистр d8 копируется d0 [результат функции strtod -> argument].
        movs    r0, #4 // в регистр r0 копируется значение 4 (CLOCK_MONOTONIC_RAW).
        bl      clock_gettime // вызов функции clock_gettime.
        vldr.64 d0, .L14 // в регистр d0 копируется константа, лежащая по метке .L14 (0.017453).
        mov     r0, r4 // в регистр r0 копируется r4 [number].
        vmul.f64        d0, d8, d0 // d0 = d8 * d0 [argument * 0.017453].
        bl      CalcSin(int, double) // вызов функции CalcSin.
        movw    r0, #:lower16:.LC2 // записывает в регистр r0 [0:15] (16 бит) 32-битного значения.
        movt    r0, #:upper16:.LC2 // записывает в регистр r0 [16:31] (16 бит) 32-битного значения.
        vmov    r2, r3, d0 // в регистре r2 будут [0:31] биты d0, в r3 - [32:63] [результат выполнения функции CalcSin].
        bl      printf // вызов функции printf.
        add     r1, sp, #8 // r1 = sp + 8.
        movs    r0, #4 // в регистр r0 записывается значение 4 (CLOCK_MONOTONIC_RAW).
        bl      clock_gettime // вызов функции clock_gettime.
        ldr     r2, [sp, #12] // в r2 загружается значение, лежащее по адресу (sp + 12) [end_clock.tv_nsec].
        ldrd    r0, r3, [sp, #4] // в регистр r2 загружается [start_clock.tv_nsec], в r3 -> [end_clock.tv_sec].
        ldr     r1, [sp] // в r1 загружается значение, лежащее по адресу (sp) [start_clock.tv_sec].
        subs    r2, r2, r0 // r2 = r2 - r0 [end_clock.tv_nsec - start_clock.tv_nsec].
        vmov    s15, r2 @ int // в s15 копируется разность наносекунд.
        subs    r3, r3, r1 // r3 = r3 - r1 [end_clock.tv_sec - start_clock.tv_sec].
        vldr.64 d18, .L14+8 // в d18 загружается значение, лежащее по адресу (.L14 + 8) [0.000000001].
        vcvt.f64.s32    d17, s15 // в d17 записывается (double) s15 (преобразование одинарной точности в двойную) [разность наносекунд].
        vmov    s15, r3 @ int // в s15 копируется r3 [разность секунд].
        movw    r0, #:lower16:.LC3 // записывает в регистр r0 [0:15] (16 бит) 32-битного значения.
        movt    r0, #:upper16:.LC3 // записывает в регистр r0 [16:31] (16 бит) 32-битного значения.
        vcvt.f64.s32    d16, s15 // в d16 записывается (double) s15 (преобразование одинарной точности в двойную) [разность секунд].
        vmla.f64        d16, d17, d18 // перемножает регистры d17 и d18, добавляя результат в d16 [разность секунд += разность наносекунд * 0.000000001].
        vmov    r2, r3, d16 // в регистре r2 будут [0:31] биты d16, в r3 - [32:63] [разность наносекунд * 0.000000001].
        bl      printf // вызов функции printf.
.L9:
        movs    r0, #0 // в регистр r0 помещается значение 0 [код возврата из функции main].
        add     sp, sp, #16 // sp = sp + 16.
        vldm    sp!, {d8} // эквивалентно "VPOP {Registers}"; извлекает со стека регистр d8.
        pop     {r4, pc} // извлекает со стека два значения и помещает их в r4 и pc; затем sp += 8.
.L13:
        movw    r0, #:lower16:.LC1 // записывает в регистр r0 [0:15] (16 бит) 32-битного значения.
        movt    r0, #:upper16:.LC1 // записывает в регистр r0 [16:31] (16 бит) 32-битного значения.
        bl      puts // вызов функции puts.
        b       .L9 // переход на метку .L9.
.L12:
        movw    r0, #:lower16:.LC0 // записывает в регистр r0 [0:15] (16 бит) 32-битного значения.
        movt    r0, #:upper16:.LC0 // записывает в регистр r0 [16:31] (16 бит) 32-битного значения.
        bl      puts // вызов функции puts.
        b       .L9 // переход на метку .L9.
.L14:
        .word   14568529 // {это для 0.017453}
        .word   1066524467 // {это для 0.017453}

        .word   -400107883 // {это для 0.000000001}
        .word   1041313291 // {это для 0.000000001}
