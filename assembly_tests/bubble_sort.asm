section .data
  arr dd 4, 3, 2, 5
  arrlen db 4

section .text

global _start

_start:
    movzx rcx, byte [arrlen]
    xor rbx, rbx

outer_loop:
    cmp rbx, rcx
    jge end

    xor rdx, rdx
    mov rsi, rcx
    sub rsi, rbx
    dec rsi

inner_loop:
    cmp rdx, rsi
    jge skip_inner

    mov eax, [arr + rdx*4]
    mov edi, [arr + (rdx+1)*4]
    cmp eax, edi
    jle no_swap

    mov [arr + rdx*4], edi
    mov [arr + (rdx+1)*4], eax

no_swap:
    inc rdx
    jmp inner_loop

skip_inner:
    inc rbx
    jmp outer_loop

end:
    mov eax, 60
    xor edi, edi
    syscall

