section .data

  SYSCALL_CODE db 60
  EXIT_CODE db 0
  
  var1 db 0x4f
  var2 db 0x20

section .text 

global _start

_start:
  mov al, byte [var1]

  and al, byte [var2]
  xor al, al

  mov al, byte [var1]
  or al, byte [var2]
  xor al, al

  mov al, byte [var1]
  xor al, byte [var2]
  xor al, al

  mov al, byte [var1]
  not al
  xor al, al

  mov al, byte [var1]
  shl al, 1
  xor al, al

  mov al, byte [var1]
  shr al, 1
  xor al, al
 
  mov al, byte [var1]
  sal al, 1
  xor al, al

  mov al, byte [var1]
  sar al, 1
  xor al, al

  mov al, byte [var1]
  rol al, 1
  xor al, al

  mov al, byte [var1]
  ror al, 1
  xor al, al

end:
  movzx rax, byte [SYSCALL_CODE]
  movzx rdi, byte [EXIT_CODE]
  syscall
