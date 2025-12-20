section .data

  ; #################### ; 
  ; Exit variables for easier closing of a program; 

  SYSCALL_EXIT equ 60

  ; #################### ; 
  ; Declare some basic variables for addition ;

  EXIT_CODE db 0
  bVar1 db 10
  bVar2 db 15
  bRes  db 0 ; Placeholder not a real value

section .text

global _start
_start:

  ; #################### ; 
  ; EXIT_CODE = bVar1 + bVar2
calc:
  mov al, byte [bVar1]
  add al, byte [bVar2]
  mov byte [EXIT_CODE], al


; #################### ; 
; Terminate a program

end:
  mov rax, SYSCALL_EXIT
  movzx edi, byte [EXIT_CODE] ; movzx beacouse it extends 1 byte EXIT_CODE to 8 bytes
  syscall
