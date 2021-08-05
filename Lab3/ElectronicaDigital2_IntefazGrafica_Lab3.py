'''------------------------------------------------------------------------------
Autor: Fernando Caceros
Programa: Laboratorio #3
Creado: 5 de agosto de 2021    
Intefaz gráfica para el laboratorio de comunicacion SPI
-------------------------------------------------------------------------------'''

'''------------------------------------------------------------------------------
-------------------------IMPORTAR LIBRERIAS--------------------------------------
------------------------------------------------------------------------------'''
import tkinter as tk            #se importa libreria de GUI
from tkinter import *           #se importa libreria de GUI
import serial                   #se importa libreria de comunicacion serial
import time
'''------------------------------------------------------------------------------
-----------------------DEFINICION DE OBJETOS------------------------------------
------------------------------------------------------------------------------'''
root = Tk()                     #se le da nombre al objeto principal

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE PUERTO SERIAL-------------------------------
------------------------------------------------------------------------------'''
#DEFINICION DE PUERTO SERIAL
dato=serial.Serial()        #declarar puerto serial y braudeaje
dato.baudrate = 9600  # set Baud rate to 9600
dato.bytesize = 8     # Number of data bits = 8
dato.parity   ='N'    # No parity
dato.stopbits = 1     # Number of Stop bits = 1

dato.timeout=3
dato.port='COM5'    #Se define puerto
dato.open()    
root.counter = 0                

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE FUNCIONES-----------------------------------
------------------------------------------------------------------------------'''
#se define funcion para sumar
#def suma():                                          
#    root.counter += 1
#    L['text'] = 'Contador : ' + str(root.counter)
#    dato.write(b'+')   #se manda + en ascii
#    time.sleep(0.1)
#    dato.write(b'\r')

#se define funcion para restar
#def resta():                                          
#    root.counter -= 1
#    L['text'] = 'Contador : ' + str(root.counter)
#    dato.write(b'-')   #se manda - en ascii
#    time.sleep(0.1)
#    dato.write(b'\r')

#se define funcion para enviar
def enviar():                                          
    a = (entry.get())
    #print(a)
    b = a[0]
    #print(b)
    c = bytes(b, "utf-8")
    dato.write(c)   #se manda - en ascii
    entry.delete(0,tk.END)

#se define función para refresh
def refresh():
    dato.reset_input_buffer()   #Se limpia el buffer
    potenciometros['text']=dato.read_until(b'\r', 24)   #Se lee hasta el enter 
    potenciometros['text']=dato.read_until(b'\r', 24)   #Se lee nuevamente para que enseñe todos los datos
    root.after(100, refresh) 

 
    
'''------------------------------------------------------------------------------
----------------------------CUERPO DE INTERFAZ-----------------------------------
------------------------------------------------------------------------------'''
#TITULO
titulo=tk.Label(root,text = "GUI para laboratorio 3, Electrónica Digital 2") #texto como titulo de GUI
titulo.place(x=90, y=20)

#titulo de la ventana
root.title("GUI Lab3, Electronica Digital 2")   #le pones titulo al objeto
root.minsize(400,300)                           #le decis el tamaño a la ventana

#texto para indicar que putas hacen los botones
label = tk.Label(root, text = "El primer caracter será representado en su forma binaria de su codigo ascii")        #texto para el cuadro de texto
label.place(x=2,y=50)

# Crear caja de texto.
entry = tk.Entry(root, width=7)
# Posicionarla en la ventana.
entry.place(x=150, y=80)

#boton de suma
#b1 = Button(root, text="Suma", command=suma)
#b1.place(x=150, y=75)

#boton de enviar
b2 = Button(root, text="Enviar", command=enviar)
b2.place(x=200,y=75)

#boton de refresh
#b3 = Button(root, text="Refresh", command=refresh)
#b3.place(x=175, y=180)



#POTENCIOMETROS
potenciometros=tk.Label(root, text=dato.read_until(b'\r', 24))
potenciometros.place(x=135, y=150)
print(dato.read_until(b'\r', 24))   #Lee hasta el enter
L = Label(root, text="Contador: 0")                      
L.pack()

root.after(1, refresh)

root.mainloop( )     #Main loop


