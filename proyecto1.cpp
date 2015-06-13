#include <curses.h>
#include <cstdlib>
#include <ctime>

#define ESC 27

struct unidades {
	int y,x,hp;
};

struct bombas{
	int y, x;
};

//UNIDADES = PERSONAJE, ARTICULOS.
#define UNIDADES_ 46 
#define BOMBAS_ 1

bombas bombas_lista[BOMBAS_];
int articulos_tomados=0;
unidades unidades_lista[UNIDADES_];

#define HEIGHT_ 30 
#define WIDTH_ 59 
unidades *unidades_mapa[HEIGHT_][WIDTH_];
bombas *bombas_mapa[HEIGHT_][WIDTH_];
const char* map[]={
"######--###################################################",
"#             #                                #          #",  
"#             #                                #          #",  
"#             #                                           #",  
"#             #                                           #",  
"#### ##########                                           #",  
"#                                                         #",  
"#                                              #          #",  
"#                                              #          #",  
"#                                              #          #",  
"#                                              #          #",  
"#                                              #          #",  
"################     #################         ############",  
"#                                                         #",  
"#                          #                              #",  
"#                        #   #                            #",  
"#                       #     #                           #",  
"#                        #   #                            #", 
"#                          #                              #",  
"#                                                         #",  
"#######################                                   #", 
"#                     #                                   #",  
"#                     #                                   #",  
"#                     #                                   #",  
"#                     #                                   #",  
"#                     #                           #########",  
"#                     #                                   #",  
"#                                                 #########", 
"#                     #                                   #",  
"###########################################################" 
};

void init_unidades();
void init_bomba();
void mover(int *y,int *x,int dy,int dx);

int main() {

	keypad(initscr(),1);
	curs_set(0);
	
	//inicializa unidades
	srand(time(0));
	init_unidades();
	init_bomba();

	//coordenadas donde comienza el jugador
	int *y=&unidades_lista[0].y;
	int *x=&unidades_lista[0].x;
	
	//agrega colores a las piezas
	start_color();
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));
	attroff(COLOR_PAIR(2));
	
	//variable que lee que tecla ha sido apretada	
	int characterpressed;
	do {
		//dibujar  mapa
        	for (int drawy=0;drawy<HEIGHT_;drawy++)
            		for (int drawx=0;drawx<WIDTH_;drawx++)
                		mvaddch(drawy,drawx,map[drawy][drawx]);

                //mueve al jugador en caso de que no se encuentren paredes
		//en el camino
        	if (KEY_UP==characterpressed && '#'!=map[*y-1][*x]){
        		if('-'==map[*y-1][*x]&&
			  (articulos_tomados<(UNIDADES_-1))){
				printw("\n\n\n\nNo has collecionado todos los articulos no puedes salir");
			}
			else if('-'==map[*y-1][*x]&&(articulos_tomados==
				(UNIDADES_-1))){
				printw("\n\n\n\n Felicidades, conseguiste todas las riquezas has ganado");
				getch();
				endwin();
				exit(1);
			}
			else{
				mover(y,x,-1,0);
				init_bomba();
			}
		}
        	if (KEY_DOWN==characterpressed && ' '==map[*y+1][*x])
        		mover(y,x,1,0);
			init_bomba();
        	if (KEY_LEFT==characterpressed && ' '==map[*y][*x-1])
        		mover(y,x,0,-1);
			init_bomba();
        	if (KEY_RIGHT==characterpressed && ' '==map[*y][*x+1])
        		mover(y,x,0,1);
			init_bomba();
		if (KEY_F(2)==characterpressed)
			printw("\n\narticulos tomados %d",articulos_tomados);
        
		//dibujar unidades
        	for (int unidad=0;unidad<UNIDADES_;unidad++) {
        		if (unidades_lista[unidad].hp>0)
 			   	 mvaddch(unidades_lista[unidad].y,
			    	 unidades_lista[unidad].x,
			   	 0==unidad?'@'|COLOR_PAIR(1):'t'|COLOR_PAIR(3));
			}
		
		//dibujar bomba
		if(KEY_UP==characterpressed||KEY_DOWN==characterpressed||
		   KEY_LEFT==characterpressed||KEY_RIGHT==characterpressed)
			if(bombas_lista[0].y==unidades_lista[0].y&&
			   bombas_lista[0].x==unidades_lista[0].x){
				printw("\n\n\n\n Has Explotado Tio");
				getch();
				endwin();
				exit(1);
			}
			else{
				mvaddch(bombas_lista[0].y,
			 	bombas_lista[0].x,'o'|COLOR_PAIR(2));	
			}
	} while ((ESC!=(characterpressed=getch())));
	
	endwin();
}

//funcion para inicializar al jugador y los articulos
void init_unidades() {
	for (int unidad=0;unidad<UNIDADES_;unidad++) {
		unidades *coordinateunidad=&unidades_lista[unidad];
        	coordinateunidad->hp=1;
        	//coloca los articulos en el mapa siempre y cuando no se
		//encuentre una pared en la coordenada dada
		do {
			 coordinateunidad->y=rand()%HEIGHT_;
            		 coordinateunidad->x=rand()%WIDTH_;
       		 } while ('#'==map[coordinateunidad->y][coordinateunidad->x] 
			   ||'-'==map[coordinateunidad->y][coordinateunidad->x]
			   || NULL!=unidades_mapa[coordinateunidad->y]
			   [coordinateunidad->x]);
 		
		unidades_mapa[coordinateunidad->y][coordinateunidad->x]=
		coordinateunidad;
    	}
}

//funcion para inicializar la bomba
void init_bomba(){
	bombas *coordinatebomba=&bombas_lista[0];
	do{
		coordinatebomba->y=rand()%HEIGHT_;
		coordinatebomba->x=rand()%WIDTH_;
	}while('#'==map[coordinatebomba->y][coordinatebomba->x]
		||'t'==map[coordinatebomba->y][coordinatebomba->x]
		||'-'==map[coordinatebomba->y][coordinatebomba->x]
		||NULL!=bombas_mapa[coordinatebomba->y][coordinatebomba->x]);
	
	bombas_mapa[coordinatebomba->y][coordinatebomba->x]=coordinatebomba;
}

//esta funcion mueve al jugador
void mover(int *y,int *x,int dy,int dx) {
	//borra la referencia y la antigua posicion del jugador
	unidades_mapa[*y][*x]=NULL;

	//esto es cuando hay un articulo en frente de la posicion
	//del jugador
   	if (NULL!=unidades_mapa[*y+dy][*x+dx]) {
        	//colecciona el articulo
        	unidades *dunidad=unidades_mapa[*y+dy][*x+dx];
        	dunidad->hp--;
		articulos_tomados++;

        	if (0 < dunidad->hp) {
            		dy=0;
            		dx=0;
        		//remueve la referencia del articulo en el mapa 
			//
        	} 
		else {
			unidades_mapa[*y+dy][*x+dx]=NULL;
       		 }
    	}
    	
	//actualiza la posicion del jugador
    	*y+=dy;
   	*x+=dx;

    	//agrega la referencia de la nueva posicion del jugador
    	unidades_mapa[*y][*x]=&unidades_lista[0];
}
