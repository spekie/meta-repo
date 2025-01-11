section .data
    prompt db "input: ", 0
    fmt db "%d", 0

section .bss
    num resd 1

section .text
    extern printf, scanf
    global _start

_start:
    mov rdi, prompt
    call printf

    mov rdi, fmt
    mov rsi, num
    call scanf

    mov rdi, prompt
    call printf

    mov rdi, fmt
    mov rsi, [num]
    call printf

    mov rax, 60
    xor rdi, rdi
    syscall
