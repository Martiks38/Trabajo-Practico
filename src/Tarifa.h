/*
 * Tarifa.h
 *
 *  Created on: 24 abr. 2021
 *      Author: mar13
 */

#ifndef TARIFA_H_
#define TARIFA_H_

typedef unsigned Tarifa;

//Número total de tarifas por línea. Esto lo sabemos por enunciado
#define Total_Tarifas 24

//Lee la línea de tarifas en el archivo y los guarda en la matriz creada.
void LlenarMTarifa(Tarifa *tar, FILE *ptr){

	int i;
		//Leemos el archivo y guardamos en el arreglo.
		for(i=0; i < Total_Tarifas; i++){
				fscanf(ptr, "%u", &tar[i]);
		}
		//Desplazamos la posición actual del archivo a la línea de los registros.
		fseek(ptr, 2, SEEK_CUR);
}

/*
//Muestra por pantalla la/s línea/s de tarifa cargada/s.
void ImprimirMTarifa(Tarifa *tar){
	int i;
	for(i=0; i < Total_Tarifas; i++){
			printf("$%.2lf\t", tar[i] * 0.01);
		}
		printf("\n");
}
*/

#endif /* TARIFA_H_ */
