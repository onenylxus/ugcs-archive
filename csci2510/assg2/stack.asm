; stack implementation
; possible input formats
;	opcode number
;	opcode
; opcode may be:
;	0: exit: exit the program
;	1: push: push a number at the top, error if the stack is full
;	2: pop: pop a number from the top, error if the stack is empty
;	3: size: check how many numbers are in the stack
;	4: clear: remove all pushed numbers, no error on clearing an empty stack
;	5; data: display the content of the stack
; constraints
;	number >= 0 and number is an 32-bit integer
; possible errors and error codes
;	-1: the stack is empty
;	-2: the stack is full

; use 80386 instruction set
.386 
; flat: only flat model is supported under Win32 program
; stdcall: calling convension, parameter passed from right to left (as if in a stack)
.model flat, stdcall
; the language is case insensitive
option casemap:none
; header files
include windows.inc
include kernel32.inc
include user32.inc
include msvcrt.inc
; lib files
includelib msvcrt.lib
includelib ucrtd.lib


; data segment
.data
; format string for a single integer
input_int_format_string db "%d", 0
; in windows the return symbol is CRLF (CR: 13 + LF: 10)
output_int_format_string db "%d", 13, 10, 0
; jump table for different operation
stack_optable dd stack_exit, stack_push, stack_pop, stack_size, stack_clear, stack_data
; reserved memory for reading the operation code
stack_opcode dd 0
; reserved memory for reading the input number
stack_input_number dd 0
; how many numbers could be stored in the stack
stack_capacity dd 8
; the numbers in the stack is stored here
stack_store dd 8 dup(0)
; stack bottom end
stack_bottom dd 0


; code segment
.code
start:
		jmp stack_clear


stack_input:
		invoke crt_scanf, addr input_int_format_string, addr stack_opcode
		cmp stack_opcode, 1
		jne input_no_number
		invoke crt_scanf, addr input_int_format_string, addr stack_input_number
	input_no_number:
		mov eax, [stack_opcode]
		mov eax, [stack_optable + eax*4]
		jmp eax


stack_exit:
		invoke ExitProcess, NULL


; basic mission: decrease the stack top  
stack_push:
		;;; exercise 2: check whether the stack is full
		;;; your code here (around 2 lines)
		; 1. compare the top pointer with the top end of the stack
		; 2. if full, execute the code inside push_full
		;		otherwise jump to push_not_full
		cmp ebp, offset stack_store
		jne push_not_full
		
	push_full:
		;;; exercise 2: output the correct error code
		;;; your code here (around 2 lines)
		; 1. call the printf function with error code -2
		; 2. turn to read the next input
		invoke crt_printf, addr output_int_format_string, -2
		jmp stack_input
		
	push_not_full:
		;;; exercise 1: push
		;;; hint: you may want to read the input number which is stored at stack_input_number
		;;; your code here (around 3 lines)
		; 1. allocate a slot for the number to be pushed
		; 2. (a register may be needed to avoid direct copy between memory)
		; 3. store the number inside the slot
		sub ebp, 4
		mov eax, [stack_input_number]
		mov [ebp], eax
		jmp stack_input


; remeber that people enter a stack at the top end of a stack (stack_store)
;	and leave a stack also from the top end (stack_store)
; basic mission: increase the stack top pointer 
stack_pop:
		;;; exercise 2: check whether the stack is empty
		;;; your code here (around 2 lines)
		; 1. compare the top pointer with the bottom end of the stack
		; 2. if empty, execute the code inside pop_empty
		;		otherwise jump to pop_not_empty
		cmp ebp, offset stack_bottom
		jne pop_not_empty
		
	pop_empty:
		;;; exercise 2: output the correct error code
		;;; your code here (around 2 lines)
		; 1. call the printf function with error code -1
		; 2. turn to read the next input
		invoke crt_printf, addr output_int_format_string, -1
		jmp stack_input
		
		
	pop_not_empty:
		;;; exercise 1: pop
		;;; your code here (around 3 lines)
		; 1. remove the top number from its slot
		; 2. (a register may be needed to avoid direct copy between memory)
		; 3. deallocate its slot on the stack
		mov eax, [ebp]
		add ebp, 4
		invoke crt_printf, addr output_int_format_string, eax
		jmp stack_input


stack_size:
		;;; bonus exercise: compute the size of the stack
		;;;				notice that empty stack is not considered an error here
		;;; your code here
		mov eax, offset stack_bottom
		sub eax, ebp
		mov ebx, 4
		idiv ebx
		invoke crt_printf, addr output_int_format_string, eax
		jmp stack_input


stack_clear:
		mov ebp, offset stack_bottom
		jmp stack_input


stack_data:
		mov esi, ebp
	data_loop:
		; check whether the stack is empty
		cmp ebp, offset stack_bottom
		jne data_loop_not_empty
		; handle empty
		jmp data_restore_ebp
	data_loop_not_empty:
		mov eax, [ebp]
		add ebp, 4
		invoke crt_printf, addr output_int_format_string, eax
		jmp data_loop
	data_restore_ebp:
		mov ebp, esi
		jmp stack_input

end start