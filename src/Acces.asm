; Declaracions públiques

public _Peek
public _Poke
public _InPort
public _OutPort

_DATA segment word public 'DATA'
_DATA ends

_BSS segment word public 'BSS'
_BSS ends

DGROUP group _DATA, _BSS

assume cs: ACCES_ASM, ds: DGROUP

ACCES_ASM segment byte public 'CODE'

; +------+
; | Peek |
; +------+

_Peek_Param Struc
        Dw ?        ; Bp
        Dd ?        ; Adreça de retorn
 _Peek_Desp_PTR Dw ?        ; Desplaçament del Punter del paràmetre
 _Peek_Segm_PTR Dw ?        ; Segment del Punter del paràmetre
_Peek_Param EndS

_Peek proc far
    push bp             ; BP es modificarà
    mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals
    push bx
    push es

    mov bx, [bp+_Peek_Segm_PTR] ; Llegeix el segment pasat per paràmetre
    mov es, bx          ; El fico a ES
    mov bx, [bp+_Peek_Desp_PTR] ; Llegeix el desplaçamet pasat per paràmetre
    mov al, es:[bx]     ; Al registre 'Al' fico el resultat (Byte)

    pop es
    pop bx
    pop bp              ; recupera BP anterior
    ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_Peek endp



; +------+
; | Poke |
; +------+

_Poke_Param Struc
        Dw ?        ; Bp
        Dd ?        ; Adreça de retorn
 _Poke_Desp_PTR Dw ?        ; Desplaçament del Punter del paràmetre
 _Poke_Segm_PTR Dw ?        ; Segment del Punter del paràmetre
 _Poke_Valor Dw ?        ; Només utilitza el byte de menys pes
_Poke_Param EndS

_Poke proc far
    push bp             ; BP es modificarà
    mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals
    push ax
    push bx
    push es

    mov bx, [bp+_Poke_Segm_PTR] ; Llegeix el segment pasat per paràmetre
    mov es, bx          ; El fico a ES
    mov bx, [bp+_Poke_Desp_PTR] ; Llegeix el desplaçamet pasat per paràmetre
    mov al, byte ptr [bp+_Poke_Valor] ; Llegeix el valor a ficar a Es:[Bx]
      ; Sense Byte ptr dona: Operand types do not match
    mov es:[bx], al     ; Escric el resultat a memòria

    pop es
    pop bx
    pop ax
    pop bp              ; recupera BP anterior
    ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_Poke endp


; +--------+
; | InPort |
; +--------+

_InPort_Param Struc
        Dw ?        ; Bp
        Dd ?        ; Adreça de retorn
 _InPort_Port Dw ?        ; Port on s'ha de llegir
_InPort_Param EndS

_InPort proc far
    push bp             ; BP es modificarà
    mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals
    push dx

    mov dx, [bp+_InPort_Port]    ; A 'Dx' port a llegir
    in al, dx           ; Llegeixo un Byte

    pop dx
    pop bp              ; recupera BP anterior
    ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_InPort endp



; +---------+
; | OutPort |
; +---------+

_OutPort_Param Struc
        Dw ?        ; Bp
        Dd ?        ; Adreça de retorn
 _OutPort_Port Dw ?        ; Port on s'ha de llegir
 _OutPort_Valor Dw ?        ; Valor a posar. Només el byte baix
_OutPort_Param EndS

_OutPort proc far
    push bp             ; BP es modificarà
    mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals
    push ax
    push dx

    mov dx, [bp+_OutPort_Port]    ; A 'Dx' port a escriure
    mov al, byte ptr [bp+_OutPort_Valor] ; Dono el valor del paràmetre a Al.
      ; Si no es posa Byte ptr dona error de tipus
    out dx, al          ; Escric un byte

    pop dx
    pop ax
    pop bp              ; recupera BP anterior
    ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_OutPort endp

ACCES_ASM ends

    end    ; final de fitxer
