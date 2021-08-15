/*
 ==============================================================================================================================
 Name    		: 	TP1 Enunciado A
 Author  		:   Grupo 2
 Modificaciones	:   ---
 Version 		:   5.0.7
 Description 	:	Crea lista de facturas de cada vehículo capturado que haya entrado y salido de la autopista.
 	 	 	 		Para ello, lee un archivo. Este archivo consta en la primera línea del número de casos de prueba
 	 	 	 		a realizar. A continuación, cada caso de prueba consta de una línea con el valor de la tarifa
 	 	 	 		cada uno se corresponde con una hora determinada. O sea, si el vehículo ingreso entre las 00:00 hs
 	 	 	 		y las 00:59 hs le corresponde 1 céntimo de dólar candiense y así sucesivamente.Luego de la línea
 	 	 	 		de tarifas están los registros de las matrículas/patentes de los vehículos capturados por las
 	 	 	 		cámaras. Dichos registros están compuestos por: la matrícula, la fecha del registro, si entró o salió
 	 	 	 		y el kilómetro donde se registró.
 	 	 	 		El propietario del vehículo recibirá una factura cuyo monto será: por cada km recorrido a una tarifa
 	 	 	 		de la hora en que ingreso a la autopista, más un dólar por viaje y más dos dólares de cargo a la cuenta.
 	 	 	 		Para esto, el código leerá el número de casos de prueba, generará una matriz con las tarifas
 	 	 	 		correspondientes, leerá los registros de cada caso de prueba los ordenará y generará las facturas
 	 	 	 		a aquellos que hayan entrado y salido de la autopista, imprimirá por pantalla la matrícula del vehículo
 	 	 	 		y el importe de su factura. Una vez finalizada la impresión de las facturas libera el espacio de memoria
 	 	 	 		de la lista de registros y de facturas/cuentas para el siguiente caso de prueba.
 ==============================================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Libreria para el arreglo de tarifas.
 * Funciones para llenar y mostrar por pantalla las tarifas.
 */
#include "Tarifa.h"

/*
 * Librería para el arreglo de registros.
 * Funciones para:
 * Definición de la estructura registro.
 * Agregar registros al arreglo.
 * Liberar el espacio de memoria asignado.
 * Oredenar el arreglo de manera alfanumérica.
 */
#include "Registro.h"

/*
 * Librería para la lista de facturas.
 * Funciones:
 * Definición del nodo Cuenta.
 * Definición de la estructura lista.
 * Crear nodo.
 * Crear lista.
 * Agregar nodo.
 * Modificar nodo.
 * Borrar nodo de la lista.
 * Imprimir lista enlazada.
 */
#include "Cuenta.h"

/*
 * Su principal función es la de controlar que el archivo abierto presente el formato correcto.
 * Pero, además, guardará en una matriz dinámica la cantidad de registros capturados por las cámaras.
 */
int Control(int cantPruebas, FILE *ptr, int *posR);

int main(){

	//Abrimos el archivo de texto
	FILE *ptr = fopen("Prueba.txt","r");

	//Comprobamos que el archivo se haya abierto correctamente
	if(ptr == NULL){
	    	perror("Error en la apertura del archivo\n");
	    	return -1;
	}

	unsigned int num_pruebas;

	//Lectura de cuantas pruebas se hayan en el archivo txt
	fscanf(ptr, "%u", &num_pruebas);

	//Asignamos espacio de memoria para saber donde termina cada caso de prueba
	int *posR;

	if(num_pruebas > 1)
		posR = (int*) malloc(num_pruebas*sizeof(int));
	else
		posR = (int*) malloc(sizeof(int));

	/*
	 * En caso de que el archivo de texto presente más o menos casos de prueba que lo indicado en la primera línea
	 * o haya más de mil (1000) registros en alguno de los casos de prueba
	 * se finalizará la ejecución del código.
	 * Y consideramos que en cada caso de prueba hay al menos un registro de las cámaras como mínimo
	 * se finalizará la ejecución del código.
	 */
	if(Control(num_pruebas, ptr, posR) == -1){
		return 0;
	}

	//Conozco la distribucion del archivo por enunciado asi que desplazo el cursor hasta el numero
	fseek(ptr, 4, SEEK_CUR);

	/*
	 * Creamos un arreglo de 24 elementos ya que la línea de tarifas es la misma en cada caso de prueba.
	 * Por lo que podemos usarla para cada caso de prueba.
	 */
	Tarifa tar[Total_Tarifas];

	//Cargamos los valores de las tarifas en la matriz
	LlenarMTarifa(tar, ptr);

	/*
	 * Esta función imprime por pantalla la matriz de tarifas.
	 * ImprimirMTarifa(tar);
	 * puts("\n\n=================================================\n");
	 */

	//Variable de control para finalizar la función recursiva do.
	int Control = 0;

	//Asignamos espacio de memoria para las listas de registros de las camaras "lr" y la lista de las cuentas de las patentes "lc".
	Registro *lr = NULL;

	Cuentas *lc = CrearLibroCuentas();

	/*
	 * Podemos usar un for o do para realizar todo el proceso.
	 * El do finalizará cuando se haya alcanzado la cantidad de caso de prueba leídos.
	 */
	do{
		lr = (Registro*) malloc(posR[Control]*sizeof(Registro));

		LlenarARegistro(lr, ptr, posR[Control]);

		/*
		 * Muestra por pantalla la lista de registros cargada
		 * ImprimirRegistros(lr, posR[Control]);
		 * puts("\n\n=================================================");
		 */

		//Ordenamos la lista de registros en orden alfanumerico y fecha
		OrdenarRegistros(lr, posR[Control]);

		/*
		 * Muestra por pantalla la lista de registros cargada.
		 * ImprimirRegistros(lr, posR[Control]);
		 * puts("\n=================================================\n");
		 */

		/*
		 * Con esta funcion creamos una lista de las cuentas que cumplan los requisitos enunciados en el tp.
		 * En el caso de que no se cumplan la lista permanecerá vacía ( el inicio de la lista apunta a NULL).
		 */
		RegistroCuentas(lc, lr, tar, Control, posR[Control]);

		//Mostramos por pantalla el listado de cuentas que hay en el caso de prueba presente
		ImprimirCuentas(num_pruebas, Control, lc);

		/*
		 * Vaciamos el arreglo de estructuras y la lista enlazada.
		 * Liberamos el espacio de memoria reservado al array de registro; y
		 * Eliminando (liberar espacio de memoria reservado) los nodos de la lista de cuentas hasta que quede vacía.
		 */
		LiberarRegistros(lr, posR[Control]);

		while(lc->inicio != NULL){
			LiberarCuentas(lc);
		}
		Control++;
		if(Control != num_pruebas)
			//Movemos la posición en el archivo al primer registro del siguiente caso de prueba.
			fseek(ptr, 68, SEEK_CUR);
	}while(Control < num_pruebas);

	//A continuacíón se procede a liberar el espacio de memoria asignado.
	free(posR);

	//Liberamos el espacio de memoria reservada para la lista de cuentas
	free(lc);

	//Cierre del archivo
	fclose(ptr);

	return EXIT_SUCCESS;
}

int Control(int cantPruebas, FILE *ptr, int *posR){

	//Con esta variable contamos la cantidad de registros por caso de prueba
	int nroRegistros;

	//Con esta variable contamos la cantidad de pruebas en el archivo
	int cant=0;

	//Posición para la matriz posR
	int i=0;

	//Índice
	int PruebaNro;

	//Guardamos la posición actual en el archivo para volver a éste al finalizar el proceso de control
	int posActual = ftell(ptr);

	fseek(ptr, 6, SEEK_SET);

	//Usamos la función recursiva "for" la cantidad de veces que diga la primera línea del archivo de texto
	for(PruebaNro = 0; PruebaNro < cantPruebas; PruebaNro++){

		//Seteamos el numero de registros a -1, ya que al leer tambien la linea de tarifas nos sumaria un registro mas y no es asi
		nroRegistros = -1;

		//Controlamos que el numero de registros leidos por caso de prueba sea menor o igual a 1000 y hasta que el archivo finalice
		while(nroRegistros <= 1000 && !feof(ptr)){
			if(fgetc(ptr) == '\n'){
				nroRegistros++;
				if(fgetc(ptr) == '\n'){
					cant++;
					//Guardamos la cantidad de registros que hay en el caso i-ésimo
					posR[i] = nroRegistros;
					//Alguna de las prueba y/o la última no contiene ningún registro de las cámaras.
					if(nroRegistros == 0){
						puts("No hay registros en el caso. Es necesario que haya al menos uno.\n");
						return -1;
					}
					//Al momento de contar las lineas sumamos la linea de tarifas por lo que al contarla queda en cero esta variable y con ello logramos un correcto conteo de los registros del caso de prueba siguiente.
					nroRegistros=-1;
					i++;
					//Para salir del caso de prueba actual
					break;
				}
			}else{
				//Prueba en la cual el último caso no contenga ningún registro de las cámaras.
				if(nroRegistros == -1 && feof(ptr)){
					puts("No hay registros en el caso. Es necesario que haya al menos uno.\n");
					return -1;
				}
				if(feof(ptr)){
					//Número de registros que hay. Dado que no hay un salto de línea hay que sumarle uno (1) para que de el valor correcto.
					posR[i] = nroRegistros + 1;
				}
			}
		}
		if(nroRegistros > 1000){
			puts("Hay más de 1000 registros en uno o más casos de prueba. Modifique el archivo.\n");
			return -1;
		}
	}

	//Dado que la cuenta de la cantidad de veces que registro un caso de prueba está restado menos uno lo sumamos al finalizar el proceso antes del control
	cant++;

	//Comprobamos que no haya más o menos casos de prueba que lo indicado en el archivo de texto
	if(cant != cantPruebas){
		puts("El número de casos de prueba indicado no se corresponde con los que hay presentes en el archivo.\n");
		return -1;
	}

	//Volvemos a la posición dentro del archivo antes del control realizado ya que este está en el final de archivo
	fseek(ptr, posActual, SEEK_SET);

	return 0;
}
