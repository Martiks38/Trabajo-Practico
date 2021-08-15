/*
 * Cuenta.h
 *
 *  Created on: 24 abr. 2021
 *      Author: mar13
 */

#ifndef CUENTA_H_
#define CUENTA_H_

/*
 * Dentro de cuenta guardamos la patente que cumple las condiciones necesarias para hacer un importe.
 * El numero de viajes que realizó durante el mes.
 * El importe de la distancia recorrida en la autopista.
 */
typedef struct Nodo{
	char *Patente;
	int nroViajes;
	float importerecorrido;
	struct Nodo *sgt;
}Cuenta;

//Lista de cuentas
typedef struct ListaCuenta{
	Cuenta *inicio;
}Cuentas;


Cuenta *CrearCuenta(Registro *actual, Registro *siguiente, Tarifa tar){
	Cuenta *cuenta = (Cuenta*) malloc(sizeof(Cuenta));
	//Guardamos la matrícula del vehículo.
	cuenta->Patente = (char*) malloc(strlen(actual->Patente)+1);
	strcpy(cuenta->Patente, actual->Patente);
	//Conteo inicial del número de viajes.
	cuenta->nroViajes = 1;
	//Importe de la distancia recorrida en el primer viaje.
	cuenta->importerecorrido = 0.01 * tar * abs(siguiente->km - actual->km);
	cuenta->sgt = NULL;
	return cuenta;
}

//Asignamos espacio de memoria para la lista de Cuenta
Cuentas *CrearLibroCuentas(){
	Cuentas *lista = (Cuentas*) malloc(sizeof(Cuentas));
	lista->inicio = NULL;
	return lista;
}//Creacion del registro de cuentas


void AgregarCuenta(Cuentas *lc, Registro *actual, Registro *siguiente, Tarifa tar){

	//Creamos una nueva Cuenta
	Cuenta *cuenta = CrearCuenta(actual, siguiente, tar);

	Cuenta *aux = lc->inicio;
	/*
 	 * Primero comprobamos si la lista esta vacía y en dicho caso agregamos la cuenta.
 	 * En el caso de que la lista presente elementos en ella buscaremos la última cuenta y agregamos la nueva cuenta.
 	 */

	if(lc->inicio == NULL){
		lc->inicio = cuenta;
	}else{
		//Buscamos el último nodo de la lista.
		aux = lc ->inicio;
		while(aux->sgt != NULL){
			aux = aux->sgt;
		}
		//Agregamos el nuevo nodo cuenta.
		aux->sgt = cuenta;
	}
}

/*
 * Esta función recibe un nodo de la lista.
 * Toma de la variable Fecha la hora y la almacena en un arreglo de tipo char.
 * Luego, usando la funcion "atoi" que recibe el arreglo hora devuelve un número entero.
 * Por último, la función retorna el número de la función "atoi".
 */
int PasarHora(Registro *reg){

	char hora[2];

	for(int i=6; i < 8; i++){
		hora[i-6] = reg->Fecha[i];
	}
	return atoi(hora);
}

//Esta función se usa para modificar el importe de los kilometros recorridos de una cuenta existente y el número de viajes que realizó.
void ModificarCuenta(Cuentas *lc, Registro *actual, Registro *siguiente, Tarifa tar){

	Cuenta *aux = lc->inicio;

	//Buscamos la cuenta que corresponde con la patente pasada en el argumento (es el último nodo de la lista).
	while((strcmp(actual->Patente, aux->Patente) != 0) && aux != NULL){
		aux = aux->sgt;
	}

	//Sumamos el importe de los kilómetros recorridos a los que tiene hasta el momento
	aux->importerecorrido += 0.01 * tar *  abs(siguiente->km - actual->km);

	//Agregamos un nuevo viaje
	aux->nroViajes++;
}

void RegistroCuentas(Cuentas *lc, Registro *lr, Tarifa *tar, int nro_prueba, int tam){

	char temp[21];
	int ind=0;

	/*
	 * En el caso de que haya solo un registro directamente no podemos establecer la factura de la matrícula.
	 *Debido a que no hay otro registro para comparar si se cumple que la misma matrícula entro y salió de la autopista.
	 */
	if(tam > 1){
		/*
		 * Buscamos un par de elementos en el arreglo que cumplan las condiciones necesarias para crear una Cuenta.
		 * Esta búsqueda continuará hasta que se cree la primera cuenta o ver alcanzado el final del arreglo de registros.
		 */
		while(ind < tam-1 && lc->inicio == NULL){
			//Comparamos para saber si son iguales las patentes
			if(strcmp(lr[ind].Patente, lr[ind+1].Patente) == 0){
				/*
				 * Dado que las patentes son iguales vemos los estados si "entra" o "sale".
				 * En caso de que sea "entra" y luego "sale" se procederá a crear la cuenta o a modificar la existente.
				 * Cualquier otro caso se ignorará.
				 */
				if(strcmp(lr[ind].Estado, "entra") == 0 && strcmp(lr[ind+1].Estado,"sale") == 0){
					/*
					 * Creamos la cuenta y lo asignamos al inicio de la lista de cuentas.
					 * Pasamos el elemento registro actual y el siguiente a este.
					 * y pasamos la tarifa correspondiente a la hora en que ingreso a la autopista.
					 */
					lc->inicio = CrearCuenta(&lr[ind], &lr[ind+1], tar[PasarHora(&lr[ind])]);
					//Adelantamos el índice para no analizar el elemento de índice ind+1.
					strcpy(temp, lr[ind].Patente);
					ind++;
				}
			}
			ind++;
		}
		/*
		 * En el caso de que el índice "ind" sea mayor o igual que la cantidad de registros "tam" no entrará al siguiente while.
		 * En caso contrario se recorrerá el resto del array de registro.
		 */

		while(ind < tam-1){
			if(strcmp(lr[ind].Patente, lr[ind+1].Patente) == 0){
				if(strcmp(lr[ind].Estado, "entra") == 0 && strcmp(lr[ind+1].Estado,"sale") == 0){
					//Compaamos la patente actual con la patente de la última cuenta creada.
					if(strcmp(temp, lr[ind].Patente) == 0){
						/*
						 * En el caso de que la patente concuerde con la patente de la última cuenta creada.
						 * Modificamos el número de viajes y el importe por la distancia recorrida con su tarifa respectiva.
						 */
						ModificarCuenta(lc, &lr[ind], &lr[ind+1], tar[PasarHora(&lr[ind])]);
					}else{
						/*
						 * Cuando las patentes son iguales pero no concuerdan con la última patente registrada y hay un registro de "entra" y "sale"
					     * Creamos una nueva cuenta.
					     */
						AgregarCuenta(lc, &lr[ind], &lr[ind+1], tar[PasarHora(&lr[ind])]);
					}
					ind++;
					//Guardamos nuevamente la patente ya que esta puede ver cambiado.
					strcpy(temp, lr[ind].Patente);
				}
			}
			ind++;
		}
	}
}

//Eliminamos el primer nodo de la lista
void LiberarCuentas(Cuentas *lc){

		Cuenta *eliminado = lc->inicio;
		lc->inicio = (lc->inicio)->sgt;
		free(eliminado);
}

/*
 * Muestra por pantalla el número de caso de prueba y las cuentas cargadas en la lista de cuentas.
 * En caso de que la lista esté vacía se indicará que no hay casos que cumplan las condiciones solicitadas para establecer la cuenta a una patente en cuestión.
 */
void ImprimirCuentas(int totalCasos, int caso_prueba, Cuentas *lc){

	Cuenta *aux = lc->inicio;

	if(lc->inicio != NULL){
		while(aux != NULL){
			printf("%s $%.2f\n", aux->Patente, aux->importerecorrido + 2 + aux->nroViajes * 1);
			aux = aux->sgt;
		}
	}else{
		printf("No se presentan casos en los que se pueda establecer la tarifa.\n");
	}
	printf("\n");
}

#endif /* CUENTA_H_ */
