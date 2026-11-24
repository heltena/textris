; Declaracions públiques

public _Ptr
public _Segment
public _Desplacament
public _Word
public _MSB
public _LSB

_DATA segment word public 'DATA'
_DATA ends

_BSS segment word public 'BSS'
_BSS ends

DGROUP  group  _DATA, _BSS

assume cs: CONVTIPS_ASM, ds: DGROUP

CONVTIPS_ASM segment byte public 'CODE'

; +-----+
; | Ptr |
; +-----+

_Ptr_Param Struc
                Dw ?        ; Bp
                Dd ?        ; Adreça de retorn
 _Ptr_Segm_PTR Dw ?        ; Segment del Punter del paràmetre
 _Ptr_Desp_PTR Dw ?        ; Desplaçament del Punter del paràmetre
_Ptr_Param EndS

_Ptr proc far
        push bp             ; BP es modificarà
        mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals

        mov dx, [bp+_Ptr_Segm_PTR] ; Llegeix el segment pasat per paràmetre DX
        mov ax, [bp+_Ptr_Desp_PTR] ; Llegeix el desplaçamet pasat per paràmetre AX

        pop bp              ; recupera BP anterior
        ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_Ptr endp



; +---------+
; | Segment |
; +---------+

_Segment_Param Struc
                        Dw ?        ; Bp
                        Dd ?        ; Adreça de retorn
 _Segment_Desp_PTR Dw ?        ; Desplaçament del Punter del paràmetre
 _Segment_Segm_PTR Dw ?        ; Segment del Punter del paràmetre
_Segment_Param EndS

_Segment proc far
        push bp             ; BP es modificarà
        mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals

        mov ax, [bp+_Segment_Segm_PTR] ; Llegeix el segment pasat per
                                       ; paràmetre a AX

        pop bp              ; recupera BP anterior
        ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_Segment endp



; +--------------+
; | Desplacament |
; +--------------+

_Desplacament_Param Struc
                             Dw ?  ; Bp
                             Dd ?  ; Adreça de retorn
 _Desplacament_Desp_PTR Dw ?  ; Desplaçament del Punter del paràmetre
 _Desplacament_Segm_PTR Dw ?  ; Segment del Punter del paràmetre
_Desplacament_Param EndS

_Desplacament proc far
        push bp             ; BP es modificarà
        mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals

        mov ax, [bp+_Desplacament_Desp_PTR] ; Llegeix el desplaçament pasat
                                            ; per paràmetre a AX

        pop bp              ; recupera BP anterior
        ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_Desplacament endp



; +------+
; | Word |
; +------+

_Word_Param Struc
            Dw ?  ; Bp
            Dd ?  ; Adreça de retorn
 _Word_MSB Dw ?  ; Part alta del word
 _Word_LSB Dw ?  ; Part baixa del word
_Word_Param EndS

_Word proc far
        push bp             ; BP es modificarà
        mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals

        mov ah, byte ptr [bp+_Word_MSB] ; A 'Ah' la part alta
        mov al, byte ptr [bp+_Word_LSB] ; A 'Al' la part baixa

        pop bp              ; recupera BP anterior
        ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_Word endp



; +-----+
; | MSB |
; +-----+

_MSB_Param Struc
            Dw ?  ; Bp
            Dd ?  ; Adreça de retorn
 _MSB_Word Dw ?  ; Paràmetre Word
_MSB_Param EndS

_MSB proc far
        push bp             ; BP es modificarà
        mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals

                        ; mov ax, [bp+_MSB_Word] ;
                        ; mov al, ah           ; A 'Al' part alta del Word del paràmetre
        mov al, byte ptr [bp+_MSB_Word+1] ; A 'Al' part alta del Word del paràmetre

        pop bp              ; recupera BP anterior
        ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_MSB endp



; +-----+
; | LSB |
; +-----+

_LSB_Param Struc
            Dw ?  ; Bp
            Dd ?  ; Adreça de retorn
 _LSB_Word Dw ?  ; Paràmetre Word
_LSB_Param EndS

_LSB proc far
        push bp             ; BP es modificarà
        mov bp, sp          ; enllaç dinàmic accedir params. i vars. locals

        mov al, byte ptr [bp+_MSB_Word] ; A 'Al' part baixa del Word del paràmetre

        pop bp              ; recupera BP anterior
        ret                 ; retorna. NO S'HAN DE TREURE ELS PARAMETRES !!!
_LSB endp

CONVTIPS_ASM ends

        end    ; final de fitxer
