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
        pushl   %ecx // помещение ecx на вершину стека.
        subl    $44, %esp
        movl    (%ecx), %eax
        movl    4(%ecx), %ebx
        cmpl    $2, %eax
        jle     .L13
        cmpl    $3, %eax
        jne     .L14
        pushl   %eax
        pushl   $10
        pushl   $0
        pushl   4(%ebx)
        call    strtol
        popl    %edx
        popl    %ecx
        pushl   $0
        pushl   8(%ebx)
        movl    %eax, %esi
        call    strtod
        fstpl   -48(%ebp)
        popl    %ebx
        popl    %eax
        leal    -40(%ebp), %eax
        pushl   %eax
        pushl   $4
        call    clock_gettime
        fldl    -48(%ebp)
        fmull   .LC5
        movl    %esi, (%esp)
        fstpl   4(%esp)
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
        leal    -12(%ebp), %esp
        xorl    %eax, %eax
        popl    %ecx
        popl    %ebx
        popl    %esi
        popl    %ebp
        leal    -4(%ecx), %esp
        ret
.L14:
        subl    $12, %esp
        pushl   $.LC4
        call    puts
        addl    $16, %esp
        jmp     .L10
.L13:
        subl    $12, %esp
        pushl   $.LC3
        call    puts
        addl    $16, %esp
        jmp     .L10
.LC5:
        .long   14568529
        .long   1066524467
.LC7:
        .long   -400107883
        .long   1041313291