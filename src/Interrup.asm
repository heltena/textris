; Declaracions públiques

public _Inhibir
public _Desinhibir
public _EOI
public _Salvar
public _Restaurar
Public _CanviarVectInt
Public _InhibirIRQ
Public _DesinhibirIRQ

_DATA segment word public 'DATA'
_DATA ends

_BSS segment word public 'BSS'
_BSS ends

DGROUP group _DATA, _BSS

assume cs: INTERRUP_ASM, ds: DGROUP

INTERRUP_ASM segment byte public 'CODE'

; +----------+
; | _Inhibir |
; +----------+

_Inhibir proc far
        pushf                 ; Apilo els flags
        pop ax                 ; Desempilo els flags FICANT-LOS A AX
        cli                  ; Inhibir interrupcions DON'T DISTURB !!!!
        ret                  ; Retorno (amb AX els flags)
_Inhibir endp



; +-------------+
; | _Desinhibir |
; +-------------+

_Desinhibir_Param struc
                   dw ?   ; Bp
                   dd ?   ; Adreça de retorn
 _Desinhibir_Flags dw ?   ; Flags que ens pasen per paràmetre
_Desinhibir_Param ends

_Desinhibir proc far
        push bp                       ; Agafar paràmetres ??? Co~o
        mov bp, sp                     ; A fer l'enllaç
        push ax                       ; No vull canviar el valor d'AX
        mov ax, [bp+_Desinhibir_Flags]   ; Ax <- Flags a posar
        push ax                       ; Fico a la pila el valor del paràmetre
        popf                         ; Recopero de la pila ELS FLAGS
        pop ax                       ; Aquí no pasa res
        pop bp                       ; o~oC ??? sertemàrap rafagA
        ret                       ; Adeu
_Desinhibir endp



; +------+
; | _EOI |
; +------+

_EOI proc far
        push ax       ; El valor insignificant d'AX no el toco
        mov al, 20h    ; aquest 20h es el valor que l'hi donaré al i8259
        out 20h, al   ; EEooooohhhhh!!!! Controlador d'interrupcioooóons!!!!
        pop ax       ; Pila, dona'm el valor que t'he donat.
        ret       ; Gut vait
_EOI endp



; +---------+
; | _Salvar |
; +---------+

_Salvar proc far
        push bx               ; Poso BX
        mov bx, sp             ; BX <- Sp
        xchg cx, ss:[bx+2]     ; Canvio la posició on és l'IP per CX
        xchg dx, ss:[bx+4]     ; Canvia la posició on és el CS per DX
        mov bx, ss:[bx]        ; Recopero BX sense treure'l de la pila
        push ax               ; poso Ax

        push Si Di Ss Es Ds

        push ax
        mov ax, DGROUP         ; Canviem DS pel grup DGROUP
        mov ds, ax
        pop ax

        push dx               ; Poso CS | Jé, ara el ret es creu que la
        push cx               ; Poso IP | pila queda com estava. Incrédul.
        ret
_Salvar endp



; +------------+
; | _Restaurar |
; +------------+

_Restaurar proc far
        pop cx                ; Agafo l'IP  | Això és de l'adreça de retorn
        pop dx                ; Agafo el CS | " " "   "  "   "

        pop Ds Es Ss Di Si

        pop ax                ; També Ax
        mov bx, sp             ; Un enllaç dinàmic una mica "cutre"
        xchg cx, ss:[bx+2]     ; Canvio Cx pel seu valor anterior (CS)
        xchg dx, ss:[bx+4]     ; Canvio Dx pel seu valor anterior (IP)
        pop bx                ; Recuperar Bx
        mov sp, bp             ; Desfaig l'enllaç de la RSI
        pop bp                ; Ara sembla que això sigui la fi de la RSI
        iret                ; retorna, però enganyat! Já, Jaaá, Jaaaá!!!
_Restaurar endp



; +-----------------+
; | _CanviarVectInt |
; +-----------------+

_CanviarVectInt_Param struc
                        dw ?   ; Bp
                        dd ?   ; Adreça de retorn
 _CanviarVectInt_NumInt dw ?   ;
  _CanviarVectInt_Desp dw ?   ;
  _CanviarVectInt_Segm dw ?   ;
_CanviarVectInt_Param ends

_CanviarVectInt proc far
        push bp               ; Muntar enllaç dinàmic
        mov bp, sp             ; Segona part de la saga començada a ip-2
        push bx               ; No vull canviar-lo
        push es              ; Aquest tampoc

        xor bx, bx            ; Segment zero ...
        mov es, bx             ; ... a ES (Vector d'interrupcions)
        xor ax, ax            ;
        mov al, byte ptr [bp+_CanviarVectInt_NumInt]  ; A Al la interrupció a modificar

        shl ax, 1            ; Multiplico Ax per 2
        shl ax, 1            ; i per 2 o sigui, Ax per 4
        mov bx, ax             ; Poso a Bx <- NumInt * 4

        pushf
        cli              ; Don't disturb me, please.
        mov dx, [bp+_CanviarVectInt_Segm]  ; 'Ax' Segment de la nova inter.
        xchg es:[bx+2], dx            ; El canvio pel que hi ha
        mov ax, [bp+_CanviarVectInt_Desp]  ; Ara el desplacament
        xchg es:[bx], ax            ; El canvio pel que hi ha
        popf               ; Admetre altre cop interrupcions si estaven
                                      ; permeses (posar els flags com estaven)

        pop es               ; Desmuntar tot el que he muntat
        pop bx               ; recuperar segments
        pop bp               ; desmuntar enllaç dinàmic
        ret               ; retorno amb Dx:Ax l'antiga adreça
_CanviarVectInt endp



; +-------------+
; | _InhibirIRQ |
; +-------------+

_InhibirIRQ_Param struc
                 dw ? ; bp
                 dd ? ; Adreça de retorn
 _InhibirIRQ_irq dw ? ; Paràmetre
_InhibirIRQ_Param ends

_InhibirIRQ proc far
        push bp
        mov bp, sp     ; El co~o d'enllaç dinàmic
        push ax
        push bx
        push cx

        mov cl, byte ptr [bp+_InhibirIRQ_irq] ; cl <- Número d'interrupció
        mov bl, 1      ; Començem a construir la màscara
        shl bl, cl    ; Només haurà un ú al bit passat com a paràmetre

        pushf        ; Guardar flags
        cli        ; Desinhibir interrupcions
        in al, 21h    ; Demanar registre de màscares d'interrupcions (IMR)
        or al, bl    ; Inhibir la interrupció que volem
        out 21h, al    ; Oido barra!!!
        popf        ; Recuperar flags avanç de la desinhibització.

        pop cx
        pop bx
        pop ax
        pop bp        ; Desmuntar l'enllaç (estació d'~)
        ret        ; Hadióx
_InhibirIRQ endp



; +----------------+
; | _DesinhibirIRQ |
; +----------------+

_DesinhibirIRQ_Param struc
                    dw ? ; bp
                    dd ? ; Adreça de retorn
 _DesinhibirIRQ_irq dw ? ; Paràmetre
_DesinhibirIRQ_Param ends

_DesinhibirIRQ proc far
        push bp
        mov bp, sp
        push ax
        push bx
        push cx

        mov cl, byte ptr [bp+_InhibirIRQ_irq] ; cl <- Número d'interrupció
        mov bl, 1      ; Començem a construir la màscara
        shl bl, cl    ; Només haurà un ú al bit passat com a paràmetre
        not bl       ; Només haurà un zero al bit passat com a paràmetre

        pushf        ; Guardar flags
        cli        ; Desinhibir interrupcions
        in al, 21h    ; Demanar registre de màscares d'interrupcions (IRM)
        and al, bl    ; Desinhibir la interrupció que volem
        out 21h, al    ; Oido barra!!!
        popf        ; Recuperar flags avanç de la desinhibització.

        pop cx
        pop dx
        pop ax
        pop bp
        ret
_DesinhibirIRQ endp
INTERRUP_ASM ends
        end    ; final de fitxer
