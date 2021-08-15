/*
 * Registro.h
 *
 *  Created on: 24 abr. 2021
 *      Author: mar13
 */

#ifndef REGISTRO_H_
#define REGISTRO_H_


/*
 * Registro de las cámaras donde tenemos la patente, la fecha en que se registro, si entro o salió de la autopista y el kilómetro donde ser registro.
 * La variable Fecha le asignamos un espacio de memoria estático dado que no varía su tamaño.
 */
typedef struct nodo{
	char *Patente;
	char Fecha[12];
	char *Estado;
	unsigned int km;
}Registro;

//Liberamos el primer nodo de la lista
void LiberarRegistros(Registro *lr, int tam){

	int ind;
	for(ind = 0; ind < tam; ind++){
		free(lr[ind].Patente);
		free(lr[ind].Estado);
		free(&lr[ind]);
	}
	free(lr);
}

//Esta funcion lee la linea actual del archivo, crea el registro y lo añade a la lista de registros pasada en el argumento
void LlenarARegistro(Registro *registros, FILE *ptr, unsigned int tam){

	char temp[21];
	char fecha[12];
	char est[6];

	/*
 	 * Primero comprobamos si la lista esta vacía y en dicho caso agregamos el registro
 	 * En el caso de que la lista presente elementos en ella buscaremos el último registro y agregamos el nuevo registro
 	 */
	for(int ind=0; ind < tam; ind++){
		fscanf(ptr, "%s", temp);
		registros[ind].Patente = (char*) malloc(strlen(temp)+1);
		strcpy(registros[ind].Patente, temp);
		fscanf(ptr, "%s", fecha);
		strcpy(registros[ind].Fecha, fecha);
		fscanf(ptr, "%s", est);
		registros[ind].Estado = (char*) malloc(strlen(est)+1);
		strcpy(registros[ind].Estado, est);
		fscanf(ptr, "%u", &registros[ind].km);
	}
}

/*
//Si bien no se pide que se muestre por pantalla los registros. Nos permitirá ver la lista incialmente y ver como es luego de ordenarla
void ImprimirRegistros(Registro *registros, int tam){

	for(int ind=0; ind < tam; ind++){
		printf("%s %s %s %u\n", registros[ind].Patente, registros[ind].Fecha, registros[ind].Estado, registros[ind].km);
	}
}
*/

/*
 * Esta función como dice su nombre nos permitirá ordenar la lista de registros hechos por las cámaras
 * Además, facilitará el proceso para establecer que patentes tendrán una cuenta.
 */
void OrdenarRegistros(Registro *list, int tam){

	//Para ordenar el arreglo de registros usaremos el método de la burbuja

	//Índices
	int ind1, ind2;

	//Variables auxiliares para guardar información temporalmente
	char patente[21];
	char fecha[12];
	char estado[6];
	unsigned int dist;

	for(ind1 = 0; ind1 < tam; ind1++ ){
		for(ind2 = 0; ind2 < tam-1; ind2++){
			if(strcmp(list[ind2].Patente, list[ind2+1].Patente) > 0){
				//Guardamos temporalmente
				strcpy(patente, list[ind2].Patente);
				strcpy(fecha, list[ind2].Fecha);
				strcpy(estado, list[ind2].Estado);
				dist = list[ind2].km;

				/*
				 * Modificamos el espacio de memoria asignado para los nuevos valores.
				 * Esto es, ya que la patente y el estado pueden ser mayor a los actuales y al pasarlos generarán un error.
				 */

				list[ind2].Patente = (char*) realloc(list[ind2].Patente, strlen(list[ind2+1].Patente)+1);
				list[ind2].Estado = (char*) realloc(list[ind2].Estado, strlen(list[ind2+1].Estado)+1);

				//Pasamos los valores.
				strcpy(list[ind2].Patente, list[ind2+1].Patente);
				strcpy(list[ind2].Fecha, list[ind2+1].Fecha);
				strcpy(list[ind2].Estado, list[ind2+1].Estado);
				list[ind2].km = list[ind2+1].km;

				//Modificamos el espacio de memoria reservado y luego pasamos los valores guardados.
				list[ind2+1].Patente = (char*) realloc(list[ind2+1].Patente, strlen(patente)+1);
				list[ind2+1].Estado = (char*) realloc(list[ind2+1].Estado, strlen(estado)+1);

				strcpy(list[ind2+1].Patente, patente);
				strcpy(list[ind2+1].Fecha, fecha);
				strcpy(list[ind2+1].Estado, estado);
				list[ind2+1].km = dist;
			}

			/*
			 * En el caso de que al comparar ambos nodos tengan la misma patente
			 * compararemos la fecha en que fue registrado por la cámara
			 * ya que el vehículo no puede estar en dos lugares distintos al mismo tiempo
			 */
			if(strcmp(list[ind2].Patente, list[ind2+1].Patente) == 0){
				if(strcmp(list[ind2].Fecha, list[ind2+1].Fecha) > 0){
					strcpy(patente, list[ind2].Patente);
					strcpy(fecha, list[ind2].Fecha);
					strcpy(estado, list[ind2].Estado);
					dist = list[ind2].km;

					list[ind2].Patente = (char*) realloc(list[ind2].Patente, strlen(list[ind2+1].Patente)+1);
					list[ind2].Estado = (char*) realloc(list[ind2].Estado, strlen(list[ind2+1].Estado)+1);

					strcpy(list[ind2].Patente, list[ind2+1].Patente);
					strcpy(list[ind2].Fecha, list[ind2+1].Fecha);
					strcpy(list[ind2].Estado, list[ind2+1].Estado);
					list[ind2].km = list[ind2+1].km;

					list[ind2+1].Patente = (char*) realloc(list[ind2+1].Patente, strlen(patente)+1);
					list[ind2+1].Estado = (char*) realloc(list[ind2+1].Estado, strlen(estado)+1);

					strcpy(list[ind2+1].Patente, patente);
					strcpy(list[ind2+1].Fecha, fecha);
					strcpy(list[ind2+1].Estado, estado);
					list[ind2+ 1].km = dist;
				}
			}
		}
	}
}

#endif /* REGISTRO_H_ */
