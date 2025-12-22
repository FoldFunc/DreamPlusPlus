section .data
  arr dd 1, 2, 3, 4
  arrlen db 4
  SYSCALL_CODE db 60
  EXIT_CODE db 0

section .text
global _start

_start:
    mov cl, byte [arrlen]
    xor rbx, rbx

loopbody:
    cmp rbx, rcx
    jge endloop

    mov eax, [arr + rbx*4]
    imul eax, eax          
    mov [arr + rbx*4], eax  

    inc rbx
    jmp loopbody

endloop:
    movzx rax, byte [SYSCALL_CODE]
    movzx rdi, byte [EXIT_CODE]
    syscall

