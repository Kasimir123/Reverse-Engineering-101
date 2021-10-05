;--------------------------------------------------------------------
;
; Program that lets you test assembly a bit. This program will print 
; out whatever is in AX after your test code is run.
;
;---------------------------------------
         .model small                  ; 64k data and 64k code
         .8086                         ; only allow 8086 instructions
         .stack 256                    ; stack size is 256 bytes
;---------------------------------------

         .data
;---------------------------------------
codes    db        '0123456789ABCDEF'  ; codes of ASCII encoded hex digits
;---------------------------------------

         .code
;---------------------------------------
; Establish addressability to the data segment.
; Initialize the work register.
; - bx is cleared to zero and will be used as
;   an index register in the conversion process
;---------------------------------------
start:                                 ;
         mov       bx,0                ; clear index register
;---------------------------------------
; Put whatever code you want to test here
;---------------------------------------





;---------------------------------------
; Below is used to output whatever is in AX
;---------------------------------------
output:                                ;
         mov       cx,ax               ; moves ax into cx
         mov       ax,@data            ; establish addressability
         mov       ds,ax               ; for the data segment
         mov       bx,0                ; clear index register
;---------------------------------------
; Convert each character represented by a
; hex byte into two printable ASCII characters.
; Example: character = 'A' = 41h.
; Output as ASCII '4' and ASCII '1'.
;
; Get the printable version of the most
; significant hex digit and print it.
; Example: character = 'A' = 41h
; Output and ASCII '4'
;---------------------------------------
; Get the printable version of the least
; significant hex digit and print it.
; Example: character = 'A' = 41h
; Output and ASCII '1'
;---------------------------------------
         mov       bl,ch               ; move the hex byte to bx (bx=00xy)
         shr       bx,1                ; isolate
         shr       bx,1                ;  the left
         shr       bx,1                ;   hex digit
         shr       bx,1                ;    bx=000x
         mov       dl,[codes+bx]       ; use bx as index into ASCII codes
         mov       ah,2                ; write the ASCII code
         int       21h                 ; how: set ah=2 and issue int 21h
;---------------------------------------
; Get the printable version of the least
; significant hex digit and print it.
; Example: character = 'A' = 41h
; Output and ASCII '1'
;---------------------------------------
         mov       bl,ch               ; move the hex byte to bx (bx=00xy)
         and       bx,000fh            ; isolate right hex digit (bx=000y)
         mov       dl,[codes+bx]       ; use bx as index into ASCII codes
         mov       ah,2                ; write the ASCII code
         int       21h                 ; how: set ah=2 and issue int 21h


         mov       bl,cl               ; move the hex byte to bx (bx=00xy)
         shr       bx,1                ; isolate
         shr       bx,1                ;  the left
         shr       bx,1                ;   hex digit
         shr       bx,1                ;    bx=000x
         mov       dl,[codes+bx]       ; use bx as index into ASCII codes
         mov       ah,2                ; write the ASCII code
         int       21h                 ; how: set ah=2 and issue int 21h
;---------------------------------------
; Get the printable version of the least
; significant hex digit and print it.
; Example: character = 'A' = 41h
; Output and ASCII '1'
;---------------------------------------
         mov       bl,cl               ; move the hex byte to bx (bx=00xy)
         and       bx,000fh            ; isolate right hex digit (bx=000y)
         mov       dl,[codes+bx]       ; use bx as index into ASCII codes
         mov       ah,2                ; write the ASCII code
         int       21h                 ; how: set ah=2 and issue int 21h
;---------------------------------------

;---------------------------------------
; The user typed a period so terminate.
;---------------------------------------
exit:                                  ;
         mov       ax,4c00h            ; set correct exit code in ax
         int       21h                 ; int 21h will terminate program
         end       start               ; execution begins at the label start
;---------------------------------------