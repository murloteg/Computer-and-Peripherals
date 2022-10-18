CalcSin:
        pushl   %ebx
        subl    $8, %esp
        movl    16(%esp), %eax
        fldl    20(%esp)
        testl   %eax, %eax
        jle     .L4
        fld1
        leal    1(%eax,%eax), %ebx
        movl    $1, %edx
        movl    $1, %eax
        fldz
.L3:
        movl    %eax, 4(%esp)
        fildl   4(%esp)
        leal    1(%eax), %ecx
        addl    $2, %eax
        movl    %edx, 4(%esp)
        negl    %edx
        fdivr   %st(3), %st
        fmulp   %st, %st(2)
        fildl   4(%esp)
        movl    %ecx, 4(%esp)
        fmul    %st(2), %st
        faddp   %st, %st(1)
        fildl   4(%esp)
        fdivr   %st(3), %st
        fmulp   %st, %st(2)
        cmpl    %eax, %ebx
        jne     .L3
        fstp    %st(1)
        fstp    %st(1)
        addl    $8, %esp
        popl    %ebx
        ret
.L4:
        fstp    %st(0)
        fldz
        addl    $8, %esp
        popl    %ebx
        ret
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
        pushl   -4(%ecx) // // на вершину стека помещается (ecx - 4) [адрес возврата из функции].
        pushl   %ebp // на вершину стека помещается база стекового фрейма ebp.
        movl    %esp, %ebp // в регистр ebp копируется содержимое esp.
        pushl   %esi // помещение esi на вершину стека.
        pushl   %ebx // помещение ebx на вершину стека.
        pushl   %ecx // помещение ecx на вершину стека [argc].
        subl    $44, %esp // esp = esp - 44 (резервирование места на стеке под локальные переменные).
        movl    (%ecx), %eax // в eax копируется значение, лежащее по адресу (ecx) [argc].
        movl    4(%ecx), %ebx // в ebx копируется значение, лежащее по адресу (ecx + 4) [argv].
        cmpl    $2, %eax // сравнение argc с 2.
        jle     .L13 // если argc <= 2, то переход на метку .L13.
        cmpl    $3, %eax // сравнение argc с 3.
        jne     .L14 // если argc != 3, то переход на метку .L14.

        /* strtol */
        pushl   %eax // помещение eax на вершину стека [тут будет результат strtol].
        pushl   $10 // помещение 10 на вершину стека.
        pushl   $0 // помещение 0 на вершину стека.
        pushl   4(%ebx) // помещение argv[1] на вершину стека.
        call    strtol

        /* strtod */
        popl    %edx // извлечение edx со стека.
        popl    %ecx // извлечение ecx со стека.
        pushl   $0 // помещение 0 на вершину стека.
        pushl   8(%ebx) // помещение argv[2] на вершину стека.
        movl    %eax, %esi // в esi копируется значение eax [результат strtol].
        call    strtod
        fstpl   -48(%ebp) // вытолкнули st(0) из стека FPU и положили по адресу (ebp - 48) [argument].

        popl    %ebx // извлечение ebx со стека.
        popl    %eax // извлечение eax со стека.
        leal    -40(%ebp), %eax // в eax копируется адрес (ebp - 40) [адрес start_clock].
        pushl   %eax // поместили адрес start_clock на вершину стека.
        pushl   $4 // поместили CLOCK_MONOTONIC_RAW на вершину стека.
        call    clock_gettime
        fldl    -48(%ebp) // поместили argument в st(0).
        fmull   .LC5 // перемножили st(0) [argument] и константу 0.017453.
        movl    %esi, (%esp) // в значение, лежащее по адресу (esp), скопировали esi [number].
        fstpl   4(%esp) // извлекли st(0) со стека FPU и поместили по адресу (esp + 4) [argument * 0.017453].
        call    CalcSin
        movl    $.LC6, (%esp)
        fstpl   4(%esp)
        call    printf
        popl    %eax
        leal    -32(%ebp), %eax
        popl    %edx
        pushl   %eax
        pushl   $4
        call    clock_gettime
        movl    -28(%ebp), %eax
        subl    -36(%ebp), %eax
        movl    %eax, -48(%ebp)
        movl    -32(%ebp), %eax
        subl    -40(%ebp), %eax
        fildl   -48(%ebp)
        movl    %eax, -48(%ebp)
        fmull   .LC7
        fildl   -48(%ebp)
        movl    $.LC8, (%esp)
        faddp   %st, %st(1)
        fstpl   4(%esp)
        call    printf
        addl    $16, %esp
.L10:
        leal    -12(%ebp), %esp // в esp копируется адрес (ebp - 12).
        xorl    %eax, %eax // обнуление регистра eax.
        popl    %ecx // извлечение ecx со стека.
        popl    %ebx // извлечение ebx со стека.
        popl    %esi // извлечение esi со стека.
        popl    %ebp // извлечение ebp со стека.
        leal    -4(%ecx), %esp // в esp копируется адрес (ecx - 4).
        ret // завершение работы подпрограммы.
.L14:
        subl    $12, %esp // esp = esp - 12 (резервирование места под локальные переменные).
        pushl   $.LC4 // помещение на вершину стека строкового литерала, лежащего по метке .LC4.
        call    puts
        addl    $16, %esp // esp = esp + 16.
        jmp     .L10 // безусловный переход на метку .L10.
.L13:
        subl    $12, %esp // esp = esp - 12 (резервирование места под локальные переменные).
        pushl   $.LC3 // помещение на вершину стека строкового литерала, лежащего по метке .LC3.
        call    puts
        addl    $16, %esp // esp = esp + 16.
        jmp     .L10 // безусловный переход на метку .L10.
.LC5:
        .long   14568529
        .long   1066524467 // эта константа равна: (-1)^0 * 1.0001110111110011001100000000110111100100110001010001 * 2^(1017 - 1023) = 0.017453.
.LC7:
        .long   -400107883
        .long   1041313291 // эта константа равна: (-1)^0 * 1.0001001011100000101111101000001001101101011010010101 * 2^(993 - 1023) = 0.000000001.
