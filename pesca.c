#include "evento_pesca.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

const int MAX_NOMBRE_ARCHIVO = 20;
const int MAX_METODOS_SELECCION = 5;
const bool POKEMON_VALIDO = true, POKEMON_INVALIDO = false;
/*
Pre:Recibe lo ingresado por linea de comando en la ejecucion.
Pos:Asignara el nombre del archivo a una variable
*/
void obtener_archivo_arrecife(int posicion, char* ingreso_linea_comando[], char nombre_archivo_arrecife[]){
	
	strcpy(nombre_archivo_arrecife, ingreso_linea_comando[1]);	
}
/*
Pre:Recibe la direccion de memoria de un pokemon.
Pos:Devuelve True si el pokemon es de color rojo.
	Devuelve False si el pokemon no es de color rojo.
*/
bool es_rojo(pokemon_t* pokemon){
	if(strcmp((*pokemon).color,"rojo") == 0){
		return POKEMON_VALIDO;
	}
	else return POKEMON_INVALIDO;
}
/*
Pre:Recibe la direccion de memoria de un pokemon.
Pos:Devuelve True si el pokemon pesa menos de 50.
	Devuelve False si el pokemon pesa mas de 50.
*/
bool es_liviano(pokemon_t* pokemon){
	return((*pokemon).peso < 50);
}
/*
Pre:Recibe la direccion de memoria de un pokemon.
Pos:Devuelve True si el pokemon pesa menos de 40 y tiene velocidad mayor a 20.
	Devuelve False si el pokemon pesa mas de 40 y tiene velocidad menor a 20.
*/
bool es_atletico(pokemon_t* pokemon){
	return (((*pokemon).peso < 40) && ((*pokemon).velocidad > 20));
}
/*
Pre:Recibe la direccion de memoria de un pokemon.
Pos:Devuelve True si el pokemon pesa menos de 30 y el nombre de su especie es de menos de 7 caracteres.
	Devuelve False si el pokemon pesa mas de 30 y el nombre de su especie es de mas de 7 caracteres.
*/
bool podria_ser_mascota(pokemon_t* pokemon){
	if((strlen((*pokemon).especie) < 7) && ((*pokemon).peso < 30)){
		return POKEMON_VALIDO;
	}
	return POKEMON_INVALIDO;
}
/*
Pre:Recibe la direccion de memoria de un pokemon.
Pos:Devuelve True si el pokemon pesa menos de 30 y su color es salmon o blanquirrojo.
	Devuelve False si el pokemon pesa mas de 30 y su color no es salmon o blanquirrojo.
*/
bool es_tierno(pokemon_t* pokemon){
	if((strcmp((*pokemon).color, "salmon") == 0 || strcmp((*pokemon).color, "blanquirojo") == 0) && (*pokemon).peso < 30){
		return POKEMON_VALIDO;
	}
	return POKEMON_INVALIDO;
}
/*
Pre:Recibe la direccion de memoria de un pokemon.
Pos:Mostrara por pantalla al pokemon al que apunta la direccion de memoria recibida.
*/
void mostrar_arrecife(pokemon_t* pokemon){

	printf("%13s%10i%5i%16s\n", (*pokemon).especie, (*pokemon).velocidad, (*pokemon).peso, (*pokemon).color);

}

int main(int argc, char* argv[]){
	acuario_t *acuario;
	arrecife_t *arrecife;
	int cantidad_a_buscar;
	char nombre_archivo_arrecife[MAX_NOMBRE_ARCHIVO];
	bool (*seleccionar_pokemon[MAX_METODOS_SELECCION])(pokemon_t*);
	void (*mostrar_pokemon)(pokemon_t*);

	seleccionar_pokemon[0] = es_rojo;
	seleccionar_pokemon[1]= es_liviano;
	seleccionar_pokemon[2]= es_atletico;
	seleccionar_pokemon[3]= podria_ser_mascota;
	seleccionar_pokemon[4]= es_tierno;
	mostrar_pokemon = mostrar_arrecife;

	if(argc <= 1){
		printf("No introdujo el archivo en el que se encuentran los pokemones.\n");
		return -1;
	}

	obtener_archivo_arrecife(argc, argv, nombre_archivo_arrecife);

	arrecife = crear_arrecife(nombre_archivo_arrecife);
	if(arrecife == NULL){
		printf("Sin arrecife no tiene sentido, revise y vuelva a intentar\n");
		return -1;
	}
	acuario = crear_acuario();

	for(int i = 0; i < MAX_METODOS_SELECCION; i++){
		printf("Ingrese la cantidad de pokemones que desea sacar del metodo de seleccion numero %i.\n", i+1);
		scanf("%i", &cantidad_a_buscar);
		trasladar_pokemon(arrecife, acuario, seleccionar_pokemon[i], cantidad_a_buscar);
		censar_arrecife(arrecife, mostrar_pokemon);
		sleep(1);
	}
	
	guardar_datos_acuario(acuario, "pokemones_acuario.txt");

	liberar_acuario(acuario);
	liberar_arrecife(arrecife);

	return 0;
}

