// ProyectoMatrixMelanyMoises.cpp : This file contains the 'main' function. Program execution begins and ends there.
//PROYECTO 1
//SIMULADOR MATRIZ
//ESTRUCTURAS DE DATOS.
//II SEMESTRE, 2021.
//AUTORES:
//		   MELANY DAHIANA SALAS FERNÁNDEZ-2021121147
//		   MOISES SOLANO ESPINOZA-2021144322


//--------------------------------Librerias-------------------------------
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include<stdio.h>
#include <ctime> 


//----------------------Librerias-de-allegro------------------------------
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;

#define FPS 30.0
unsigned t0, t1;
ALLEGRO_FONT* fuente;

//-----------------------------------------------------------------------
//FUNCION PARA EVALUAR LOS CARACTERES DEL RAMDOM- EVITA PUNTOS, COMAS...
//ENTRADAS: UN NUMERO.

bool excepciones(int num) {
	int excepciones[6];
	excepciones[0] = 91;
	excepciones[1] = 92;
	excepciones[2] = 93;
	excepciones[3] = 94;
	excepciones[4] = 95;
	excepciones[5] = 96;

	for (int i = 0; i < 6; i++) {
		if (excepciones[i] == num)
			return false;
	}
	return true;
}

//-----------------------------------------------------------------------
//FUNCION PARA GENERAR CARACTERES RANDOM
//ENTRADAS: NONE.
//SALIDAS: UN CHAR
char GenerarRandom()
{
	char c;
	do {
		c = 65 + rand() % (122 - 65);
	} while (excepciones(c) == false);

	return c;
}


//Variables de control de tamaño de pantalla y espaciado entre los caracteres
int anchoPantalla = 1200;
int altoPantalla = 700;
int espacioLetras = 20;


int coorX = 10, coorY = 0;//coordenadas en pantalla


char actual[1];
int contPistas = 0;
int contChar = 0;
int contAgrupaciones = 0;

int timeD;

//----------------------------------------------------------------------
//                     ESTRUCTURA DE TIPO TPISTA
//----------------------------------------------------------------------
typedef struct TPista {
	int codigo;
	int X;
	int Y;
	char pila[13];
	TPista* Siguiente;
}*PtrTPista;
//

//-----------------------------------------------------------------------
//FUNCION PARA CAMBIAR LA VARIABLE EN LA SALIDA DEL CICLO.
//ENTRADAS: VARIABLE DE TIPO BOOLEANO Y EL EVENTO, AMBOS POR REFERENCIA.

void salidaCiclo(bool& ciclo1, ALLEGRO_EVENT& evento) {

	if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
		if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			ciclo1 = false;
		}
	}
}
//-----------------------------------------------------------------------
//FUNCION PARA INICIALIZAR UN PUNTERO DE TIPO PtrTPista
//ENTRADAS: UN PUNTERO DE TIPO PtrTPista

void inicializarPistas(PtrTPista& listaP) {
	listaP = NULL;
}

//-----------------------------------------------------------------------
//FUNCION PARA CREAR UN NUEVO ELEMENTO DE TIPO TPista
//ENTRADAS: RECIBE UN CODIGO UNA COORDENADA X Y OTRA Y Y UN CHAR
//SALIDAS:RETORNA EL LA DIRECCION DE MEMORIA DE LA VARIABLE ANONIMA CREADA

PtrTPista crea_pista(int codigo, int coordX, int coordY, char caracter[1]) {
	PtrTPista Aux = new(TPista);

	// SE ASIGNAN LOS VALORES A LA NUEVA PISTA
	Aux->codigo = codigo;
	Aux->X = coordX;
	Aux->Y = coordY;

	for (int i = 0; i < 13; i++) { // SE ASIGNAN CADA UNO DE LOS 13 CARACTERES DEL ARRAY
		Aux->pila[i] = caracter[0];
	}
	Aux->Siguiente = NULL; // APUNTA A NULL Y SE RETORNA
	return Aux;
}

//-----------------------------------------------------------------------
//FUNCION PARA AGREGAR ELEMENTOS A LA LISTA
//ENTRADAS: RECIBE LA LISTA ENLAZADA POR REFERECIA Y EL ELEMENTO CREADO
//SALIDAS:NONE

void agregaLista(PtrTPista& lista, PtrTPista nuevo) {
	nuevo->Siguiente = lista; // al siguiente del nuevo elemento 
	lista = nuevo; // lista va a ser igual al nuevo que almacena 

}

//-----------------------------------------------------------------------
//FUNCION PARA DESTRUIR UNA LISTA ENLAZADA
//ENTRADAS: RECIBE LA LISTA ENLAZADA POR REFERECIA 
//SALIDAS:NONE

void destruirPistas(PtrTPista& lista) {
	PtrTPista Aux; // variable local auxiliar 
	Aux = lista; // a auxiliar se le asigna los datos de la lista
	while (Aux != NULL) {
		lista = lista->Siguiente; // lista va a ser igual a si misma pero sin el primer elemento
		delete(Aux); // se elimina el primer elemento
		Aux = lista; // a auxiliar se le asigna nuevamente el valor de lista
	}
	cout << "La lista enlazada ha sido destruida" << endl;
}

//-----------------------------------------------------------------------
//FUNCION PARA BUSCAR UNA PISTA
//ENTRADAS: RECIBE LA LISTA ENLAZADA Y CODIGO DEL ELEMENTO A BUSCAR
//SALIDAS: RETORNA EL PUNTERO CON LA DIRECCION DEL ELEMENTO ENCONTRADO

PtrTPista BuscarPista(PtrTPista& Lista, int cual) // funcion para buscar un articulo en el inventario
{
	bool encontro = false; // variable que controla si se encontró la pista buscada
	PtrTPista Aux; // Puntero auxiliar que buscara el elemento
	Aux = Lista; // auxiliar se inicializa con el valor del inventario

	while (encontro != true && Aux != NULL) // mientras encontro sea false y auxiliar sea diferente a NULL
	{
		if (Aux->codigo == cual) // si el codigo de la pista es igual al buscado
			encontro = true; // encontro se vuelve true
		else
			Aux = Aux->Siguiente; // auxiliar avanza a la siguiente pista
	}
	return Aux; // retorna el valor de aux
}

//-----------------------------------------------------------------------
//FUNCION PARA BORRAR 1 ELEMENTO
//ENTRADAS: RECIBE LA LISTA ENLAZADA POR REFERECIA Y EL ELEMENTO A ELIMINAR
//SALIDAS: FALSE SI NO LO LOGRA BORRAR Y TRUE SI LO LOGRA

bool Borrar1Pista(PtrTPista& Lista, int Cual) {
	bool encontro = false; // variable que guarda si se ha encontrado el elemento o no
	PtrTPista Aux, Aux2, AuxBorrar; // punteros para recorrer la lista
	Aux = Lista; // aux va a ser igual a lista
	int contador = 0; // contador para verificar que solo la primera iteración entre al primer if

	// CASO 1: EL ELEMENTO NO SE ENCUENTRA EN LA LISTA ENLAZADA
	if (BuscarPista(Lista, Cual) == NULL) { // si el elemento no está en la lista
		return false;
	}
	// CASO 2: LA LISTA SOLO TIENE UN ELEMENTO Y ES EL QUE SE DESEA ELIMINAR
	if ((Aux != NULL) && (Aux->Siguiente == NULL) && (Aux->codigo == Cual)) { // si la lista tiene solo un elemento
		Lista = Lista->Siguiente; // lista va a ser igual a si misma pero sin el primer elemento
		delete(Aux); // se elimina el primer elemento
		Aux = Lista; // a auxiliar se le asigna nuevamente el valor de lista
		encontro = true;
	}

	// CASO 3: EL ELEMENTO QUE SE QUIERE BORRAR ES EL PRIMERO DE LA LISTA PERO TIENE MAS ELEMENTOS
	while ((Aux != NULL) && !encontro) { // mientras no se haya encontrado y la lista no apunte a NULL
		if ((contador == 0) && Aux->codigo == Cual) { // si el primer elemento es el buscado
			Lista = Lista->Siguiente; // lista va a ser igual a si misma pero sin el primer elemento
			delete(Aux); // se elimina el primer elemento
			Aux = Lista; // a auxiliar se le asigna nuevamente el valor de lista
			contador++;
			encontro = true;

		}
		// CASO 4: EL SIGUIENTE ELEMENTO ES EL QUE SE QUIERE ELIMINAR
		if (Aux->Siguiente != NULL) { // si el siguiente articulo es el que se quiere borrar
			if (Aux->Siguiente->codigo == Cual) {
				AuxBorrar = Aux->Siguiente; // auxBorrar va a apuntar al elemento que se desea borrar
				Aux2 = Aux->Siguiente->Siguiente; // aux2 va a apuntar al siguiente elemento después del borrado
				delete(AuxBorrar); // se borra el elemento
				Aux->Siguiente = Aux2; // se une la parte de antes y después del elemento borrado
				encontro = true;
			}

		}
		Aux = Aux->Siguiente; // avanza de articulo
	}
	return encontro;
}


//-----------------------------------------------------------------------
//FUNCION PARA MOVER LOS ELEMENTOS DE LA PILA DE CARACTERES
//ENTRADAS: RECIBE EL ELEMETO ACTUAL
//SALIDAS:NONE
void moverPista(PtrTPista& actual) {
	// LOS CARACTERES DEL ARRAY SE DESPLAZAN PARA GENERAR EL EFECTO DE QUE SE MUEVEN
	actual->pila[12] = actual->pila[11];
	actual->pila[11] = actual->pila[10];
	actual->pila[10] = actual->pila[9];
	actual->pila[9] = actual->pila[8];
	actual->pila[8] = actual->pila[7];
	actual->pila[7] = actual->pila[6];
	actual->pila[6] = actual->pila[5];
	actual->pila[5] = actual->pila[4];
	actual->pila[4] = actual->pila[3];
	actual->pila[3] = actual->pila[2];
	actual->pila[2] = actual->pila[1];
	actual->pila[1] = actual->pila[0];

	// LA PILA DE CARACTERES AVANZA 20 PIXELES HACIA ABAJO
	actual->Y = actual->Y + 20;
}


//-----------------------------------------------------------------------
//FUNCION PARA DUBUJAS LA PILA EN PANTALLA
//ENTRADAS: LA PISTA ACTUAL POR REFERECIA Y UN CARACTER ALEATORIO
//SALIDAS:NONE

void dibujaPantalla(PtrTPista& pistaActual, char caracter[1]) {
	if (pistaActual != NULL) {
		// SE PINTAN EN LA PANTALLA CADA UNO DE LOS CARACTERES DE LA HILERA CON UN TONO DIFERENTE DE VERDE, EMPEZANDO EN
		// BLANCO HASTA LLEGAR A NEGRO PARA GENERAR LA ILUSIÓN DE DESAPARICION
		actual[0] = pistaActual->pila[0];
		al_draw_text(fuente, al_map_rgb(71, 172, 52), pistaActual->X, pistaActual->Y - 20, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[1];
		al_draw_text(fuente, al_map_rgb(66, 153, 53), pistaActual->X, pistaActual->Y - 40, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[2];
		al_draw_text(fuente, al_map_rgb(63, 140, 53), pistaActual->X, pistaActual->Y - 60, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[3];
		al_draw_text(fuente, al_map_rgb(59, 125, 50), pistaActual->X, pistaActual->Y - 80, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[4];
		al_draw_text(fuente, al_map_rgb(59, 125, 50), pistaActual->X, pistaActual->Y - 100, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[5];
		al_draw_text(fuente, al_map_rgb(54, 110, 46), pistaActual->X, pistaActual->Y - 120, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[6];
		al_draw_text(fuente, al_map_rgb(50, 94, 41), pistaActual->X, pistaActual->Y - 140, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[7];
		al_draw_text(fuente, al_map_rgb(42, 76, 34), pistaActual->X, pistaActual->Y - 160, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[8];
		al_draw_text(fuente, al_map_rgb(35, 58, 30), pistaActual->X, pistaActual->Y - 180, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[9];
		al_draw_text(fuente, al_map_rgb(27, 43, 25), pistaActual->X, pistaActual->Y - 200, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[10];
		al_draw_text(fuente, al_map_rgb(19, 28, 19), pistaActual->X, pistaActual->Y - 220, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[11];
		al_draw_text(fuente, al_map_rgb(11, 14, 12), pistaActual->X, pistaActual->Y - 240, ALLEGRO_ALIGN_CENTRE, actual);
		actual[0] = pistaActual->pila[12];
		al_draw_text(fuente, al_map_rgb(6, 7, 7), pistaActual->X, pistaActual->Y - 260, ALLEGRO_ALIGN_CENTRE, actual);

		// ESTE CARACTER ES EL ALEATORIO QUE SE PINTA EN COLOR BLANCO
		al_draw_text(fuente, al_map_rgb(255, 255, 255), pistaActual->X, pistaActual->Y, ALLEGRO_ALIGN_CENTRE, caracter);
	}

	// SE MUEVEN LOS CARACTERES DE LA HILERA DE CARACTERES
	moverPista(pistaActual);
	pistaActual->pila[0] = caracter[0]; // SE COPIA EL CARACTER

}

//-----------------------------------------------------------------------
//FUNCION PARA PINTAR TODAS LAS PISTAS
//ENTRADAS: RECIBE LA LISTA ENLAZADA 
//SALIDAS:NONE
void pintarTodas(PtrTPista lista) {
	PtrTPista Aux = lista; // AUXILIAR PARA RECORRER LA LISTA ENLAZADA
	char caracter[1]; // CARACTER QUE ALMACENARÁ EL ALEATORIO
	while (Aux != NULL) {
		caracter[0] = GenerarRandom(); // SE GENERA EL CARACTER ALEATORIO
		contChar++; // SE INCREMENTA EL CONTADOR DE CARACTERES IMPRESOS EN PANTALLA
		dibujaPantalla(Aux, caracter); // SE LLAMA A LA FUNCION DE DIBUJAR EN PANTALLA CON EL ELEMENTO DE LA LISTA 
									   // ENLAZADA ACTUAL CON EL CARACTER ALEATORIO
		Aux = Aux->Siguiente; // SE AVANZA DE ELEMENTO
	}
}

//-----------------------------------------------------------------------
//FUNCION PARA VERIFICAR LOS EXTREMOS DE LA VENTANA
//ENTRADAS: RECIBE LA LISTA ENLAZADA 
//SALIDAS:NONE
void verificarExtremo(PtrTPista& lista) {
	PtrTPista Aux = lista; // AUXILIAR PARA RECORRER LA LISTA ENLAZADA
	while (Aux != NULL) {
		if (Aux->Y >= (altoPantalla + 170)) { // SE VALIDA QUE LA POSICION "Y" DE LA PISTA SEA MAYOR O IGUAL
											  // A 170, ES DECIR QUE YA NO SE VERIA EN LA PANTALLA
			Borrar1Pista(lista, Aux->codigo); // SE BORRA EL ELEMENTO DE LA LISTA ENLAZADA
			Aux = lista; // AUXILIAR VA A EMPEZAR DE NUEVO DESDE EL COMIENZO DE LA LISTA
		}
		else
			Aux = Aux->Siguiente; // AUXILIAR AVANZA DE ELEMENTO EN LA LISTA ENLAZADA
	}
}

//-----------------------------------------------------------------------
//FUNCION PARA VER SI LA COORDENADA X ESTA DISPONIBLE (NO HAY CARACTERES CAYENDO EN EL RANGO ESTABLECIDO)
//ENTRADAS: RECIBE LA LISTA ENLAZADA Y EL CODIGO DE UNA PISTA
//SALIDAS:NONE
bool estaCoordenadaDisponible(PtrTPista lista, int cual) {
	PtrTPista Aux = lista; // AUXILIAR PARA RECORRER LA LISTA
	while (Aux != NULL) {
		if (cual == Aux->X && Aux->Y < 250) // SI EL NUMERO CUAL ES IGUAL A LA COORDENADA X DEL ELEMENTO ACTUAL
											// Y LA COORDENADA "Y" DEL ELEMENTO ES MENOR A 250
			return false; // NO SE PUEDE IMPRIMIR EN ESTA COORDENADA
		Aux = Aux->Siguiente;
	}
	return true; // LA HILERA SE PUEDE IMPRIMIR
}

//-----------------------------------------------------------------------
//FUNCION PARA GUARDAR LAS ESTADISTICAS EN EL ARCHIVO (HACE APPEND)
//ENTRADAS: NO TIENE PARAMETROS, NO RECIBE ARGUMENTOS
//SALIDAS:NONE
void guardarArchivo() {
	FILE* arcEstMatrix; // PUNTERO FILE
	fopen_s(&arcEstMatrix, "EstadisticasMatrix.txt", "a"); // SE ABRE EL ARCHIVO CON FORMATO APPEND PARA QUE SE SIGAN ESCRIBIENDO
														   // LOS DATOS DE FUTURAS EJECUCIONES
	fprintf_s(arcEstMatrix, "\n\tESTADISTICAS\t");
	fprintf_s(arcEstMatrix, "\nCANTIDAD TOTAL DE PISTAS: %i", contPistas);
	fprintf_s(arcEstMatrix, "\nCANTIDAD TOTAL DE CARACTERES: %i", contChar);
	fprintf_s(arcEstMatrix, "\nCANTIDAD TOTAL DE HILERAS: %i", contAgrupaciones);
	fprintf_s(arcEstMatrix, "\nCANTIDAD TOTAL DE SEGUNDOS: %i", timeD);
	fprintf_s(arcEstMatrix, "\n-------------------------------------");

	fclose(arcEstMatrix); // SE CIERRA EL ARCHIVO
}

//-----------------------------------------------------------------------
//FUNCION PARA PINTAR LISTA
//ENTRADAS: RECIBE LA LISTA ENLAZADA POR REFERECIA
//SALIDAS:NONE
void pintarLista(PtrTPista lista) {
	PtrTPista Aux = lista; // AUXILIAR PARA RECORRER LA LISTA

	if (Aux == NULL) { // SI LA LISTA NO TIENE ELEMENTOS
		cout << "La lista no tiene elementos" << endl;
	}

	while (Aux != NULL) { // SE IMPRIME EL CODIGO DE CADA HILERA QUE SE ENCUENTRE EN LA LISTA ENLAZADA
		cout << "Elemento numero: " << Aux->codigo << endl;
		Aux = Aux->Siguiente;
	}
}


//---------------------------------PROGRAMA PRINCIPAL--------------------------------------
int main() {
	t0 = clock(); // SE UTILIZA PARA MEDIR EL TIEMPO DE EJECUCION

	PtrTPista lista; // LISTA ENLAZADA DE HILERAS DE CARACTERES
	PtrTPista nuevo; // EL NUEVO ELEMENTO QUE SE IRÁ AÑADIENDO A LA LISTA ENLAZADA

	inicializarPistas(lista); //Se llama la funcion que inicializa la lista
	inicializarPistas(nuevo);
	int identificador = 0; //Contador que sirve de indentificador para los elementos 
	char caracter[1] = "";
	int numY;
	int numX;
	coorX = 10;

	while (coorX <= anchoPantalla) { // CALCULA LA CANTIDAD DE PISTAS QUE EXISTEN DEPENDIENDO DEL ANCHO DE LA PANTALLA
									// Y EL ESPACIADO ENTRE LAS PISTAS
		contPistas++; // SE INCREMENTA EL CONTADOR DE PISTAS
		coorX += espacioLetras; // SE INCREMENTA LA coorX CON EL ESPACIO ENTRE LETRAS
	}

	if (!al_init()) { // ERROR AL ABRIR ALLEGRO
		fprintf(stderr, "No se puede iniciar allegro!\n");
		return -1;
	}

	//Hablitacion de complementos
	al_install_keyboard(); //teclado
	al_init_font_addon();  //fuentes
	al_init_ttf_addon();   //fuentes

	al_install_audio(); // se utiliza para el sonido
	al_init_acodec_addon();

	al_reserve_samples(1); // se reserva el espacio de los samples
	//Elementos a usar
	ALLEGRO_DISPLAY* pantalla; // pantalla
	ALLEGRO_KEYBOARD_STATE teclado; // teclado
	ALLEGRO_FONT* fuenteC = al_load_font("Fonts/CONSOLAB.ttf", 50, NULL); // SE CARGAN LAS FUENTES
	ALLEGRO_FONT* fuenteD = al_load_font("Fonts/CONSOLAB.ttf", 18, NULL);
	fuente = al_load_font("Fonts/matrix code nfi.ttf", 19, NULL);

	ALLEGRO_SAMPLE* song; // CARGA DE LA MUSICA
	song = al_load_sample("Musica/matrix.wav");
	al_play_sample(song, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

	ALLEGRO_EVENT_QUEUE* colaEventos = al_create_event_queue(); // COLA DE EVENTOS
	ALLEGRO_TIMER* timer = al_create_timer(2 / 30.0); // CREACION DE LOS TIMERS
	ALLEGRO_TIMER* timer2 = al_create_timer(8.0 / 30.0);

	//PANTALLA
	pantalla = al_create_display(anchoPantalla, altoPantalla); // SE ESTABLECEN LAS DIMENSIONES DE LA PANTALLA
	al_set_window_position(pantalla, 100, 80); // POSICION DE LA PANTALLA
	al_set_window_title(pantalla, "MATRIX"); // NOMBRE DE LA PANTALLA

	al_set_new_display_flags(ALLEGRO_WINDOWED);

	//Registro de colas de eventos
	al_register_event_source(colaEventos, al_get_keyboard_event_source());
	al_register_event_source(colaEventos, al_get_timer_event_source(timer));
	al_register_event_source(colaEventos, al_get_timer_event_source(timer2));

	al_start_timer(timer);
	al_start_timer(timer2);
	bool ciclo1 = true; // ciclo infinito
	while (ciclo1) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(colaEventos, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {

			if (evento.timer.source == timer) {
				al_clear_to_color(al_map_rgb(0, 0, 0));	// PANTALLA EN NEGRO

				for (int i = 0; i < 3; i++) { // SE REPITE ESTE CODIGO 3 VECES
					do { // DO WHILE QUE BUSCA QUE LA NUEVA COORDENA X SE ENCUENTRE ENTRE EL RANGO ESTABLECIDO
						numX = 10 + rand() % (anchoPantalla - 10);
					} while ((numX % espacioLetras != 0));

					if (estaCoordenadaDisponible(lista, numX) == true) { // SI LA COORDENADA X ESTA DISPONIBLE
						numY = -100 + rand() % (0 - -100); // LA COORDENADA Y VA A ENCONTRARSE ENTRE -100 Y 0
						coorY = numY;
						nuevo = crea_pista(identificador, numX, coorY, caracter); // CREACION DE LA NUEVA HILERA DE CARACTERES
						agregaLista(lista, nuevo); // SE AGREGA LA HILERA A LA LISTA ENLAZADA
						identificador++; // SE INCREMENTA EL IDENTIFICADOR

						contAgrupaciones++; // SE INCREMENTA EL CONTADOR DE AGRUPACIONES
					}
				}

				pintarTodas(lista); // SE PINTAN LAS HILERAS EN LA PANTALLA

				al_flip_display();

			}
			verificarExtremo(lista); // SE VERIFICAN LOS RANGOS EN PANTALLA DE LAS HILERAS PARA SABER SI SE BORRAN O NO
		}
		salidaCiclo(ciclo1, evento); // SE SALE DE LA EMULACION CON LA TECLA ESCAPE

	}

	//pintarLista(lista);


	al_destroy_sample(song); // SE DESTRUYE LA CANCION PARA QUE DEJE DE SONAR

	t1 = clock(); // SE UTILIZA PARA CALCULAR EL TIEMPO DE EJECUCION
	timeD = (double(t1 - t0) / CLOCKS_PER_SEC);
	//cout << "El tiempo de ejecucion es de: " << timeD << endl;
	//cout << "El tiempo de ejecucion es de: " << timeD/60 << endl;
	bool ciclo2 = true; // VARIABLE PARA EL CICLO INFINITO 
	bool color = true; // VARIABLE PARA PINTAR EL GUIN BAJO QUE PARPADEA EN PANTALLA

	char bufferAgrupaciones[15]; // PARA MOSTRAR EL CONTADOR DE AGRUPACIONES
	_itoa_s(contAgrupaciones, bufferAgrupaciones, 10);

	char bufferChar[15]; // PARA MOSTRAR EL CONTADOR DE CARACTERES
	_itoa_s(contChar, bufferChar, 10);

	char bufferTime[15]; // PARA MOSTRAR EL TIEMPO DE EJECUCION EN SEGUNDOS
	_itoa_s(timeD, bufferTime, 10);

	char seg[] = " segundos"; // SE CONCATENA SEGUNDOS
	strcat_s(bufferTime, seg);

	char bufferPistas[4]; // PARA MOSTRAR EL CONTADOR DE PISTAS
	_itoa_s(contPistas, bufferPistas, 10);


	guardarArchivo(); // SE GUARDAN LOS DATOS DE LA SIMULACION EN EL ARCHIVO TXT.

	while (ciclo2) { // CICLO INFINITO

		ALLEGRO_EVENT evento;
		al_wait_for_event(colaEventos, &evento);
		if (evento.type == ALLEGRO_EVENT_TIMER) {

			if (evento.timer.source == timer2) {

				al_clear_to_color(al_map_rgb(0, 0, 0));
				// PINTADO EN PANTALLA DE LA PALABRA ESTADISTICAS
				al_draw_text(fuenteC, al_map_rgb(255, 255, 255), 400, 150, ALLEGRO_ALIGN_LEFT, "E");
				al_draw_text(fuenteC, al_map_rgb(71, 172, 52), 430, 150, ALLEGRO_ALIGN_LEFT, "S");
				al_draw_text(fuenteC, al_map_rgb(66, 153, 53), 460, 150, ALLEGRO_ALIGN_LEFT, "T");
				al_draw_text(fuenteC, al_map_rgb(63, 140, 53), 490, 150, ALLEGRO_ALIGN_LEFT, "A");
				al_draw_text(fuenteC, al_map_rgb(59, 125, 50), 520, 150, ALLEGRO_ALIGN_LEFT, "D");
				al_draw_text(fuenteC, al_map_rgb(54, 110, 46), 550, 150, ALLEGRO_ALIGN_LEFT, "I");
				al_draw_text(fuenteC, al_map_rgb(54, 103, 45), 580, 150, ALLEGRO_ALIGN_LEFT, "S");
				al_draw_text(fuenteC, al_map_rgb(50, 91, 40), 610, 150, ALLEGRO_ALIGN_LEFT, "T");
				al_draw_text(fuenteC, al_map_rgb(43, 76, 35), 640, 150, ALLEGRO_ALIGN_LEFT, "I");
				al_draw_text(fuenteC, al_map_rgb(38, 63, 30), 670, 150, ALLEGRO_ALIGN_LEFT, "C");
				al_draw_text(fuenteC, al_map_rgb(33, 51, 27), 700, 150, ALLEGRO_ALIGN_LEFT, "A");
				al_draw_text(fuenteC, al_map_rgb(29, 43, 25), 730, 150, ALLEGRO_ALIGN_LEFT, "S");

				if (color) { // IF ELSE PARA PINTAR EL GUION BAJO PARPADEANTE
					al_draw_text(fuenteC, al_map_rgb(255, 255, 255), 760, 150, ALLEGRO_ALIGN_LEFT, "_");
					color = !color;
				}
				else {
					al_draw_text(fuenteC, al_map_rgb(0, 0, 0), 760, 150, ALLEGRO_ALIGN_LEFT, "_");
					color = !color;
				}

				// PINTADO EN PANTALLA DE LAS ESTADISTICAS DE EJECUCION
				al_draw_text(fuenteD, al_map_rgb(71, 172, 52), 460, 300, ALLEGRO_ALIGN_LEFT, "PISTAS: ");
				al_draw_text(fuenteD, al_map_rgb(255, 255, 255), 540, 300, ALLEGRO_ALIGN_LEFT, bufferPistas);

				al_draw_text(fuenteD, al_map_rgb(71, 172, 52), 460, 340, ALLEGRO_ALIGN_LEFT, "AGRUPACIONES: ");
				al_draw_text(fuenteD, al_map_rgb(255, 255, 255), 600, 340, ALLEGRO_ALIGN_LEFT, bufferAgrupaciones);

				al_draw_text(fuenteD, al_map_rgb(71, 172, 52), 460, 380, ALLEGRO_ALIGN_LEFT, "LETRAS: ");
				al_draw_text(fuenteD, al_map_rgb(255, 255, 255), 540, 380, ALLEGRO_ALIGN_LEFT, bufferChar);

				al_draw_text(fuenteD, al_map_rgb(71, 172, 52), 460, 420, ALLEGRO_ALIGN_LEFT, "TIEMPO: ");
				al_draw_text(fuenteD, al_map_rgb(255, 255, 255), 540, 420, ALLEGRO_ALIGN_LEFT, bufferTime);


				al_draw_text(fuenteD, al_map_rgb(71, 172, 52), 480, 640, ALLEGRO_ALIGN_LEFT, "MATRIX VERSION 2.0.0 ");

				al_flip_display();

			}


		}

		salidaCiclo(ciclo2, evento); // SE SALE DEL CICLO CON LA TECLA ESCAPE
	}

	//Destruccion de variables, listas, componentes... usados
	al_destroy_event_queue(colaEventos);
	al_destroy_font(fuente);
	al_destroy_font(fuenteC);
	al_destroy_font(fuenteD);
	al_destroy_timer(timer);
	al_destroy_timer(timer2);

	al_destroy_display(pantalla);

	destruirPistas(lista);


}