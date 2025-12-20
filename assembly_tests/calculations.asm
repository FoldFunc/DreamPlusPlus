section .data

  EXIT_CODE db 0
  SYSCALL db 60

  var1 db 10
  var2 db 30

  addsum db 0

  subsum db 0

  mulsum dw 0

  divsum db 0
  divsumrest db 0

  mulsignedsum2 dw 0
  mulsignedsum1 dw 0

  incsum db 0
  decsum db 0

  carryvar1 ddq 0x1A000000000000000
  carryvar2 ddq 0x2C000000000000000
  carrysum  ddq 0

section .text

global _start
_start:

addition:
  mov al, byte [var1]
  add al, byte [var2]
  mov byte [addsum], al
  xor al, al

increment:
  mov al, byte [var1]
  inc al
  jmp zero
  mov byte [incsum], al
  xor al, al

addcarry:

  mov rax, qword [carryvar1]
  mov rbx, qword [carryvar1 + 8]

  add rax, qword [carryvar2]
  adc rbx, qword [carryvar2 + 8]
  
  mov qword [carrysum], rax
  mov qword [carrysum + 8], rbx
  
  xor rax, rax
  xor rbx, rbx

substract:
  mov al, byte [var2]
  sub al, byte [var1]
  mov byte [subsum], al

  xor al, al

decriment:
  mov al, byte [var1]
  dec al
  mov byte [decsum], al

mulitply:
  mov al, byte [var1]
  mul byte[var2]
  mov word [mulsum], ax
  mov word [mulsum+2], dx
  xor al, al
  xor ax, ax
  xor dx, dx

signedmul1:
  mov al, byte [var1]
  imul al, -13
  mov word [mulsignedsum1], ax
  xor al, al
  xor ax, ax

signedmul2:
  mov al, byte [var1]
  imul al, byte [var2]
  mov word [mulsignedsum2], ax
  xor al, al
  xor ax, ax

; There is also a idiv instruction but I am lazy and it's the same as imul.
divide:
  mov al, byte [var2]
  mov ah, 0
  mov bl, byte [var1]
  div bl
  mov byte [divsum], al
  mov byte [divsumrest], ah
  xor al, al
  xor ah, ah
  xor bl, bl

end:
  mov rax, byte [SYSCALL]
  mov rdi, byte [EXIT_CODE]
  syscall
