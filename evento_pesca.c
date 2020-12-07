#include "evento_pesca.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FORMATO_POKEMONES "%100[^;];%i;%i;%50[^\n]\n"

acuario_t* crear_acuario(){
	acuario_t *acuario;

	acuario = malloc(sizeof(acuario_t));
	(*acuario).pokemon = NULL;
	(*acuario).cantidad_pokemon = 0;
	
	return acuario;
}

void liberar_acuario(acuario_t* acuario){
	if((*acuario).cantidad_pokemon > 0){
		free((*acuario).pokemon);
	}
	free(acuario);
}
/*
Pre:Recibe la ruta del archivo del arrecife y la cantidad de pokemones inicializado en 0.
Pos:Devuelve un puntero al primer elemento de un vector de pokemones.
*/
pokemon_t* obtener_pokemones(const char* ruta_archivo,int* cantidad_pokemones){
	int leidos = 0;
	pokemon_t *pokemon;
	pokemon_t *pokemon_aux = NULL;

	pokemon = malloc(sizeof(pokemon_t));
	if (pokemon == NULL){
		return pokemon;
	}
	FILE* lista_pokemones = fopen(ruta_archivo, "r");
	if (!lista_pokemones){
		printf("No se pudo abrir el archivo\n");
		sleep(1);
		free(pokemon);
		return NULL;
	}

	leidos = fscanf(lista_pokemones, FORMATO_POKEMONES, (*pokemon).especie,&((*pokemon).velocidad), &((*pokemon).peso), (*pokemon).color);
	if(leidos != 4){
		free(pokemon);
		fclose(lista_pokemones);
		return NULL;
	}
	while(leidos == 4){
		*cantidad_pokemones +=1;
		pokemon_aux = realloc(pokemon, (size_t)(*cantidad_pokemones+1)*(sizeof(pokemon_t)));

		if (pokemon_aux == NULL){
			return pokemon;
		}
		pokemon = pokemon_aux;
		leidos = fscanf(lista_pokemones, FORMATO_POKEMONES, (pokemon[*cantidad_pokemones]).especie,&(pokemon[*cantidad_pokemones].velocidad), &(pokemon[*cantidad_pokemones].peso), (pokemon[*cantidad_pokemones]).color);
	}
	pokemon_aux = realloc(pokemon, (size_t)(*cantidad_pokemones)*(sizeof(pokemon_t)));

	if (pokemon_aux == NULL){
		return pokemon;
	}
	pokemon = pokemon_aux;
	
	fclose(lista_pokemones);
	return pokemon;
}
/*
Pre:Recibe la ruta del archivo del arrecife.
Pos:Devuelve un puntero a un arrecife valido.
*/
arrecife_t* crear_arrecife(const char* ruta_archivo){
	arrecife_t *arrecife;
	pokemon_t* pokemon;
	int cantidad_pokemones = 0;

	arrecife = malloc(sizeof(arrecife_t));

	pokemon = obtener_pokemones(ruta_archivo, &cantidad_pokemones);

	if(pokemon == NULL){
		free(arrecife);
		return NULL;
	}

	(*arrecife).pokemon = pokemon;
	(*arrecife).cantidad_pokemon = cantidad_pokemones;
	return arrecife;
}

void liberar_arrecife(arrecife_t* arrecife){
	if((*arrecife).cantidad_pokemon > 0){
		free((*arrecife).pokemon);	
	}
	
	free(arrecife);
}
/*
Pre: Recibe la direccion de memoria de un pokemon valido a mover y la direccion de memoria destino.
Pos:
*/
void cargar_pokemon(pokemon_t* pokemon_a_mover, pokemon_t* posicion_destino){
	if(pokemon_a_mover != posicion_destino){
		strcpy((*posicion_destino).especie, (*pokemon_a_mover).especie);
		(*posicion_destino).velocidad = (*pokemon_a_mover).velocidad;
		(*posicion_destino).peso = (*pokemon_a_mover).peso;
		strcpy((*posicion_destino).color, (*pokemon_a_mover).color);
	}	
}
/*
Pre:Recibe la direccion de memoria de un pokemon y del acuario.
Pos:Traslada un pokemon y modifica el tamaño del vector segun sea necesario.
*/
void traslado_a_acuario(pokemon_t* pokemon, acuario_t* acuario){
	pokemon_t* pokemon_acuario = NULL;
	
	if((*acuario).pokemon == NULL){
		pokemon_acuario = malloc(sizeof(pokemon_t));
		cargar_pokemon(pokemon, pokemon_acuario);
		(*acuario).pokemon = pokemon_acuario;
	}

	pokemon_acuario = realloc((*acuario).pokemon, (size_t)((*acuario).cantidad_pokemon+1)*(sizeof(pokemon_t)));

	if(pokemon_acuario == NULL){
		return;
	}
	(*acuario).pokemon = pokemon_acuario;
	cargar_pokemon(pokemon, (*acuario).pokemon +(*acuario).cantidad_pokemon);
}
/*
Pre:Recibe la direccion de memoria del arrecife y la posicion de un pokemon a eliminar.
Pos: Saca al pokemon de la posicion a eliminar del arrecife y modifica el tamaño de este.
*/
void eliminar_pokemon(arrecife_t* arrecife, int posicion_a_eliminar){
	pokemon_t* pokemon_aux = NULL;

	cargar_pokemon((*arrecife).pokemon + (*arrecife).cantidad_pokemon -1, (*arrecife).pokemon + posicion_a_eliminar);
	pokemon_aux = realloc((*arrecife).pokemon, (size_t)(((*arrecife).cantidad_pokemon)-1)*sizeof(pokemon_t));

	if(pokemon_aux == NULL){
		return;
	}
	(*arrecife).pokemon = pokemon_aux;
}
/*
Pre:Recibe la direccion de memoria del arrecife, la cantidad de pokemones y un puntero a una funcion de seleccion de pokemon todos validos.
Pos:Devuelve True si hay pokemones suficientes que cumplan la condicion recibida.
	Devuelve False si no hay pokemones suficientes que cumplan la condicion recibida.
*/
bool hay_pokemones_suficientes(arrecife_t* arrecife, int cant_seleccion, bool (*seleccionar_pokemon) (pokemon_t*)){
	bool alcanzan_los_pokemon = false;
	int cantidad_pokemon_buscados = 0;
	int i = 0;

	while((i < (*arrecife).cantidad_pokemon) && (alcanzan_los_pokemon == false)){
		if(seleccionar_pokemon(((*arrecife).pokemon) + i) == true){
			cantidad_pokemon_buscados ++;
		}
		if(cantidad_pokemon_buscados >= cant_seleccion){
			alcanzan_los_pokemon = true;
		}
		i++;
	}

	return alcanzan_los_pokemon;
}

int trasladar_pokemon(arrecife_t* arrecife, acuario_t* acuario, bool (*seleccionar_pokemon) (pokemon_t*), int cant_seleccion){
	int i = 0;

	if(hay_pokemones_suficientes(arrecife, cant_seleccion, seleccionar_pokemon) == false){
		printf("No hay pokemones suficientes, los podes matar!!!!\n\n");
		sleep(1);
		return -1;
	}

	while((cant_seleccion > 0) && (i < (*arrecife).cantidad_pokemon)){
		if(seleccionar_pokemon(((*arrecife).pokemon) + i) == true){
			traslado_a_acuario((*arrecife).pokemon + i, acuario);
			eliminar_pokemon(arrecife, i);
			(*acuario).cantidad_pokemon +=1;
			(*arrecife).cantidad_pokemon -=1;
			cant_seleccion --;
			i--;			
		}
		i++;
	}
	return 0;
}

void censar_arrecife(arrecife_t* arrecife, void (*mostrar_pokemon)(pokemon_t*)){
	printf("      Pokemon|Velocidad|Peso|          Color\n");
	for (int i = 0; i < (*arrecife).cantidad_pokemon; i++){
		mostrar_pokemon((*arrecife).pokemon+i);	
	}
	printf("\n");
}

int guardar_datos_acuario(acuario_t* acuario, const char* nombre_archivo){

	FILE* datos_acuario = fopen(nombre_archivo, "w");

	if(!datos_acuario){
		printf("No se pudo abrir el archivo o crearlo para escribir\n");
		return -1;
	}
	for(int i = 0; i < (*acuario).cantidad_pokemon; i++){
		fprintf(datos_acuario, "%s;%i;%i;%s\n", (*acuario).pokemon[i].especie, (*acuario).pokemon[i].velocidad, (*acuario).pokemon[i].peso, (*acuario).pokemon[i].color);
	}

	fclose(datos_acuario);
	return 0;
}