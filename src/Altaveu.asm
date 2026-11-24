; Declaracions públiques

public _So      ; només cal posar el nom de la variable o funció

_DATA  segment word public 'DATA'
_DATA  ends

_BSS   segment word public 'BSS'
_BSS   ends
DGROUP  group  _DATA, _BSS

assume cs:_ALTAVEU_ASM, ds:DGROUP

_ALTAVEU_ASM segment byte public 'CODE'

_TParam_So Struc
        Dw  ?  ; Bp
        Dd  ?  ; Adreça de Retorn
 _So_frequencia Dw  ?  ; Frecuencia
_TParam_So EndS

_So proc far
    push bp    ; BP es modificarà
    mov bp, sp  ; enllaç dinàmic per accedir params. i vars. locals
    push ax
    push dx

    mov ax, [bp+_So_frequencia]
    cmp ax, 0
    jne _So_Programar_timer

    in al, 61h  ; Agafo el port 61h
           ;      mov dx, 61h
           ;      in al, dx   ; Agafo el port 61h
    and al, 0FCh ; Desactivo el timer 2 y el speaker data
    Out 61h, al ; i li dic
           ;      Out dx, al  ; i li dic
    Jmp _So_Final ; Adeu

_So_Programar_timer:
    mov dx, 43h  ; Programar el timer 2
    mov al, 0B6h ; Tipus d'ona
    out dx, al   ; Oido barra
    dec dx       ; El registre 42h
    mov ax, [bp+_So_frequencia]
    out dx, al   ; Part baixa de la frecuencia
    xchg al, ah  ; Canvio al per ah i viceversa
    out dx, al   ; Part alta de la frecuencia

    mov dx, 61h
    in al, dx   ; Agafo el port 61h
    or al, 3    ; Activar el speaker data y el timer 2
    out dx, al  ; Començar a pitar

_So_Final:
    pop dx
    pop ax
    pop bp     ; recupera BP anterior
    ret        ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_So endp

_ALTAVEU_ASM ends

    end    ; final de fitxer
