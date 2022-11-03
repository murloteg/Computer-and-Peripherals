CalcSin:
        pushl   %ebp // помещение базы стекового фрейма на вершину стека.
        movl    %esp, %ebp // в регистр ebp копируется содержимое esp.
        subl    $40, %esp // esp = esp - 40.
        movl    12(%ebp), %eax // в eax копируется значение, лежащее по адресу (ebp + 12) [там лежали 4 байта argument].
        movl    16(%ebp), %edx // в edx копируется значение, лежащее по адресу (ebp + 16) [а тут - ещё 4 байта argument].
        movl    %eax, -40(%ebp) // в значение, лежащее по адресу (ebp - 40), копируется eax [4 байта argument].
        movl    %edx, -36(%ebp) // в значение, лежащее по адресу (ebp - 36), копируется edx [ещё 4 байта argument].
        fldz // st(0) = 0.
        fstpl   -8(%ebp) // сохранение вершины стека st(0) [т.е. 0] по адресу (ebp - 8) [result].
        movl    $1, -12(%ebp) // в значение, лежащее по адресу (ebp - 12), копируется 1 [signStatus].
        fld1 // st(0) = 1.
        fstpl   -24(%ebp) // сохранение вершины стека st(0) [т.е. 1] по адресу (ebp - 24) [nextMultiplier].
        movl    $1, -28(%ebp) // в значение, лежащее по адресу (ebp - 28), копируется 1 [factorial].
        movl    $0, -32(%ebp) // в значение, лежащее по адресу (ebp - 32), копируется 0 [i].
        jmp     .L2 // безусловный переход на метку .L2.
.L3:
        fildl   -28(%ebp) // загрузка операнда [factorial] на вершину стека (st(0) = [factorial]).
        fldl    -40(%ebp) // загрузка операнда [argument] на вершину стека (st(0) = [argument]).
        fdivp   %st, %st(1) // st(0) = st(0) / st(1).
        fldl    -24(%ebp) // загрузка операнда [nextMultiplier] на вершину стека (st(0) = [nextMultiplier]).
        fmulp   %st, %st(1) // st(0) = st(0) * st(1) [nextMultiplier *= (argument / factorial)].
        fstpl   -24(%ebp) // сохранение вершины стека st(0) по адресу (ebp - 24) [поместили nextMultiplier].
        addl    $1, -28(%ebp) // [factorial += 1].
        fildl   -12(%ebp) // загрузка операнда [signStatus] на вершину стека (st(0) = [signStatus]).
        fmull   -24(%ebp) // st(0) = st(0) * [nextMultiplier].
        fldl    -8(%ebp) // загрузка операнда [result] на вершину стека (st(0) = [result]).
        faddp   %st, %st(1) // st(0) = st(0) + st(1) (тут лежит signStatus * nextMultiplier).
        fstpl   -8(%ebp) // сохранение вершины стека st(0) по адресу (ebp - 8) [тут новое значение result].
        fildl   -28(%ebp) // загрузка операнда [factorial] на вершину стека (st(0) = [factorial]).
        fldl    -40(%ebp) // загрузка операнда [argument] на вершину стека (st(0) = [argument]).
        fdivp   %st, %st(1) // st(0) = st(0) / st(1).
        fldl    -24(%ebp) // загрузка операнда [nextMultiplier] на вершину стека (st(0) = [nextMultiplier]).
        fmulp   %st, %st(1) // st(0) = st(0) * st(1) [nextMultiplier *= (argument / factorial)].
        fstpl   -24(%ebp) // сохранение вершины стека st(0) по адресу (ebp - 24) [поместили nextMultiplier].
        addl    $1, -28(%ebp) // [factorial += 1].
        negl    -12(%ebp) // в (ebp - 12) помещается результат (0 - [signStatus]), т.е. signStatus *= (-1).
        addl    $1, -32(%ebp) // i += 1.
.L2:
        movl    -32(%ebp), %eax // в регистр eax копируется значение, лежащее по адресу (ebp - 32) [i].
        cmpl    8(%ebp), %eax // сравнение i с значением, лежащем по адресу (ebp + 8) [number].
        jl      .L3 // если i < number, то переход на метку .L3.
        fldl    -8(%ebp) // иначе: на вершину стека загружается [result] (st(0) = [result]).
        leave // сначала [movl %ebp, %esp] -> затем -> [pop %ebp]
        ret // завершение работы подпрограммы.
.LC3:
        .string "too few arguments. try again"
.LC4:
        .string "too many arguments. try again"
.LC6:
        .string "%lf\n"
.LC8:
        .string "(clock_gettime) total time: %lf sec.\n"
main:
        leal    4(%esp), %ecx // адрес (esp + 4) копируется в регистр ecx [адрес нулевого аргумента функции].
        andl    $-16, %esp // выравнивние адреса esp в направлении к вершине стека до ближайшего, который будет делится на 16.
        pushl   -4(%ecx) // на вершину стека помещается (ecx - 4) [адрес возврата из функции].
        pushl   %ebp // на вершину стека помещается база стекового фрейма ebp.
        movl    %esp, %ebp // в регистр ebp копируется содержимое esp.
        pushl   %ebx // на вершину стека помещается ebx.
        pushl   %ecx // на вершину стека помещается ecx (то есть argc).
        subl    $48, %esp // esp = esp - 48.
        movl    %ecx, %ebx // в регистр ebx копируется содержимое ecx (argc).
        cmpl    $2, (%ebx) // сравнение значения, лежащего по адресу ebx, с числом 2.
        jg      .L6 // если argc > 2, то переход к метке .L6.
        subl    $12, %esp // иначе: esp = esp - 12.
        pushl   $.LC3 // на вершину стека помещается строковый литерал .LC3.
        call    puts // его вывод в стандартный поток вывода stdout.
        addl    $16, %esp // esp = esp + 16.
        movl    $0, %eax // в регистр eax кладется значение 0 (индикатор завершения работы подпрограммы).
        jmp     .L9 // безусловный переход на метку .L9.
.L6:
        cmpl    $3, (%ebx) // сравнение значения, лежащего по адресу (ebx), с числом 3.
        jle     .L8 // если (ebx) <= 3, то переход на метку .L8.
        subl    $12, %esp // иначе: esp = esp - 12.
        pushl   $.LC4 // на вершину стека кладется строковый литерал .LC4.
        call    puts // его вывод в стандартный поток вывода stdout.
        addl    $16, %esp // esp = esp + 16.
        movl    $0, %eax // в регистр eax копируется значение 0 (индикатор завершения работы подпрограммы).
        jmp     .L9 // безусловный переход к метке .L9.
.L8:
        movl    4(%ebx), %eax // в регистр eax копируется значение, лежащее по адресу (ebx + 4) (то есть argv[0]).
        addl    $4, %eax // адресная арифметика: переход к указателю на argv[1].

        /* strtoll */
        movl    (%eax), %eax // разыменование указателя.
        subl    $4, %esp // esp = esp - 4.
        pushl   $10 // на вершину стека кладется значение 10.
        pushl   $0 // на вершину стека кладется значение 0.
        pushl   %eax // на вершину стека кладется agrv[1].
        call    strtol // вызов функции strol для трех верхних параметров, лежащих на стеке.
        addl    $16, %esp // esp = esp + 16.
        movl    %eax, -12(%ebp) // в значение, лежащее по адресу (ebp - 12) копируется содержимое eax [number].

        /* strtod */
        movl    4(%ebx), %eax // в регистр eax копируется значение, расположенное по адресу (ebx + 4).
        addl    $8, %eax // адресная арифметика: переход к указателю на argv[2].
        movl    (%eax), %eax // разыменование указателя.
        subl    $8, %esp // esp = esp - 8.
        pushl   $0 // на вершину стека кладется значение 0.
        pushl   %eax // на вершину стека кладется argv[2].
        call    strtod // вызов функции strtod для двух верхних параметров, лежащих на стеке.
        addl    $16, %esp // esp = esp + 16.
        fstpl   -24(%ebp) // вытолкнули st(0) из стека сопроцессора и положили по адресу (ebp - 24) (argument).

        subl    $8, %esp // esp = esp - 8.
        leal    -32(%ebp), %eax // в регистр eax копируется адрес (ebp - 32).
        pushl   %eax // на вершину стека кладется eax.
        pushl   $4 // на вершину стека кладется значение 4.
        call    clock_gettime // вызов функции clock_gettime для верхних параметров, лежащих на стеке.
        addl    $16, %esp // esp = esp + 16.
        fldl    -24(%ebp) // в st(0) помещается (ebp - 24) (то есть argument).
        fldl    .LC5 // в st(0) помещается константа, лежащая по метке .LC5 (0.017453).
        fmulp   %st, %st(1) // st(0) = st(0) * st(1).
        leal    -8(%esp), %esp // в esp копируется адрес (esp - 8).
        fstpl   (%esp) // вытолкнули st(0) из стека сопроцессора и положили по адресу (esp) [тут лежит argument * 0.017453].
        pushl   -12(%ebp) // на вершину стека кладется (ebp - 12) [number].
        call    CalcSin // вызов функции CalcSin для number и (argument * 0.017453).
        addl    $16, %esp // esp = esp + 16.
        subl    $4, %esp // esp = esp - 4.
        leal    -8(%esp), %esp // в esp копируется адрес (esp - 8).
        fstpl   (%esp) // сохранение вершины стека st(0) по адресу (esp) [результат].
        pushl   $.LC6 // на вершину стека кладется строковый литерал, лежащий по метке .LC6.
        call    printf
        addl    $16, %esp // esp = esp + 16.
        subl    $8, %esp // esp = esp - 8.
        leal    -40(%ebp), %eax // в eax копируется адрес (ebx - 40) [адрес start_clock].
        pushl   %eax // на вершину стека кладется eax.
        pushl   $4 // на вершину стека кладется CLOCK_MONOTONIC_RAW.
        call    clock_gettime // вызов функции для аргументов CLOCK_MONOTONIC_RAW и &start_clock.
        addl    $16, %esp // esp = esp + 16.
        movl    -40(%ebp), %edx // в регистр edx копируется значение, лежащее по адресу (ebp - 40) [end_clock.tv_sec].
        movl    -32(%ebp), %eax // в регистр eax копируется значение, лежащее по адресу (ebp - 32) [start_clock.tv_sec].
        subl    %eax, %edx // в регистр edx помещается результат: end_clock.tv_sec - start_clock.tv_sec
        movl    %edx, -44(%ebp) // этот результат копируется в значение, лежащее по адресу (ebp - 44).
        fildl   -44(%ebp) // загрузка [результата] на вершину стека (st(0) = [result_1]).
        movl    -36(%ebp), %edx // в регистр edx копируется значение, лежащее по адресу (ebp - 36) [end_clock.tv_nsec].
        movl    -28(%ebp), %eax // в регистр edx копируется значение, лежащее по адресу (ebp - 28) [start_clock.tv_nsec].
        subl    %eax, %edx // в регистр edx помещается результат: end_clock.tv_nsec - start_clock.tv_nsec.
        movl    %edx, -44(%ebp) // этот результат копируется в значение, лежащее по адресу (ebp - 44).
        fildl   -44(%ebp) // загрузка [результата] на вершину стека (st(0) = [result_2]).
        fldl    .LC7 // загрузка константы 0.000000001 на вершину стека (st(0) = 0.000000001).
        fmulp   %st, %st(1) // st(0) = st(0) * st(1), потом st(1) = st(0), извлечение st(0) => в st(0) лежит 0.000000001 * result_2.
        faddp   %st, %st(1) // st(0) = st(0) + st(1), st(0) = 0.000000001 * result_2 + result_1.
        subl    $4, %esp // esp = esp - 4.
        leal    -8(%esp), %esp // в esp копируется адрес (esp - 8).
        fstpl   (%esp) // сохранение вершины стека st(0) по адресу (esp) [итоговое время работы].
        pushl   $.LC8 // на вершину стека помещается строковый литерал, лежащий по метке .LC8.
        call    printf
        addl    $16, %esp // esp = esp + 16.
        movl    $0, %eax // в регистр eax копируется значение 0 (индикатор завершения работы подпрограммы).
.L9:
        leal    -8(%ebp), %esp // в esp копируется адрес (esp - 8).
        popl    %ecx // извлечение ecx со стека.
        popl    %ebx // извлечение ebx со стека.
        popl    %ebp // извлечение ebp со стека.
        leal    -4(%ecx), %esp // в esp копируется адрес (esp - 4) [значение esp до выравнивания до 16 байт].
        ret // завершение работы подпрограммы.
.LC5:
        .long   14568529
        .long   1066524467 // эта константа равна: (-1)^0 * 1.0001110111110011001100000000110111100100110001010001 * 2^(1017 - 1023) = 0.017453.
.LC7:
        .long   -400107883
        .long   1041313291 // эта константа равна: (-1)^0 * 1.0001001011100000101111101000001001101101011010010101 * 2^(993 - 1023) = 0.000000001.
