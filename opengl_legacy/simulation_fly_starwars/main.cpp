/*
****************************************************
*	Trabalho Prático 2
*	77431 - Igor de Oliveira Lopes
*	68064 - Michael Canesche	
*
***************************************************
*/

#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <GL/glut.h>
#include "glm.h"
#include "tgaload.h"
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

#define MAX_TEXTURAS 20 //Maximo de texturas
#define MENU 0
#define MILLENIUM 1
#define XWING 2
#define CITY 3
#define REPUBLIC 4
#define ATAT 5
#define EXECUTOR 6
#define TERRENO 7
#define HELIPORTO 8
#define SPACE 9
#define TELA 10
#define LASERVERDE 11
#define LASERAZUL 12
#define LASERVERMELHO 13
#define INTERCEPTOR 14
#define CRAWLER 15
#define SEMTEXTURA 16

float angle = 0.0f; // angulo de rotacao da direcao da camera
float dx = 0.0f,dy = 1.0f, dz = -1.0f,d = 0, r = 0; // vetor da direcao da camera 
static float x = 0.0f, y = 0.0f,z = 0.0f ; // posicao da camera XZ
static float angulo,velocidade = 1.0;

float deltaAngle = 0.0f;
float deltaMove = 0, alfaMove = -1.8;
bool menu = false,fora = false,telaInicial = true, flat = false, smooth = false,
moveu = false,ilum = false, ilum1 = true, ilum2 = false, pause = false,textura =false; 

GLuint objeto = -1;
static GLuint shader = GLM_FLAT;

// modelos
GLMmodel* city = NULL;
GLMmodel* millenium = NULL;
GLMmodel* hercules = NULL;
GLMmodel* xwing = NULL;
GLMmodel* executor = NULL;
GLMmodel* republic = NULL;
GLMmodel* atat = NULL;
GLMmodel* terreno = NULL;
GLMmodel* heliporto = NULL;
GLMmodel* space = NULL;
GLMmodel* laserVermelho = NULL;
GLMmodel* laserVerde = NULL;
GLMmodel* laserAzul = NULL;
GLMmodel* interceptor = NULL;
GLMmodel* crawler = NULL;
GLMmodel* aattank = NULL;

// caminhos dos objetos
static char path_hercules[] = {"Data/hercules/hercules-airplane.obj"};
static char path_millenium[] = {"Data/millenium/Millennium_Falcon.obj"};
static char path_xwing[] = {"Data/xwing/x.obj"};
static char	path_executor[] = {"Data/crusador/source/Star_destroyer.obj"};
static char path_city[] = {"Data/city/nubian_complex.obj"};
static char path_republic[] = {"Data/republicgunship/gunship.obj"};
static char path_atat[] = {"Data/atat/atat.obj"};
static char path_terreno[] = {"Data/terreno/source/far_world.obj"};
static char path_heliporto[] = {"Data/heliporto/heliporto.obj"};
static char path_space[] = {"Data/space/space.obj"};
static char path_laserVermelho[] = {"Data/laserVermelho/laser.obj"};
static char path_laserVerde[] = {"Data/laserVerde/laser.obj"};
static char path_laserAzul[] = {"Data/laserAzul/laser.obj"};
static char path_interceptor[] ={"Data/starwarstieinterceptor/TieInterceptor.obj"};
static char path_crawler[] ={"Data/JAWA/Star_wars_JAWA.obj"};
static char path_aattank[] ={"Data/aattank/StarWarsAATTANK.obj"};

GLfloat mat_ambient[ ] = { 1.0,1.0,1.0, 1.0 };
GLfloat mat_specular[ ] = { 0.6, 0.6,0.6, 1.0 };
GLfloat mat_difuse[ ] = { 0.5, 0.5,0.5, 1.0 };
GLfloat mat_shininess[ ] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess2[ ] = { 50.0f };
GLfloat light_position[ ] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position2[ ] = { 1.0, 1.0, 1.0, 0.0f };
GLfloat white_light[ ] = { 1.0, 1.0, 1.0, 1.0 };

//Vetor com os numeros das texturas
GLuint texture_id[MAX_TEXTURAS];

struct Laser{
	float xpos, ypos,zpos;
	int velocidade = 2.5;
	bool desenha = true;
	float anguloRotacao;
	float angulo;
};

vector<Laser> dMillenium ;
vector<Laser> dXwing ;
vector<Laser> dRepublic ;
Laser lmillenium;
Laser lXwing;
Laser lRepublic;
static float dxx,dzz;

Mix_Chunk *laserEffectMillenium;
Mix_Chunk *laserEffectXwing ;
Mix_Chunk *laserEffectRepublic ;
Mix_Music *abertura;
Mix_Music *background;


void menuComandos(void){
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, texture_id[MENU]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-45, -45, -70);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-45, 45, -70);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(45, 45, -70);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(45, -45, -70);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void tela(void){
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, texture_id[TELA]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-45, -45, -70);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-45, 45, -70);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(45, 45, -70);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(45, -45, -70);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void carregaTextura(int x, char* path,image_t temp_image) {
	DIR* dirFile = opendir( path );
	if ( dirFile ) 
	{
		struct dirent* hFile;
		while (( hFile = readdir( dirFile )) != NULL ) 
		{
			if ( !strcmp( hFile->d_name, "."  )) continue;
			if ( !strcmp( hFile->d_name, ".." )) continue;
			char nome [(strlen(path)+strlen(hFile->d_name))]; 
			strcpy(nome, path ); 
			if ( strstr( hFile->d_name, ".tga" )){
				strcat(nome, hFile->d_name);
				glBindTexture(GL_TEXTURE_2D,texture_id[x]);
				tgaLoad(nome, &temp_image, TGA_FREE | TGA_LOW_QUALITY);
			}
		} 
	}
	closedir( dirFile );
}

void iniciaTexturas(void){
	image_t temp_image;
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(MAX_TEXTURAS, texture_id);

	carregaTextura(MILLENIUM,"Data/millenium/",temp_image);
	carregaTextura(XWING,"Data/xwing/",temp_image);
	carregaTextura(EXECUTOR,"Data/crusador/textures/",temp_image);
	carregaTextura(REPUBLIC,"Data/republicgunship/",temp_image);
	carregaTextura(ATAT,"Data/atat/",temp_image);
	carregaTextura(MENU,"Data/menu/",temp_image);
	carregaTextura(TERRENO,"Data/terreno/textures/",temp_image);
	carregaTextura(HELIPORTO,"Data/heliporto/",temp_image);
	carregaTextura(SPACE,"Data/space/",temp_image);
	carregaTextura(TELA,"Data/tela/",temp_image);
	carregaTextura(LASERVERMELHO,"Data/laserVermelho/",temp_image);
	carregaTextura(LASERVERDE,"Data/laserVerde/",temp_image);
	carregaTextura(LASERAZUL,"Data/laserAzul/",temp_image);
	carregaTextura(INTERCEPTOR,"Data/starwarstieinterceptor/",temp_image);
	carregaTextura(CITY,"Data/city/Maps/",temp_image);
}

void mudaTamanho(int w, int h) {

	// Previne divisao de zero (janela muito pequena)
	h = (h == 0) ? 1 : h;
	float relacao =  w * 1.0 / h;
	// Usa a projecao da matriz
	glMatrixMode(GL_PROJECTION);
	// Limpa Matriz
	glLoadIdentity();
	// Defina a janela de exibicao como a janela inteira
	glViewport(0, 0, w, h);
	// Define a correta pespectiva.
	gluPerspective(65.0f, relacao, 0.1, 150.0f);
	// Retorna para o ModelView
	glMatrixMode(GL_MODELVIEW);
}

// Funcao responsavel por desenhar o modelo e aplicar o shader
void drawmodel(GLMmodel** model, char* ender, int i){
	if (!(*model)) {
		*model = glmReadOBJ(ender);	
		if (!(*model)) exit(0);
		glmUnitize(*model);
		glmFacetNormals(*model);
		glmVertexNormals(*model, 90.0);
	}
	if(!textura){
		glmDraw(*model, shader | GLM_MATERIAL);

	}else{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);

		glmDraw(*model, GLM_TEXTURE | shader | GLM_MATERIAL);

		glDisable(GL_TEXTURE_2D);
	}

}

void processaPos(float deltaMove) {
	
	r = float(sqrt(pow(86,2)-pow(y,2))) ;
	
	if( d < r){
		d = float (sqrt(pow(x,2)+pow(z,2))) ;

		x += deltaMove * dx * 0.1f *velocidade;
		z += deltaMove * dz * 0.1f *velocidade;
	}else {
		if(z < 0) {
			z += abs(d-r)+1.5;
			d = float(sqrt(pow(x,2)+pow(z,2)))-3.0 ; 
		}
		else if(z >= 0 ){
			z -= abs(d-r)+2;
			d = float(sqrt(pow(x,2)+pow(z,2)))-3.0; 
		}	
	}
	
}

void processaPosf(float deltaMove){

	r = float(sqrt(pow(84,2)-pow(y,2))) ;
	
	if ( d < r) {
		d = float (sqrt(pow(x,2)+pow(z,2))) ;
		x += deltaMove * sin(angle*0.9545) * 0.1f *velocidade;
		z += deltaMove * (-cos(angle*0.9545)) * 0.1f *velocidade;
	} else {
		if(z < 0) {
			z += abs(d-r)+1;
			d = float(sqrt(pow(x,2)+pow(z,2)))-3.0 ; 
		}
		else if(z >= 0 ){
			z -= abs(d-r)+1;
			d = float(sqrt(pow(x,2)+pow(z,2)))-3.0; 
		}
	}
}

void processaDir(float deltaAngle) {
	angle += deltaAngle;
	dx = sin(angle);
	dz = -cos(angle);
}

void desenhaMillenium(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[MILLENIUM]);
	glScaled(0.2f,0.2f,0.2f);
	glRotatef(180.0f,0,1,0);
	drawmodel(&millenium,path_millenium,MILLENIUM);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void desenhaXwing(){
	glPushMatrix();	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[XWING]);
	drawmodel(&xwing,path_xwing,XWING);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void desenhaCrawler(){
	glPushMatrix();	
	
	drawmodel(&crawler,path_crawler,SEMTEXTURA);
	glPopMatrix();
}
void desenhaAattank(){
	glPushMatrix();	
	drawmodel(&aattank,path_aattank,SEMTEXTURA);
	glPopMatrix();
}
void desenhaRepuplic(){
	glPushMatrix();	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[REPUBLIC]);
	drawmodel(&republic,path_republic,REPUBLIC);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaExecutor(){
	glPushMatrix();	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[EXECUTOR]);
	drawmodel(&executor,path_executor,EXECUTOR);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void desenhaAt(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[ATAT]);
	drawmodel(&atat,path_atat,ATAT);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void desenhaTerreno(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[TERRENO]);
	drawmodel(&terreno,path_terreno,TERRENO);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaHeliporto(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[HELIPORTO]);
	drawmodel(&heliporto,path_heliporto,HELIPORTO);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaSpace(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[SPACE]);
	drawmodel(&space,path_space,SPACE);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaLaserVermellho(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[LASERVERMELHO]);
	drawmodel(&laserVermelho,path_laserVermelho,LASERVERMELHO);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaLaserVerde(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[LASERVERDE]);
	drawmodel(&laserVerde,path_laserVerde,LASERVERDE);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaLaserAzul(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[LASERAZUL]);
	drawmodel(&laserAzul,path_laserAzul,LASERAZUL);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaInterceptor(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[INTERCEPTOR]);
	drawmodel(&interceptor,path_interceptor,INTERCEPTOR);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void desenhaCity(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id[CITY]);
	drawmodel(&city,path_city,CITY);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void laserDispara(Laser temp,vector<Laser> *disparos){
	disparos->push_back(temp);
}

// Função dispara ainda necessita de reparos
void dispara(vector<Laser> *disparos){
	vector<Laser> &temp = *disparos;

	for(int i=0; i < temp.size(); i++)
	{
		if(temp[i].desenha){
			if( temp[i].zpos > -120){
				dxx = sin(temp[i].angulo);
				dzz = -cos(temp[i].angulo);
				temp[i].zpos += 8 * dzz * 0.14f ;
				temp[i].xpos += 8 * dxx * 0.14f ;
			}else{
				temp[i].desenha = false;
				temp.erase(temp.begin(),temp.begin()+i);
			}		
			glPushMatrix();
			glTranslatef(temp[i].xpos, temp[i].ypos,temp[i].zpos);
			glRotatef(temp[i].anguloRotacao,0,1,0);

			if(objeto == 0)
				desenhaLaserVerde();
			if(objeto == 1)
				desenhaLaserVermellho();

			if(objeto == 2)
				desenhaLaserAzul();

			glPopMatrix();
		}
	}
}

void iluminacao(void){
	if(ilum){
		glClearColor (1.0, 1.0, 1.0, 1.0);

		if(ilum1){	// iluminacao omnidirecional	
			glLightfv(GL_LIGHT0, GL_POSITION, light_position2);
			glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_difuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, mat_shininess);
	
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_COLOR_MATERIAL);

		} else {
			glDisable(GL_LIGHT0);
		} 

		if(ilum2) { // spotlight
			GLfloat spotlightpos[ ] = {x,alfaMove+0.45f, z, 1.0f};
			GLfloat spotlightp[ ] = {dx,0, dz, 1.0f};

			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightp);
			glLightfv(GL_LIGHT1, GL_AMBIENT, mat_ambient);

			glLightfv(GL_LIGHT1, GL_POSITION, spotlightpos);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
			glLightfv(GL_LIGHT1, GL_SPECULAR, mat_shininess);

			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f );
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION , 0.0014f );
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION , 0.000007f );

			glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0f);

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT1);
			glEnable(GL_COLOR_MATERIAL);
		} else {
			
			glDisable(GL_LIGHT1);
			
		}
	} else {
		glDisable(GL_LIGHTING); // tira a luz ambiente padrao que vem inerente ao objeto 
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_COLOR_MATERIAL);

	}
}


void desenhaCena(void) {
	glClearColor (0.0f,0.0f,0.0f,0.0f);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity (); 
	if(!telaInicial){

		if(!menu){
			if (deltaMove || moveu){
				if(!fora)
					processaPos(deltaMove);
				else
					processaPosf(deltaMove);
				moveu = false;
			}
			if (deltaAngle)
				processaDir(deltaAngle);

			if(objeto == -1) // escolha do objeto inicial
				gluLookAt(0,-0.2, -2.4, 0, -2.5, -3, 0.0f, 1.0f,  0.0f);
			else if(!fora)
				gluLookAt(x,alfaMove+0.5f, z,x+dx,alfaMove+0.5f, z+dz,0.0f, 1.0f,  0.0f);
			else 
				gluLookAt(x,alfaMove+0.7f, z,x+dx,alfaMove+0.5f, z+dz,0.0f, 1.0f,  0.0f);

			// Chama iluminacao
			iluminacao();
			
			// Desenha Terreno
			glPushMatrix();
			glTranslated(0,2,0);
			glScaled(90,80,90);
			desenhaTerreno();
			glPopMatrix();
			
			// Desenha Space
			glPushMatrix();
			glTranslated(0,20,0);
			glScaled(90,70,90);
			desenhaSpace();
			glPopMatrix();
			
			// Desenha Helioporto
			for(int i = -1; i <= 1; i++){
				glPushMatrix();
				glTranslated(0.5*i,-1.45f,-3);
				glScaled(0.23,0.23,0.23);
				desenhaHeliporto();
				glPopMatrix();
			}
			if(objeto != 0){ // nave millenium
				glPushMatrix();
				glTranslated(-0.5,-1.4,-3);
				desenhaMillenium();
				glPopMatrix();
			} else{
				
				glPushMatrix();
				if(!fora){
					y = alfaMove+0.44;
					angulo = -(angle*57.45);
					lmillenium.xpos = x;
					lmillenium.ypos = y;
					lmillenium.zpos = z;
					lmillenium.anguloRotacao = angulo;
					lmillenium.angulo = angle;
					glTranslatef(x,y,z);
					glRotatef(angulo,0,1,0);
				}
				else{
					y = alfaMove+0.439;
					angulo = -(angle*57.45);
					lmillenium.xpos = x+dx;
					lmillenium.ypos = y;
					lmillenium.zpos = z+dz;
					lmillenium.anguloRotacao = angulo;
					lmillenium.angulo = angle;
					glTranslatef(x+dx,y,z+dz);
					glRotatef(angulo,0,1,0);
					glScaled(2.5,2.5,2.5);
					glTranslatef(0,0,0);
				}
				desenhaMillenium();
				glPopMatrix();
				dispara(&dMillenium);


			}
			if(objeto != 1){ // nave xwing
				
				glPushMatrix();
				glTranslatef(0.0,-1.4,-3);
				glRotatef(180.0f,0,1,0);
				desenhaXwing();
				glPopMatrix();
			}else{
				glPushMatrix();
				if(!fora){
					y = alfaMove+0.44;
					angulo = -(angle*57.45);
					lXwing.xpos = x;
					lXwing.ypos = y;
					lXwing.zpos = z;
					lXwing.anguloRotacao = angulo;
					lXwing.angulo = angle;
					glTranslatef(x,y,z);
					glRotatef(angulo,0,1,0);	
				}
				else{
					y = alfaMove+0.42;
					angulo = -(angle*57.45);
					lXwing.xpos = x+dx;
					lXwing.ypos = y;
					lXwing.zpos = z+dz;
					lXwing.anguloRotacao = angulo;
					lXwing.angulo = angle;
					glTranslatef(x+dx,y,z+dz);
					glRotatef(angulo,0,1,0);
					glScaled(2.5,2.5,2.5);
					glTranslatef(0,0,0);
				}
				glRotatef(180.0f,0,1,0);
				desenhaXwing();
				glPopMatrix();
				dispara(&dXwing);
			} 

			if(objeto != 2){ // nave Republic
				glPushMatrix();
				glTranslatef(0.5,-1.4,-3);
				glRotatef(180.0f,0,1,0);
				glScaled(0.15f,0.15f,0.15f);
				desenhaRepuplic();
				glPopMatrix();
			}else{
				glPushMatrix();
				if(!fora){
					y = alfaMove+0.44;
					angulo = -(angle*57.45);
					lRepublic.xpos = x;
					lRepublic.ypos = y;
					lRepublic.zpos = z;
					lRepublic.anguloRotacao = angulo;
					lRepublic.angulo = angle;
					glTranslatef(x,y,z);
					glRotatef(angulo,0,1,0);
					glScaled(2.5,2.5,2.5);
				} else {
					y = alfaMove+0.40;
					angulo = -(angle*57.45);
					lRepublic.xpos = x+dx;
					lRepublic.ypos = y;
					lRepublic.zpos = z+dz;
					lRepublic.anguloRotacao = angulo;
					lRepublic.angulo = angle;
					glTranslatef(x+dx,y,z+dz);
					glRotatef(angulo,0,1,0);
					glScaled(6,6,6);
					glTranslatef(0,0,0);
				}
				glScaled(0.1f,0.1f,0.1f);	
				glRotatef(180.0f,0,1,0);
				desenhaRepuplic();
				glPopMatrix();
				dispara(&dRepublic);
			}


			glPushMatrix();
			glTranslatef(-8,-0.55f,-4);
			glRotatef(125.0f,0,1,0);
			desenhaAt();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-8,-0.65f,-8);
			glRotatef(125.0f,0,1,0);
			desenhaAt();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-8,-0.8f,-12);
			glRotatef(125.0f,0,1,0);
			desenhaAt();
			glPopMatrix();

			glPushMatrix();
			glRotatef(80.0f,0,1,0);
			glTranslatef(-5,23,-35);
			glScaled(8,8,8);
			desenhaExecutor();
			glPopMatrix();

			glPushMatrix();
			glRotatef(90.0f,0,1,0);
			glTranslatef(-12,23,-40);
			glScaled(8,8,8);
			desenhaExecutor();
			glPopMatrix();

			glPushMatrix();
			glRotatef(90.0f,0,1,0);
			glTranslatef(-12,26,-42);
			desenhaInterceptor();
			glPopMatrix();

			glPushMatrix();
			glRotatef(90.0f,0,1,0);
			glTranslatef(-8,26,-44);
			desenhaInterceptor();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(22,4.5f,-34);
			glScaled(9,9,9);
			desenhaCity();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-16,0.9f,-20);
			glScalef(0.30,0.30,0.30);
			glRotatef(80.0f,0,1,0);
			desenhaCrawler();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-15,1.0f,-21);
			glScalef(0.30,0.30,0.30);
			glRotatef(80.0f,0,1,0);
			desenhaCrawler();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-17,0.9f,-20);
			glScalef(0.30,0.30,0.30);
			glRotatef(80.0f,0,1,0);
			desenhaCrawler();
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(10,-1.05f,-5);
			glRotatef(320.0f,0,1,0);
			desenhaAattank();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(10,-0.9f,-7);
			glRotatef(320.0f,0,1,0);
			desenhaAattank();
			glPopMatrix();


		}else if(menu){
			menuComandos();
		}
	} else {
		
		tela();
		
	}
	glutSwapBuffers();
}
void encerraSom(){
	Mix_FreeChunk(laserEffectMillenium);
	Mix_FreeChunk(laserEffectXwing);
	Mix_FreeChunk(laserEffectRepublic);
	Mix_CloseAudio();
	SDL_Quit();
}
void iniciaSom(){
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	abertura = Mix_LoadMUS("Sounds/abertura.wav");
	laserEffectXwing =Mix_LoadWAV("Sounds/LaserEffectXwing.wav");
	laserEffectMillenium =Mix_LoadWAV("Sounds/LaserEffect.wav");
	laserEffectRepublic =Mix_LoadWAV("Sounds/LaserEffectRepublic.wav");
	background = Mix_LoadMUS("Sounds/background.wav");
	Mix_PlayMusic(abertura, -1);
}
void posicaoInicial(){
	alfaMove = -1.8;
	angle = 0;
	dx = 0;
	dz = -1;
}

void teclado(unsigned char key, int xx, int yy){

	switch (key) {
		case ' ': // espaco
		if(objeto == 0){
			laserDispara(lmillenium,&dMillenium);			
			Mix_PlayChannel(-1, laserEffectMillenium, 0);
		}
		else if(objeto == 1){
			laserDispara(lXwing,&dXwing);
			Mix_PlayChannel(-1, laserEffectXwing, 0);
		}
		else if(objeto == 2){
			laserDispara(lRepublic,&dRepublic);
			Mix_PlayChannel(-1, laserEffectRepublic, 0);
		}
		break;
		
		case 49: // numero 1
		ilum1 = !ilum1;
		break;
		case 50: // numero 2
		ilum2 = !ilum2;
		break;
		case 27: // ESC
		encerraSom(); exit(0);
		break;
		case 's': case 'S': 
		if(alfaMove<50) alfaMove += 1.0f; moveu = true; break;
		case 'd': case 'D':
		if(alfaMove>-1.0) alfaMove += -1.0f; moveu = true; break;
		case 'c': case 'C':
		fora = false; break;
		case 'v': case 'V':
		fora = true; break;
		case 'm': case 'M':
		menu = !menu; break;
		case 'l': case 'L':
		textura = ! textura;
		ilum = !ilum; 
		break;
		case 13: 
		telaInicial = false;
		Mix_HaltMusic();
		Mix_PlayMusic(background, -1);
		break;
		case 'p': case 'P':
		pause = !pause;
		if(pause) Mix_ResumeMusic();
		else Mix_PauseMusic();
		break;
		case 'g': case 'G':
		shader = GLM_SMOOTH; 
		glShadeModel(GL_SMOOTH);
		break;
		case 'f': case 'F':
		shader = GLM_FLAT;
		glShadeModel(GL_FLAT);

		break;
	}

}

void apertaKey( int key, int xx, int yy) {
	switch (key) {
		case 27: exit(0);
		case GLUT_KEY_F4:
		velocidade +=1;
		break;
		case GLUT_KEY_F5:
		if(velocidade-0.4 >= 1.0)
			velocidade -=0.4;
		break;
		case GLUT_KEY_F1:
		if(objeto != 0){
			x = -0.5;z = -3;objeto = 0;
			posicaoInicial();
		}
		break;
		case GLUT_KEY_F2:
		if(objeto != 1){
			x = 0.0;z = -3.0;objeto = 1;
			posicaoInicial();
		}
		break;
		case GLUT_KEY_F3:
		if(objeto != 2){
			x = 0.5;z = -3; objeto = 2;
			posicaoInicial();
		}
		break;
		case GLUT_KEY_LEFT : deltaAngle = -0.04f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.04f; break;
		case GLUT_KEY_UP :deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
}

void soltaKey(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = 0.0f;break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP : deltaMove = 0.0f;break;
		case GLUT_KEY_DOWN : deltaMove = 0.0f;break;
	}
}


int main(int argc, char **argv) {

	// inicia GLUT e cria janela
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(500,500);
	glutCreateWindow("Trabalho Final");

	iniciaTexturas();
	iniciaSom();
	// registro callbacks
	glutDisplayFunc(desenhaCena);
	glutReshapeFunc(mudaTamanho);
	glutIdleFunc(desenhaCena);
	glutSpecialFunc(apertaKey);

	// Aqui estao as novas entradas
	glutKeyboardFunc(teclado);
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(soltaKey);

	// inicia OpenGL 
	glEnable(GL_DEPTH_TEST);
	
	// processa o ciclo de de eventos do GLUT
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess2);

	glutMainLoop();

	return 1;
}
