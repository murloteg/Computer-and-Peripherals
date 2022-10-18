CalcSin:
        testl   %edi, %edi // логическое "И" для изменения флагов SF, OF и ZF.
        jle     .L4 // проверка флагов SF, OF и ZF. Если SF [флаг знака] != OF [флаг переполнения] или
                    // ZF = 1 [флаг нуля], то переход на метку .L4.
        movsd   .LC0(%rip), %xmm1 // nextMultiplier = 1.
        leal    1(%rdi,%rdi), %esi // esi = 1 + rdi + rdi.
        movl    $1, %eax // factorial = 1.
        movl    $1, %edx // signStatus = 1.
        pxor    %xmm3, %xmm3 // обнуление xmm3 [тут лежит result].
.L3:
        pxor    %xmm4, %xmm4 // обнуление xmm4.
        movapd  %xmm0, %xmm2 // перемещает значения из xmm0 [argument] в xmm2.
        leal    1(%rax), %ecx // в ecx копируется адрес (rax + 1).
        cvtsi2sdl       %eax, %xmm4 // преобразование factorial к типу double.
        addl    $2, %eax // factorial += 2.
        divsd   %xmm4, %xmm2 // argument [лежит в xmm2] делится на factorial [лежит в xmm4].
        pxor    %xmm4, %xmm4 // обнуление xmm4.
        cvtsi2sdl       %ecx, %xmm4 // преобразование нового factorial к типу double.
        mulsd   %xmm2, %xmm1 // xmm1 = xmm1 * xmm2 (nextMultiplier *= [argument / factorial]).
        pxor    %xmm2, %xmm2 // обнуление xmm2.
        cvtsi2sdl       %edx, %xmm2 // преобразование signStatus к типу double.
        negl    %edx // signStatus *= -1.
        mulsd   %xmm1, %xmm2 // xmm2 = xmm2 * xmm1 (signStatus *= nextMultiplier).
        addsd   %xmm2, %xmm3 // xmm3 = xmm3 + xmm2 (result += частичный результат).
        movapd  %xmm0, %xmm2 // перемещает значения из xmm0 [argument] в xmm2.
        divsd   %xmm4, %xmm2 // деление [argument] на [factorial].
        mulsd   %xmm2, %xmm1 // xmm1 = xmm1 * xmm2 (nextMultiplier *= [argument / factorial]).
        cmpl    %eax, %esi // сравнение [factorial] и [number].
        jne     .L3 // если они не равны, то переход на метку .L3.
        movapd  %xmm3, %xmm0 // иначе: перемещает значения из xmm3 [result] в xmm0.
        ret // возврат из подпрограммы.
.L4:
        pxor    %xmm3, %xmm3 // обнуление регистра xmm3 [result = 0].
        movapd  %xmm3, %xmm0 // перемещает значения из xmm3 [result] в xmm0.
        ret // возврат из подпрограммы.
.LC2:
        .string "too few arguments. try again"
.LC3:
        .string "too many arguments. try again"
.LC5:
        .string "%lf\n"
.LC7:
        .string "(clock_gettime) total time: %lf sec.\n"
main:
        pushq   %rbp // помещение регистра базы стекового фрейма на стек.
        pushq   %rbx // помещение регистра rbx на стек.
        subq    $56, %rsp // rsp = rsp - 56.
        cmpl    $2, %edi // сравнение значений edi [argc] и 2.
        jle     .L12 // если argc <= 2, то переход на метку .L12.
        cmpl    $3, %edi // сравнение edi [argc] и 3.
        jne     .L13 // если argc != 3, то переход на метку .L13.

        /* strtol */
        movq    8(%rsi), %rdi // иначе: в регистр rdi копируется значение, лежащее по адресу (rsi + 8) [argv[1]].
        movl    $10, %edx // в edx копируется значение 10.
        movq    %rsi, %rbx // в rbx копируется содержимое регистра rsi [argv[0]].
        xorl    %esi, %esi // обнуление esi.
        call    strtol

        /* strtod */
        movq    16(%rbx), %rdi // в регистр rdi копируется значение, лежащее по адресу (rbx + 16) [argv[2]];
        xorl    %esi, %esi // обнуление регистра esi.
        movq    %rax, %rbp // в регистр rbp копируется содержимое rax [результат strtol].
        call    strtod

        leaq    16(%rsp), %rsi // в rsi копируется адрес (rsp + 16) [адрес start_clock].
        movl    $4, %edi // в edi копируется 4 [CLOCK_MONOTONIC_RAW].
        movsd   %xmm0, 8(%rsp) // в значение, лежащее по адресу (rsp + 8), копируется xmm0 [результат strtod].
        call    clock_gettime
        movsd   8(%rsp), %xmm0 // в регистр xmm0 копируется [результат strtod] (неявное приведение к double).
        movl    %ebp, %edi // в регистр edi копируется ebp.
        mulsd   .LC4(%rip), %xmm0 // в регистр xmm0 записывается: xmm0 * 0.017453. [strtod(argv[2]) * 0.017453].
        call    CalcSin
        movl    $.LC5, %edi // в регистр edi копируется строковый литерал, лежащий по метке .LC5.
        movl    $1, %eax // в eax копируется 1 (кол-во аргументов неинтегрального типа в printf).
        call    printf
        movl    $4, %edi //  в edi копируется 4 [CLOCK_MONOTONIC_RAW].
        leaq    32(%rsp), %rsi // в rsi копируется адрес (rsp + 32) [адрес end_clock].
        call    clock_gettime
        movq    40(%rsp), %rax // в rax копируется значение, лежащее по адресу (rsp + 40) [end_clock.tv_nsec].
        pxor    %xmm0, %xmm0 // обнуление регистра xmm0.
        subq    24(%rsp), %rax // rax = rax - (rsp + 24) [тут лежит start_clock.tv_nsec].
        cvtsi2sdq       %rax, %xmm0 // преобразование разности end_clock.tv_nsec - start_clock.tv_nsec к типу double.
        pxor    %xmm1, %xmm1 // обнуление регистра xmm1.
        movq    32(%rsp), %rax // в rax копируется значение, лежащее по адресу (rsp + 32) [end_clock.tv_sec].
        subq    16(%rsp), %rax // rax = rax - (rsp + 16) [тут лежит start_clock.tv_sec].
        mulsd   .LC6(%rip), %xmm0 // разность наносекунд умножается на 0.000000001.
        cvtsi2sdq       %rax, %xmm1 // преобразование разности секунд к типу double.
        movl    $.LC7, %edi // в edi копируется строковый литерал, лежащий по адресу .LC7.
        movl    $1, %eax // в eax копируется 1 (кол-во аргументов неинтегрального типа в функции printf).
        addsd   %xmm1, %xmm0 // xmm0 = xmm0 + xmm1 (разность секунд + 0.000000001 * разность наносекунд).
        call    printf
.L9:
        addq    $56, %rsp // rsp = rsp + 56.
        xorl    %eax, %eax // обнуление регистра eax.
        popq    %rbx // извлечение регистра rbx со стека.
        popq    %rbp // извлечение регистра rbp со стека.
        ret // завершение работы подпрограммы.
.L13:
        movl    $.LC3, %edi // в edi копируется строковый литерал, лежащий по метке .LC3.
        call    puts
        jmp     .L9 // безусловный переход на метку .L9.
.L12:
        movl    $.LC2, %edi // в регистр edi копируется строковый литерал, лежащий по метке .LC2.
        xorl    %eax, %eax // обнуление регистра eax.
        call    printf
        jmp     .L9 // безусловный переход на метку .L9.
.LC0:
        .long   0
        .long   1072693248 // эта константа равна: (-1)^0 * 1.0000000000000000000000000000000000000000000000000000 * 2^(1023 - 1023) = 1.
.LC4:
        .long   14568529
        .long   1066524467 // эта константа равна: (-1)^0 * 1.0001110111110011001100000000110111100100110001010001 * 2^(1017 - 1023) = 0.017453.
.LC6:
        .long   -400107883
        .long   1041313291 // эта константа равна: (-1)^0 * 1.0001001011100000101111101000001001101101011010010101 * 2^(993 - 1023) = 0.000000001.
