section .data

  EXIT_CODE db 0
  SYSCALL_CODE db 60
  
section .text

global _start

_start:

narrow:
  ; Narrowing conversion
  mov rax, 500
  ;mov byte [EXIT_CODE], al ; To much data for a al register so value get's trimmed.

wide_unsign:
  ; Widening conversion
  ; Can also be achived with movzx instruction showed in end label
  mov al, 40
  mov rbx, 0 ; setting bigger register to zeros so there s no value in upper most bytes
  mov bl, al
  ;movzx byte [EXIT_CODE], rbx

wide_sign:
  ; Widening sign conversion
  mov cl, -10
  movsx rcx, cl
  mov byte [EXIT_CODE], cl

end:
  xor rax, rax
  xor rdi, rdi
  movzx rax, byte [SYSCALL_CODE]
  movzx rdi, byte [EXIT_CODE]
  syscall
