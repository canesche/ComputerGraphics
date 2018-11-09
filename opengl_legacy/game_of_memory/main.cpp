/*
	Trabalho 1 - INF 390
	Estudante: Michael Canesche - 68064
	Prof.: Marcus V. A. Andrade  
*/

#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <iostream>
using namespace std;

// Constantes
#define PARES 		0
#define GENIUS 		1 

#define PI 3.1415926535898 

// Variaveis globais
char texto[30];
GLfloat win;
GLint view_w, view_h, game = -1, dif, row, col;

// VARIAVEIS GLOBAIS PARA O JOGO PARES
int** mPARES;
int** mPARESse;
int click, perdeu, ganhou;
int linha, coluna;
int op1x, op1y, op2x, op2y;
bool errou;

// VARIAVEIS GLOBAIS PARA O JOGO GENIUS
int* seq, *seqd;
int nSeq, n, opcao;
bool jogada, start, suaVez;
float tempo;

// delay em segundos
void delay(float secs){
	float end = clock()/CLOCKS_PER_SEC + secs;
	while((clock()/CLOCKS_PER_SEC) < end);
}

// declara vetor de sequencia
void criaSequencia(){
	seq = new int[25];
	
	srand((unsigned)time(NULL));

	int i;
	for(i = 0; i < 25; ++i){
		seq[i] = rand() % (row*col);
		//cout << seq[i] << " ";  
	}
	//cout << endl;
	nSeq = 1;
}

// declara matriz dinamica
void criaMatriz(int r, int c){
	mPARES = new int*[c];
	mPARESse = new int*[c];
	for(int i = 0; i < c; i++){
		mPARES[i] = new int[r];
		mPARESse[i] = new int[r];
	}

	// setando a matriz com -1
	for(int i = 0; i < c; i++)
		for(int j = 0; j < r; j++)
			mPARES[i][j] = mPARESse[i][j] = -1;

	// alimentacao seed
	srand((unsigned)time(NULL));

	int count;
	for(int i = 0; i < c; i++)
		for(int j = 0; j < r; j++){
			do {
				count = 0;
				mPARES[i][j] = rand() % (r*c/2);
				for(int k = 0; k < c; k++){
					for(int l = 0; l < r; l++){
						if(mPARES[k][l] == mPARES[i][j])
							count++;
						else if(mPARES[k][l] == -1)
							break; 
					}
				}
			} while(count > 2); // caso tenha mais do que dois elementos repetidos, gera nova combinacao
		}

	// desenha no terminal a matriz
	/*for(int i = c-1; i >= 0; --i){
		for(int j = 0; j < r; ++j){
			cout << mPARES[i][j] << " ";
		}
		cout << endl;
	}*/
}

// funcao que colore o quadro
void ColoreQuadrado(int x, int y, float r, float g, float b){
	float dimX = 1.0f*view_h/col;
	float dimY = 1.0f*view_w/row;
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
        glVertex2f(y*dimY, x*dimX);
		glVertex2f((y+1)*dimY, x*dimX);
        glVertex2f((y+1)*dimY, (x+1)*dimX);
		glVertex2f(y*dimY, (x+1)*dimX);                
    glEnd();
}

// Funcao que desenha um quadrado
void DesenhaQuadrado(int x, int y, float r, float g, float b){
	float dimX = 1.0f*view_h/col;
	float dimY = 1.0f*view_w/row;
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
		glVertex2f(y*dimY+20, x*dimX+20);
		glVertex2f((y+1)*dimY-20, x*dimX+20);
		glVertex2f((y+1)*dimY-20, (x+1)*dimX-20);
		glVertex2f(y*dimY+20, (x+1)*dimX-20);               
    glEnd();
}

void DesenhaPipa(int x, int y, float r, float g, float b){
	float dimX = 1.0f*view_h/col;
	float dimY = 1.0f*view_w/row;
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
		glVertex2f(y*dimY+20, x*dimX+20);
		glVertex2f(y*dimY+20, (x+1)*dimX-20);
		glVertex2f((y+1)*dimY-20, x*dimX+20);
		glVertex2f((y+1)*dimY-20, (x+1)*dimX-20);               
    glEnd();
}

// Funcao que desenha um triangulo
void DesenhaTriangulo(int x, int y, float r, float g, float b){
	float dimX = 1.0f*view_h/col;
	float dimY = 1.0f*view_w/row;
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLES);
		glVertex2f(y*dimY+20, x*dimX+20);
		glVertex2f(y*dimY+dimY/2.0f, (x+1)*dimX-20);
		glVertex2f((y+1)*dimY-20, x*dimX+20);              
    glEnd();
}

// Funcao que desenha um losango
void DesenhaLosango(int x, int y, float r, float g, float b){
	float dimX = 1.0f*view_h/col;
	float dimY = 1.0f*view_w/row;
	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
		glVertex2f(y*dimY+20, x*dimX+dimX/2.0f);
		glVertex2f(y*dimY+dimY/2.0f, (x+1)*dimX-20);
		glVertex2f((y+1)*dimY-20, x*dimX+dimX/2.0f);
		glVertex2f(y*dimY+dimY/2.0f, x*dimX+20);               
	glEnd();
}

// Funcao que desenha um circulo
void DesenhaCirculo(int x, int y, float r, float g, float b){
	float angle;
	float dimX = 1.0f*view_h/col;
	float dimY = 1.0f*view_w/row;
    int i, raio = 0.07f*(view_h+view_w)/2.0f, circle_points = 40;
	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
        for(i = 0; i < circle_points; i++){
            angle = 2*PI*i/circle_points;
            glVertex2f((y+1/2.0f)*dimY+raio*cos(angle), (x+1/2.0f)*dimX+raio*sin(angle));
        }
    glEnd();
}

// Desenha um texto na janela GLUT
void DesenhaTexto(char *string) {  
	glColor3f(.6f,.6f,.6f);
	int tam = strlen(string);
	glBegin(GL_QUADS);
		glVertex2f(0.425f*view_w-10, 0.5f*view_h-20);
		glVertex2f(0.425f*view_w-10, 0.5f*view_h+20); 
		glVertex2f(0.45f*view_w+10*tam+10, 0.5f*view_h+20);
		glVertex2f(0.45f*view_w+10*tam+10, 0.5f*view_h-20);       
	glEnd();
	glColor3f(0.0f,0.0f,0.0f);  
	glPushMatrix();
        // Posicao no universo onde o texto sera colocado          
        glRasterPos2f(0.425f*view_w,0.48f*view_h); 
        // Exibe caracter a caracter
        while(*string) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++); 
	glPopMatrix();
}

void DesenhaFiguras(int valor = 0){
	int i, j, tipo;
	switch(game){
		case 0:
			for(i = 0; i < col; ++i){
				for(j = 0; j < row; ++j){
					if(mPARESse[i][j] == 1)
						ColoreQuadrado(i,j,0,1,0);
					if(mPARESse[i][j] == 0)
						ColoreQuadrado(i,j,1,1,0);
					if(mPARESse[i][j] == 2)
						ColoreQuadrado(i,j,1,0,0);
					if(mPARESse[i][j] == 0 || mPARESse[i][j] == 1 || mPARESse[i][j] == 2){
						switch(mPARES[i][j]){
							case 0: DesenhaQuadrado(i,j,0.3,0,0.6); break;
							case 1: DesenhaTriangulo(i,j,0.8,0.4,0); break;
							case 2: DesenhaLosango(i,j,0,0.2,0.8); break;
							case 3: DesenhaCirculo(i,j,0.1,0.7,0.1); break;
							case 4: DesenhaPipa(i,j,0.9,0.3,0.8); break;
							case 5: DesenhaTriangulo(i,j,0,0.8,0.8); break;
							case 6: DesenhaLosango(i,j,0.6,0.6,0); break;
							case 7: DesenhaCirculo(i,j,0.3,0.3,0.3); break;
							case 8: DesenhaQuadrado(i,j,0.6,0.6,0.2); break;
							case 9: DesenhaPipa(i,j,0.1,0.5,0.1); break;
						}
					}
				}
			}
			break;
		case 1:
			if(opcao != -1){
				switch(opcao){
					case 0: DesenhaQuadrado(0,0,1,0,0); break;
					case 1: DesenhaTriangulo(0,1,0,1,0); break;
					case 2: DesenhaLosango(1,0,0,0,1); break;
					case 3: DesenhaCirculo(1,1,0,1,1); break;
					case 4: DesenhaPipa(2,0,0.5,0.5,0.5); break;
					case 5: DesenhaTriangulo(0,2,0,0.5,0.5); break;
					case 6: DesenhaLosango(2,1,0.5,0.5,0); break;
					case 7: DesenhaCirculo(1,2,0,0,0); break;
					case 8: DesenhaQuadrado(2,2,0,0,0); break;
				}
			} else if(!jogada) {
				if(n >= nSeq){
					jogada = true;
				} else {
					switch(seq[n]){
						case 0: DesenhaQuadrado(0,0,1,0,0); break;
						case 1: DesenhaTriangulo(0,1,0,1,0); break;
						case 2: DesenhaLosango(1,0,0,0,1); break;
						case 3: DesenhaCirculo(1,1,0,1,1); break;
						case 4: DesenhaPipa(2,0,0,0,0); break;
						case 5: DesenhaTriangulo(0,2,0,0,0); break;
						case 6: DesenhaLosango(2,1,0,0,0); break;
						case 7: DesenhaCirculo(1,2,0,0,0); break;
						case 8: DesenhaQuadrado(2,2,0,0,0); break;
					}
				}
				n++;
			}
			break; 
	}
}

void DesenhaTabuleiro(){
	int i;
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	
	for(i = 0; i < col; ++i){ // linhas verticais
		glVertex2f(0, i*(1.0f*view_h/col));
		glVertex2f(view_w, i*(1.0f*view_h/col));            
    }
	for(i = 0; i < row; ++i){ // linhas horizontais
		glVertex2f(i*(1.0f*view_w/row), 0);
		glVertex2f(i*(1.0f*view_w/row), view_h);            
    } 
    glEnd();
}

// Funcao callback chamada para fazer o desenho
void Desenha(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
   	glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o jogo   
    switch (game) {
		case 0: // PARES 
			DesenhaFiguras();
			DesenhaTabuleiro();
			if(ganhou == row*col) { 
				DesenhaTexto((char *)"Parabens!...");
			} else if(perdeu >= 5) { 
				DesenhaTexto((char *)"Perdeu!...");
			}
            break;
		case 1: // GENIUS
			if(start){
				DesenhaTabuleiro();
				if(nSeq > 25){
					ganhou = 1;
					DesenhaTexto((char *)"Parabens!...");
				} else {
					DesenhaFiguras();
				}
				if(errou){
					DesenhaTexto((char *)"Perdeu!...");
				}
			} else {
				DesenhaTexto((char *)"Start?");
				click = n = ganhou = 0;
				jogada = errou = start = false;
				suaVez = true;
				tempo = 1.0f;
			}                       
            break;
    }
     
	glutSwapBuffers();
	//
	switch (game) {
        case 0: // PARES 
			if(errou){
				errou = false;
				mPARESse[op1x][op1y] = -1;
				mPARESse[op2x][op2y] = -1;
				delay(1);
			}
			break;
		case 1: // GENIUS 
			
			if (opcao != -1) {
				delay(0.001);
				DesenhaTabuleiro();
				opcao = -1;
			} else if (!suaVez) {
				jogada = false;
				suaVez = true;
				delay(0.001);
				glClear(GL_COLOR_BUFFER_BIT);
				DesenhaTabuleiro();
				DesenhaTexto((char *) "NEXT..." );
				glutSwapBuffers();
				delay(0.001);	
			} else if(!jogada && start && ganhou != 1){
				delay(tempo);
				glClear(GL_COLOR_BUFFER_BIT);
				DesenhaTabuleiro();
				glutSwapBuffers();
				delay(tempo-0.1);
			}                   
            break;
	}	
	glutPostRedisplay();
}

// Gerenciamento do menu GENIUS com dificuldade          
void MenuGenius(int op){
	
	click = 0;
	ganhou = 0;
	perdeu = 0;
	start = false;
	
	game = GENIUS;
	switch(op) {
		case 0: row = 2; col = 2; break;
		case 1: row = 3; col = 3; break;
	}
	criaSequencia();
	glutPostRedisplay();
} 

// Gerenciamento do menu PARES com dificuldade           
void MenuPares(int op){

	click = ganhou = perdeu = n = 0;
	jogada = errou = start = suaVez = false;
	game = PARES;
	tempo = 1.0f;
	switch(op) {
		case 0: row = 3; col = 4; break;
		case 1: row = 4; col = 5; break;
	}
	criaMatriz(row,col);
    glutPostRedisplay();
}       
        
// Gerenciamento do menu principal           
void MenuPrincipal(int op){
	// responsavel em sair do jogo
	if(op == 1) exit(0);
}

// Criacao do Menu
void CriaMenu() {
    int menu, submenu1, submenu2;

    submenu1 = glutCreateMenu(MenuPares);
    glutAddMenuEntry("Facil",0);
    glutAddMenuEntry("Dificil",1);

    submenu2 = glutCreateMenu(MenuGenius);
    glutAddMenuEntry("Facil",0);
    glutAddMenuEntry("Dificil",1);

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Formar os pares",submenu1);
    glutAddSubMenu("Sequencia (Genius)",submenu2);
    glutAddMenuEntry("Sair",1);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glFlush();
}

// Inicializa parametros de rendering
void Inicializa (void){   
    // Define a cor de fundo da janela de visualizacao como branca
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    CriaMenu();
}

// Funcao callback chamada quando o tamanho da janela eh alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h){ 
    // Especifica as dimensoes da Viewport
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;                   

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0, view_w, 0, view_h);
}
           
// Funcao callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y){        
   
    if (button == GLUT_LEFT_BUTTON && game != -1){
		if (state == GLUT_DOWN) {
			start = true;
			linha = (view_h-y)/(view_h/col);	
			coluna = x / (view_w/row) ;

			// correcao de possiveis bugs 
			linha = (linha < 0) ? 0 : linha;
			linha = (linha >= col) ? col-1 : linha;
			coluna = (coluna < 0) ? 0 : coluna;
			coluna = (coluna >= row) ? row-1 : coluna;
			
			switch(game){
				case 0:
					if(mPARESse[linha][coluna] == -1 && perdeu < 5){
						mPARESse[linha][coluna] = 0; 
						if(click == 0){	 
							op1x = linha; op1y = coluna;
						} else {
							op2x = linha; op2y = coluna;
						} 
						++click;
					}
					// verifica se ao clickar, acertou alguma jogada 
    				if(click == 2 && perdeu < 5){
						if(mPARES[op1x][op1y] == mPARES[op2x][op2y]){
							ganhou += 2;
							mPARESse[op1x][op1y] = 1;
							mPARESse[op2x][op2y] = 1;
						} else {
							++perdeu;
							mPARESse[op1x][op1y] = 2;
							mPARESse[op2x][op2y] = 2;
							errou = true;
						}
						click = 0;
					}
					break;
				case 1:
					if(jogada && !errou){
						++click;
						if(linha == 0 && coluna == 0) opcao = 0;
						else if(linha == 0 && coluna == 1) opcao = 1;
						else if(linha == 1 && coluna == 0) opcao = 2;
						else if(linha == 1 && coluna == 1) opcao = 3;
						else if(linha == 2 && coluna == 0) opcao = 4; 
						else if(linha == 0 && coluna == 2) opcao = 5;
						else if(linha == 2 && coluna == 1) opcao = 6;
						else if(linha == 1 && coluna == 2) opcao = 7;
						else if(linha == 2 && coluna == 2) opcao = 8;
						
						if(click > 0 && ganhou != 1){

							if(seq[click-1] != opcao){
								errou = true;
							}
							if(click == nSeq && !errou && ganhou != 1){
								n = click = 0;
								++nSeq;
								suaVez = false;
								// responsavel em diminuir o tempo a cada vitoria
								if (tempo >= 0.2) { 
									tempo -= 0.1;
								} 
							} 
						}
						
					}
					break;
			}
	    }
	}

    glutPostRedisplay();
}

                     
// Programa Principal 
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);     
	glutInitWindowSize(800,600);
	glutInitWindowPosition(10,10);
	glutCreateWindow("Jogos de memoria");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);    
	Inicializa();
	glutMainLoop();
}
