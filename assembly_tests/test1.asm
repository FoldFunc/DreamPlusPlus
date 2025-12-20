; comment
section .data ; initialized data 
  bytevar db 128 ; byte variable
  charvar db "A" ; character variable
  strvar  db "Hello , world" ; string variable
  fltvar  dd 3.14159 ; a 32-bit floating point number
section .bss ; uninitialized data
  arr resb 100 ; 100 element array of bytes
section .text
global _start
_start:
  SIZE equ 1000
  mov rax, 0xa
  mov rsp, 0
  ret
