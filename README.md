Informe:

Trabajo Práctico de laboratorio

Organización y Arquitectura de computadoras

Comisión 4

Matias Nicolas Hernandez

DNI: 45327037



Aplicación de calculadora simple en C sobre microcontrolador ATmega328p (Arduino Uno)


![image](https://github.com/user-attachments/assets/775b2b1d-ca08-4bd4-a3b2-43b2066c1ee1)


Introducción:

  En una primera instancia, en base a los componentes disponibles, me surgió la idea de implementar una calculadora. Por más que su funcionamiento en principio es sencillo, el llevarlo a cabo tanto en la comprensión del funcionamiento de los componentes a utilizar, como en la implementación de la lógica del código, proponía un reto interesante.

  
Integración de periféricos:


La implementación de la calculadora se basa en la utilización del display lcd y el teclado. Ambos componentes ya los conocía como también el funcionamiento general del microcontrolador y el lenguaje C, pero en un principio no recordaba todo, por lo que comenzar fue con todo todo esto fue bastante más difícil que la expectativa.

Tanto los videos del campus como la búsqueda de información fueron cruciales al comienzo, ya que si bien en el kit dado por la cátedra ya tenía conexiones con la placa arduino, terminar de entender los componentes a utilizar fue la primera gran barrera.


Display LCD


![image](https://github.com/user-attachments/assets/c5cf44cd-392b-44e5-a3a7-90b9fbc0d9a0)


El display puede funcionar con 4 u 8 bits, en este caso, ya que los pines de 7 a 10 están sin utilizar, podemos dar cuenta de que se trabajará con 4 bits en el display.

Los pines 1 a 3, VSS, VDD y VEE, son de alimentación  del componente, cada uno debe ir conectado a su alimentación correspondiente para el funcionamiento. 

Los pines 3 a 6 RS, RW y E, son pines de control, los cuales habilitan o controlan funcionalidades.


Teclado Matricial


![image](https://github.com/user-attachments/assets/083531f8-8292-48b7-b326-e902e4513596)


El teclado matricial es aún más fácil de comprender, cada tecla conecta  una determinada fila con una determinada columna.

Los pines del teclado deben conectarse a pines del microcontrolador que puedan funcionar como entradas digitales, de modo que definiendo una matriz en el código se pueda determinar el número u operación correspondiente a cada tecla. 

En síntesis, cada tecla presionada, es información que el microcontrolador recibe y puede utilizar.





Programacion:

Bibliotecas Utilizadas

	
	#include <Keypad.h>
	#include <LiquidCrystal.h>


Keypad.h: Permite gestionar un teclado matricial (keypad) con filas y columnas conectadas a pines digitales. Facilita la lectura de teclas presionadas mediante un mapeo.

LiquidCrystal.h: Maneja pantallas LCD de tipo HD44780. Proporciona funciones como begin(), print(), clear(), y setCursor() para controlar el contenido del display.

Inicialización del LCD:


	LiquidCrystal lcd(1, 0, A1, A2, 10, 9);


Se declara un objeto lcd que define la conexión a la pantalla.

Los parámetros son los pines conectados al LCD.      

El LCD tendrá 2 filas y 16 columnas.

Configuración del Teclado Matricial


	const byte ROWS = 4;
	const byte COLS = 4;

Se especifica un teclado de 4 filas x 4 columnas (16 teclas en total).


	char keys[ROWS][COLS] = {
	  {'7','8','9','D'},
	  {'4','5','6','M'},
	  {'1','2','3','S'},
	  {'C','0','E','A'}
	};

Se define el mapa de teclas según su disposición física en el teclado.


Leyenda:
	
	C: Clear, Limpiar
 
	E: equal, Igual
  
	D: DIV, División
 
	M: MUL, Multiplicación
 
	S: SUB, Restar
 
	A: ADD, Suma
	 

Se define la conexión física del teclado con los pines del microcontrolador.

	
	byte rowPins[ROWS] = {12,11,8,7};
	byte colPins[COLS] = {6,5,4,2};
	Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);



Se crea el objeto keypad utilizando la función makeKeymap().

Variables Globales

	
	String num1 = "";
	String num2 = "";
	String lastResult = "";
	bool currentNumber = false;
	bool error = false;
	char operation = ' ';


num1: Guarda el primer número ingresado.

num2: Guarda el segundo número (tras ingresar una operación).

lastResult: Guarda el último resultado para reutilizar.

currentNumber: Indica si estamos ingresando num2 (true) o num1 (false).

operation: Guarda la operación aritmética seleccionada ('+', '-','/','*').

Error: Bandera de error (por ejemplo, división por cero). Esto es para no poder usar el resultado anterior en caso de que este sea un error.


 Función setup()

 
	void setup() {
	  lcd.begin(16, 2);
	}


Inicializa el LCD en modo 16x2 (16 columnas, 2 filas).


Función Principal loop()


	void loop() {
	  char key = keypad.getKey();
	Se consulta si se presionó una tecla.


Manejo de Teclas Especiales

	if (key) {
	    switch (key) {



Se evalúa la tecla presionada:


 Tecla C - Clear (Reset)

      case 'C':
        lcd.clear();
        reset();
        lastResult = "";
        return;
      break;


Limpia el display.

Llama a reset() para reiniciar las variables (num1, num2, operation, etc.).

También borra el último resultado.

Termina el loop() para evitar procesamiento adicional.


Operadores Aritméticos ('D', 'M', 'S', 'A')


      case 'D':
      case 'M':
      case 'S':
      case 'A':



Se muestra num1:

          lcd.clear();
          lcd.print(num1);
          operation = key;
          currentNumber = true;

y se setea currentNumber a true para poder ingresar el segundo número
Luego se imprime el símbolo de operación:


          switch (key) {
            case 'D': lcd.print(" % "); break;
            case 'M': lcd.print(" * "); break;
            case 'S': lcd.print(" - "); break;
            case 'A': lcd.print(" + "); break;
          }




Tecla “=”


      case 'E':
        if (operation == ' ' || num1 == "" || num2 == "") return;

 
Verifica si la operación es válida (no vacía), y si existe tanto num1 como num2.


        int number1 = num1.toInt();
        int number2 = num2.toInt();
        int result = 0;
        error = false;

Convierte los operandos a enteros


Realiza el cálculo según la operación:


        switch (operation) {
          case 'M':
            result = number1 * number2;
            break;
          case 'A':
            result = number1 + number2;
            break;
          case 'S':
            result = number1 - number2;
            break;
          case 'D':
            if (number2 != 0)
              result = number1 / number2;
            else
              error = true;
            break;
        }

Se muestra el resultado o mensaje de error en la segunda línea del LCD:


        lcd.setCursor(0, 1);
        if (error) {
          lcd.print("Error");
          lastResult = "";
        } else {
          lcd.print(result);
          lastResult = String(result);
        }
        reset();
        break;

Se llama a reset() al finalizar para reiniciar los operandos.


	Ingreso de Números (0–9)
	    switch (key) {
	      case '0'...'9':
	        if (!currentNumber) {
	          error = false;
	          num1 += key;
	          lastResult = num1;
	          lcd.clear();
	          lcd.print(num1);
	        } else {
	          num2 += key;
	          lcd.print(key);
	        }
	        break;
	    }
	  }
	}


Se evalúa si la tecla ingresada es un dígito.

Si aún no hay operación seleccionada (currentNumber == false), se agrega a num1 y se actualiza el display.

Si ya se seleccionó operación, se agrega el número a num2 y se imprime directamente.

Función Auxiliar reset()


	void reset() {
	  num1 = "";
	  num2 = "";
	  operation = ' ';
	  currentNumber = false;
	}


Restablece el estado interno de la calculadora, excepto lastResult


Resumen General del Funcionamiento:

Al iniciar, se ingresan números que forman num1.

Al presionar una tecla de operación (A, S, M, D), se espera un segundo número (num2).

Al presionar E, se realiza el cálculo y se muestra el resultado en la segunda línea del LCD.

Si se presiona C, se reinicia todo el sistema.

El último resultado puede reutilizarse en operaciones siguientes.

Más adelante desarrollo estas funciones con más profundidad.



En un principio llevé en cuenta, evitar funcionalidades u operaciones demasiado complejas. Pero el funcionamiento simple al que apunté funciona correctamente y da lugar a una calculadora funcional y cómoda para el usuario.


El funcionamiento final, nos permite sumar, restar, multiplicar y dividir, sólo un par de números, es decir, no se puede realizar una operación como “3 + 4 + 2”, pero en caso que se quiera realizar la misma operación, sí se puede aprovechar la funcionalidad de operar con la respuesta anterior, de modo que si realizamos  primero “3 + 4” y luego “+ 2”, cumplirá con la operación “3 + 4 + 2”.

La tecla on/C nos permite “limpiar”  el display, tanto también como los números registrados por el microcontrolador a nivel código. Cómo agregado, si se intenta dividir por 0, nos da un mensaje de error.

Entre las limitaciones que tenemos en la calculadora simple, además de que no se pueden hacer operaciones de más de una operación aritmética en un solo paso, como explicamos  anteriormente. También tenemos que si quiere realizar por ejemplo, una multiplicación, y por accidente tocás el botón de suma, esto no se puede revertir, se debe limpiar todo el display(con el botón clear) y escribir de nuevo.

De la mano de esta limitación encontramos que no podemos realizar  multiplicaciones o divisiones con números negativos.
Pero sí se puede conseguir este funcionamiento, no directamente. Si obtenemos dicho número negativo como resultado de una operación, luego podremos operar con dicho resultado, de manera correcta con cualquier operación. Como última limitación, o funcionamiento para mejorar mejor dicho, tenemos que sólo trabajamos con números enteros, de modo que muchas divisiones son inexactas.

Todas estas limitaciones, requieren una programación y lógica más compleja, pero para nada imposible. Me parece importante remarcarlas no solo para tener en consideración el funcionamiento, sino también para ser consciente de posibles mejoras que no solo corrijan, sino que también amplíen las funcionalidades. 
