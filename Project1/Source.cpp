#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

#include "texture.h"
#include "figuras.h"
#include "Camera.h"

#include "cmodel/CModel.h"
//#include <CModel.h>

#pragma comment(lib, "CModel.lib")

//bloque de definicion para sonido
//incluyendo biblioteca de irrklang
#include <irrKlang.h>

//enlazando al dll
#pragma comment(lib, "irrKlang.lib")

//arrancando el motor de sonido
irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

//agregando fuente de sonido para el audio de fondo
irrklang::ISoundSource* audioFondo = engine->addSoundSourceFromFile("audios/ambiente lego.mp3");

//agregando fuente de sonido para el audio de pikachu que es lanzado con tecla
irrklang::ISoundSource* audioPikachu = engine->addSoundSourceFromFile("audios/025-Pikachu.wav");

//importante para cargar modelos porque hace referencia al .lib de CModel
//Solo para Visual Studio 2015 o mayor
#if (_MSC_VER >= 1900)
#   pragma comment( lib, "legacy_stdio_definitions.lib" )
#endif


static GLuint ciudad_display_list;	//Display List for the Monito

//variables para cámara
CCamera objCamera;	//Create objet Camera
GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW) 



//variables para texto dibujado
char s[30];
long int font = (long int)GLUT_BITMAP_HELVETICA_18;


//objetos de textura para skybox
CTexture textSkyboxUnica;
CTexture textSkyboxFrontal;
CTexture textSkyboxAtras;
CTexture textSkyboxIzquierda;
CTexture textSkyboxDerecha;
CTexture textSkyboxArriba;
CTexture textSkyboxAbajo;
CTexture arbolt;

//objetos de textura para ciudad
CTexture textPiso;	//textura para piso
CTexture textCaminoCarritos;
CTexture pisoverde;
CTexture tronco;
//texturas casa
CTexture textpuerta;
CTexture textventana;
CTexture textcasamadera;
CTexture texttejado;
CTexture textpiso1;
CTexture textparedint;
CTexture textescalon;
CTexture texttecho;
CTexture textbalon;

CTexture textburo;
CTexture textManija;
CTexture textCajon;
CTexture textPez;
CTexture textlibre;
CTexture textlibros;
CTexture textportada;
CTexture texttele;
CTexture textbaul;
CTexture textboton;
CTexture textmabaul;
CTexture textbase;
CTexture textpantalla;
CTexture textcamas;
CTexture textcobija;
CTexture textalmu;
CTexture textcamabase;




//figuras casa
CFiguras puerta;
CFiguras ventanas;
CFiguras maderacasa;
CFiguras tejado;
CFiguras piso1;
CFiguras paredint;
CFiguras escalon;
CFiguras cuadro;
CFiguras techo;


CFiguras maderaBuro;
CFiguras manija;
CFiguras cajon;
CFiguras pecera;
CFiguras libre;
CFiguras libros;
CFiguras portada;
CFiguras televi;
CFiguras baul;
CFiguras mabaul;
CFiguras antena;
CFiguras boton;
CFiguras base;
CFiguras cilindro;
CFiguras pantalla;
CFiguras camas;
CFiguras almu;
CFiguras mueblecama;
CFiguras cobija;

//objetos de figura
CFiguras figSkybox;
CFiguras figCampo;
CFiguras figCaminoCarritos;
CFiguras figpisoverde;
CFiguras linea;
CFiguras arbol1;
CFiguras balon;

//******************************************************
//******************************************************
//objetos de cmodel para cargar modelos externos en .3ds

CModel Carro;
CModel arbol;
CModel arbol2;
CModel arbRoca;
CModel fuente;
CModel lampara;
CModel carro1;

float open = 0.0;
float bota = 0.0;
bool p_animacion1 = false;
bool p_animacion = false;
bool p_animacion2 = false;
bool g_fanimacion = false;
bool g_avanza = false;
float movKitZ = 0.0;

//******************************************************
//******************************************************


void ciudad(void)
{
	glPushMatrix(); //Campo
		glTranslatef(0.0,-60.1,0.0);
		//el campo mide 100 x 64 pero usaré la mitad de tamaño
		//agrego 4 más por el pasto fuera del campo
		glScalef(130, 0.1, 130);
		glDisable(GL_LIGHTING);
		figCampo.prisma2(textPiso.GLindex, 0);
		glEnable(GL_LIGHTING);
	glPopMatrix();


}



GLuint createDL(void)
{
	GLuint ciudadDL;
	//GLuint cieloDL;

	// Create the id for the list
	ciudadDL = glGenLists(1);
	// start list
	glNewList(ciudadDL, GL_COMPILE);
	// call the function that contains 
	// the rendering commands
	ciudad();
	//monito();
// endList
	glEndList();

	return(ciudadDL);
}

void inicializar(void)     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Negro de fondo	

	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH);

	//glLightfv(GL_LIGHT1, GL_POSITION, Position);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearDepth(1.0f);			// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);	// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);		// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);



	//skybox con sólo una imagen de cielo para las 4 caras
	textSkyboxUnica.LoadBMP("01.bmp");
	textSkyboxUnica.BuildGLTexture();
	textSkyboxUnica.ReleaseImage();

	//con imágenes hechas para skybox para cada lado y arriba

	textSkyboxFrontal.LoadTGA("ely_hills/hills_ft.tga");
	textSkyboxFrontal.BuildGLTexture();
	textSkyboxFrontal.ReleaseImage();

	textSkyboxAtras.LoadTGA("ely_hills/hills_bk.tga");
	textSkyboxAtras.BuildGLTexture();
	textSkyboxAtras.ReleaseImage();

	textSkyboxIzquierda.LoadTGA("ely_hills/hills_lf.tga");
	textSkyboxIzquierda.BuildGLTexture();
	textSkyboxIzquierda.ReleaseImage();

	textSkyboxDerecha.LoadTGA("ely_hills/hills_rt.tga");
	textSkyboxDerecha.BuildGLTexture();
	textSkyboxDerecha.ReleaseImage();

	textSkyboxArriba.LoadTGA("ely_hills/hills_up.tga");
	textSkyboxArriba.BuildGLTexture();
	textSkyboxArriba.ReleaseImage();

	


	textPiso.LoadTGA("texturas/piso.tga");
	textPiso.BuildGLTexture();
	textPiso.ReleaseImage();

	textCaminoCarritos.LoadTGA("caminoCarritos512x512.tga");
	textCaminoCarritos.BuildGLTexture();
	textCaminoCarritos.ReleaseImage();

	arbolt.LoadTGA("texturas/arbolt.tga");
	arbolt.BuildGLTexture();
	arbolt.ReleaseImage();

	pisoverde.LoadTGA("texturas/pisolego.tga");
	pisoverde.BuildGLTexture();
	pisoverde.ReleaseImage();

	tronco.LoadTGA("texturas/tronco.tga");
	tronco.BuildGLTexture();
	tronco.ReleaseImage();

	//******************************************************
	//******************************************************
	//carga de modelos externos .3ds
	

	Carro._3dsLoad("modelos/carro.3DS");
	Carro.LoadTextureImages();
	Carro.GLIniTextures();

	arbol._3dsLoad("modelos/arbol.3DS");
	arbol.LoadTextureImages();
	arbol.GLIniTextures();

	arbol2._3dsLoad("modelos/arbol2.3DS");
	arbol2.LoadTextureImages();
	arbol2.GLIniTextures();  

	
	fuente._3dsLoad("modelos/fuente/fuente.3DS");
	fuente.LoadTextureImages();
	fuente.GLIniTextures();
	 
	lampara._3dsLoad("modelos/lampara.3DS");
	lampara.LoadTextureImages();
	lampara.GLIniTextures();

	carro1._3dsLoad("modelos/carro1.3DS");
	carro1.LoadTextureImages();
	carro1.GLIniTextures();

	textpuerta.LoadTGA("puerta.tga");
	textpuerta.BuildGLTexture();
	textpuerta.ReleaseImage();

	textventana.LoadTGA("ventana.tga");
	textventana.BuildGLTexture();
	textventana.ReleaseImage();

	textcasamadera.LoadTGA("paredb.tga");
	textcasamadera.BuildGLTexture();
	textcasamadera.ReleaseImage();

	texttejado.LoadTGA("tejado1.tga");
	texttejado.BuildGLTexture();
	texttejado.ReleaseImage();

	textpiso1.LoadTGA("pisol.tga");
	textpiso1.BuildGLTexture();
	textpiso1.ReleaseImage();


	textparedint.LoadTGA("paredintl.tga");
	textparedint.BuildGLTexture();
	textparedint.ReleaseImage();

	textescalon.LoadTGA("escalon.tga");
	textescalon.BuildGLTexture();
	textescalon.ReleaseImage();

	textbalon.LoadTGA("balon.tga");
	textbalon.BuildGLTexture();
	textbalon.ReleaseImage();
	


	textPez.LoadTGA("cosmo.tga");
	textPez.BuildGLTexture();
	textPez.ReleaseImage();

	textManija.LoadTGA("jala.tga");
	textManija.BuildGLTexture();
	textManija.ReleaseImage();

	textCajon.LoadTGA("mueblecl.tga");
	textCajon.BuildGLTexture();
	textCajon.ReleaseImage();

	textmabaul.LoadTGA("maderacama.tga");
	textmabaul.BuildGLTexture();
	textmabaul.ReleaseImage();

	textbaul.LoadTGA("baul.tga");
	textbaul.BuildGLTexture();
	textbaul.ReleaseImage();


	textcobija.LoadTGA("colchon.tga");
	textcobija.BuildGLTexture();
	textcobija.ReleaseImage();

	textalmu.LoadTGA("almu.tga");
	textalmu.BuildGLTexture();
	textalmu.ReleaseImage();


	textbase.LoadTGA("basetele.tga");
	textbase.BuildGLTexture();
	textbase.ReleaseImage();

	texttele.LoadTGA("pantallal.tga");
	texttele.BuildGLTexture();
	texttele.ReleaseImage();

	textpantalla.LoadTGA("botones.tga");
	textpantalla.BuildGLTexture();
	textpantalla.ReleaseImage();

	textburo.LoadTGA("mueblel.tga");
	textburo.BuildGLTexture();
	textburo.ReleaseImage();


	textlibros.LoadTGA("libros1.tga");
	textlibros.BuildGLTexture();
	textlibros.ReleaseImage();

	textportada.LoadTGA("portada.tga");
	textportada.BuildGLTexture();
	textportada.ReleaseImage();



	/// creacion de la funcio para reproducir audio
	//valida si pudo crear el motor de audio
	if (!engine)
	{
		printf("\n No pude crear el motor de audio :(");
		exit(0); //termina ejecución justo aquí
	}

	//reproduce con el 20% del sonido
	audioFondo->setDefaultVolume(0.2f);

	//reproduce el sonido sin detenerse
	engine->play2D(audioFondo, true);


	objCamera.Position_Camera(0, 2.5f, 3, 0, 2.5f, 0, 0, 1, 0);



	//NEW Crear una lista de dibujo
	ciudad_display_list = createDL();


}

void pintaTexto(float x, float y, float z, void *font, char *string)
{
	char *c;     //Almacena los caracteres a escribir
	glRasterPos3f(x, y, z);	//Posicion apartir del centro de la ventana
	//glWindowPos2i(150,100);
	for (c = string; *c != '\0'; c++) //Condicion de fin de cadena
	{
		glutBitmapCharacter(font, *c); //imprime
	}
}

void arbolcono(void) {
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0, -59, 0);
	cilindro.cilindro(.5, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -58.5, 0);
	cilindro.cilindro(.7, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -58, 0);
	cilindro.cilindro(.9, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -57.5, 0);
	cilindro.cilindro(1.1, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -57, 0);
	cilindro.cilindro(1.3, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -56.5, 0);
	cilindro.cilindro(1.5, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -56, 0);
	cilindro.cilindro(1.7, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -55.5, 0);
	cilindro.cilindro(1.9, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -55, 0);
	cilindro.cilindro(2.1, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -54.5, 0);
	cilindro.cilindro(1.9, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -54, 0);
	cilindro.cilindro(1.7, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -53.5, 0);
	cilindro.cilindro(1.5, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -53, 0);
	cilindro.cilindro(1.3, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -52.5, 0);
	cilindro.cilindro(1.1, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -52, 0);
	cilindro.cilindro(0.9, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -51.5, 0);
	cilindro.cilindro(0.7, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -51, 0);
	cilindro.cilindro(0.5, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -50.5, 0);
	cilindro.cilindro(0.3, .08, 60, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -50, 0);
	cilindro.cilindro(0.1, .08, 60, arbolt.GLindex);
	glPopMatrix();
	glPopMatrix();

}
//funcion para crear objetos 3d
void objetos3d(void) {
	////	objetos 3d ////////
	
	//Carro rojo
	glPushMatrix();
	glTranslatef(-1.5, -59.9, 55);
	glScalef(.020, .020, .020);
	Carro.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-48.5, -59.9, 10);
	glScalef(.020, .020, .020);
	Carro.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.5, -59.9, -55);
	glScalef(.020, .020, .020);
	Carro.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(23.5, -59.9, -30);
	glScalef(.020, .020, .020);
	Carro.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50.5, -59.9, 20);
	glScalef(.020, .020, .020);
	Carro.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	
	glPushMatrix();
	
	glRotatef(90, 0, 3, 0);
	glTranslatef(0, 0, movKitZ);
	glTranslatef(-10.5, -59.9, 50);
	
	glScalef(.020, .020, .020);
	Carro.GLrender(NULL, _SHADED, 1.0);

	if (g_fanimacion)
	{
		if (g_avanza)
		{
			movKitZ += 0.8;
			//rotTires -= 10;
			if (movKitZ > 10)
				g_avanza = false;
		}
		else
		{
			movKitZ -= 0.8;
			//rotTires += 10;
			if (movKitZ < -80)
				g_avanza = true;
		}
	}


	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-50.5, -59.9, -45);
	glScalef(.020, .020, .020);
	Carro.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	//arbol cono
	glPushMatrix();
	glTranslatef(29.0, -58.0, -60.0);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(29.0, -58.0, -35.0);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(29.0, -58.0, -20.0);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(29.0, -58.0, 5.0);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(29.0, -58.0, 25.0);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(29.0, -58.0, 40.0);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(29.0, -58.0, 60.0);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, -58.0, 2);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17, -58.0, 2);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-18, -58.0, -12);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-8, -58.0, -6);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, -58.0, 4);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, -58.0, -9);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, -58.0, -13);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9, -58.0, -27);
	glScalef(.7, .7, .7);
	arbol.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
	//
	//arbol copa redonda
	glPushMatrix();
	glTranslatef(-54.5, -60, -60);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-54.5, -60, -35);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-54.5, -60, -15);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-54.5, -60, 5);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-54.5, -60, 20);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-54.5, -60, 45);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-54.5, -60, 60);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
	//
	glPushMatrix();
	glTranslatef(-20.5, -60, -60);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.5, -60, -35);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.5, -60, -15);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.5, -60, 5);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.5, -60, 20);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.5, -60, 45);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.5, -60, 60);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
	//
	glPushMatrix();
	glTranslatef(20.5, -60, -60);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.5, -60, -35);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.5, -60, -15);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.5, -60, 5);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.5, -60, 20);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.5, -60, 45);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.5, -60, 60);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(5, -60, -10);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(15, -60, -20);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, -60, -19);
	glScalef(.03, .03, .03);
	arbol2.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	////

	glPushMatrix();
	glTranslatef(0, -59.9, -10);
	glScalef(0.06, 0.06, 0.06);
	fuente.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	//Poste Lampara
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-46.5, -60.1, 6);
	glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-46.5, -60.1, 6);
	glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(57.4, -60.1, -26.7);
	glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(57.4, -60.1, 26.7);
	glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-55.4, -60.1, -53.7);
	//glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-55.4, -60.1, -26.7);
	//glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-55.4, -60.1, 26.7);
	//glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(-30.4, -60.1, 46.7);
	//glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(20.4, -60.1, -53.7);
	//glRotatef(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();

	///






	glPopMatrix();
	glPopMatrix();

}

void tele(GLfloat xMadera, GLfloat yMadera, GLfloat zMadera, GLfloat xPosTel, GLfloat yPosTel, GLfloat zPosTel) {
	glPushMatrix();
	glScalef(xMadera, yMadera, zMadera);
	glTranslatef(xPosTel, yPosTel, zPosTel);

	glPushMatrix();
	glScalef(3, 1.5f, 2);
	glDisable(GL_LIGHTING);
	mabaul.prisma(1.0, 1.0, 1.0, textmabaul.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, 1.0f, 0.0);
	glScalef(2.5, 2.5f, 1.0);
	glDisable(GL_LIGHTING);
	base.prisma(1.0, 1.0, 1.0, textbase.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, 1.6f, -0.5);
	glScalef(2.0, 1.0f, 0.1);
	glDisable(GL_LIGHTING);
	televi.prisma(1.0, 1.0, 0.1, texttele.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.6, 1.0f, -0.5);
	glScalef(1.0, 0.1f, 0.1);
	glDisable(GL_LIGHTING);
	televi.prisma(1.0, 1.0, 0.1, textpantalla.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glPopMatrix();
}






void cama(GLfloat xMadera, GLfloat yMadera, GLfloat zMadera, GLfloat xPosCam, GLfloat yPosCam, GLfloat zPosCam) {
	glPushMatrix();
	glScalef(xMadera, yMadera, zMadera);
	glTranslatef(xPosCam, yPosCam, zPosCam);
	//cabecera
	glPushMatrix();
	glScalef(4, 2.5f, .3);
	glDisable(GL_LIGHTING);
	mabaul.prisma(1.0, 1.0, 1.0, textmabaul.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0, 0.0f, 0.0);
	glScalef(0.5, 3.0f, .3);
	glDisable(GL_LIGHTING);
	mabaul.prisma(1.0, 1.0, 1.0, textmabaul.GLindex);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(0.0, 0.6f, 0.0);
	glScalef(0.5, 0.2f, 0.5);
	glDisable(GL_LIGHTING);
	manija.esfera(0.5, 20.0, 20.0, textManija.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();



	glPopMatrix();


	//patabasepies
	glPushMatrix();
	glTranslatef(-2.0, 0.0f, 0.0);
	glScalef(0.5, 3.0f, .3);
	mabaul.prisma(1.0, 1.0, 1.0, textmabaul.GLindex);
	//esferapata
	glPushMatrix();
	glTranslatef(0.0, 0.6f, 0.0);
	glScalef(0.5, 0.2f, 0.5);
	manija.esfera(0.5, 20.0, 20.0, textManija.GLindex);
	glPopMatrix();
	glPopMatrix();



	//patabasepies
	glPushMatrix();
	glTranslatef(-2.0, 0.0f, 5.0);
	glScalef(0.5, 3.0f, .3);
	mabaul.prisma(1.0, 1.0, 1.0, textmabaul.GLindex);
	//esferapata
	glPushMatrix();
	glTranslatef(0.0, 0.6f, 0.0);
	glScalef(0.5, 0.2f, 0.5);
	manija.esfera(0.5, 20.0, 20.0, textManija.GLindex);
	glPopMatrix();
	glPopMatrix();

	//patabasepies
	glPushMatrix();
	glTranslatef(2.0, 0.0f, 5.0);
	glScalef(0.5, 3.0f, .3);
	mabaul.prisma(1.0, 1.0, 1.0, textmabaul.GLindex);
	//esferapata
	glPushMatrix();
	glTranslatef(0.0, 0.6f, 0.0);
	glScalef(0.5, 0.2f, 0.5);
	manija.esfera(0.5, 20.0, 20.0, textManija.GLindex);
	glPopMatrix();
	glPopMatrix();


	//basepies
	glPushMatrix();
	glTranslatef(0.0, 0.5f, 5.0);
	glScalef(3.5, 4.5f, .3);
	mabaul.prisma(1.0, 1.0, 1.0, textmabaul.GLindex);
	glPopMatrix();

	//base cama

	glPushMatrix();
	glTranslatef(0.0, -1.0f, 2.6);
	glScalef(4, 0.2f, 4.6);
	glDisable(GL_LIGHTING);
	libre.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	//colchon
	glPushMatrix();
	glTranslatef(0.0, -0.4f, 2.6);
	glScalef(4, 1.0f, 4.6);
	glDisable(GL_LIGHTING);
	cobija.prisma(1.0, 1.0, 1.0, textcobija.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.2f, 4.2);
	glScalef(1.5, 0.2f, 1.0);
	glDisable(GL_LIGHTING);
	almu.prisma(1.0, 1.0, 1.0, textalmu.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glPopMatrix();

}

void casa(GLfloat xMadera, GLfloat yMadera, GLfloat zMadera, GLfloat xPosCas, GLfloat yPosCas, GLfloat zPosCas) {

	glPushMatrix();
	//glScalef(xMadera, yMadera, zMadera);
	//glTranslatef(xPosCas, yPosCas, zPosCas);
	//pared derecha a la puerta
	glPushMatrix();
	glTranslatef(-20.5f, 6.5, 5);
	glScalef(9.0, 15.0f, 0.3);
	maderacasa.prisma(1.0, 1.0, 1.0, textcasamadera.GLindex);
	glPopMatrix();

	
	//puerta
	glPushMatrix();
	glTranslatef(-15, 1.0, 5);
	glTranslatef(1, 0, 0);
	glRotatef(open, 0, 1.0, 0);
	glTranslatef(-1, 0, 0);
	glScalef(2.0, 3.5f, 0.4);
	glDisable(GL_LIGHTING);
	puerta.prisma(1.0, 1.0, 1.0, textpuerta.GLindex);
	glEnable(GL_LIGHTING);
	if (p_animacion)
	{
		open = open + 1.0;
		if (open > 90)
		{
			p_animacion = false;
		}

	}
	
	glPopMatrix();

	
	//escalon
	glPushMatrix();
	glTranslatef(-15, -0.7, 4.5);
	glScalef(2.0, 0.4f, 0.4);
	glDisable(GL_LIGHTING);
	escalon.prisma(1.0, 1.0, 1.0, textescalon.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();




	//pared tracera
	glPushMatrix();
	glTranslatef(-15, 6.5, 15.0);
	glScalef(19.8, 15.0f, 0.4);
	maderacasa.prisma(1.0, 1.0, 1.0, textcasamadera.GLindex);
	glPopMatrix();


	//pared tracera int
	glPushMatrix();
	glTranslatef(-15, 6.5, 14.9);
	glScalef(19.7, 15.0f, 0.4);
	glDisable(GL_LIGHTING);
	paredint.prisma(1.0, 1.0, 1.0, textparedint.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	//pared derecha de la puerta
	glPushMatrix();
	glTranslatef(-9.5, 6.5, 5);
	glScalef(9.0, 15.0f, 0.3);
	maderacasa.prisma(1.0, 1.0, 1.0, textcasamadera.GLindex);
	glPopMatrix();




	//pared arriba puerta
	glPushMatrix();
	glTranslatef(-15.0, 8.4, 5);
	glScalef(2.0, 11.2f, 0.3);
	maderacasa.prisma(1.0, 1.0, 1.0, textcasamadera.GLindex);
	glPopMatrix();

	//ventana
	glPushMatrix();
	glTranslatef(-9.5, 2.0, 5.0);
	glScalef(2.0, 2.0f, 0.4);
	glDisable(GL_LIGHTING);
	ventanas.prisma(1.0, 1.0, 1.0, textventana.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	//ventana superior
	glPushMatrix();
	glTranslatef(-9.5, 8.0, 5.0);
	glScalef(2.0, 2.0f, 0.4);
	glDisable(GL_LIGHTING);
	ventanas.prisma(1.0, 1.0, 1.0, textventana.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();



	//ventana
	glPushMatrix();
	glTranslatef(-19.5, 2.0, 5.0);
	glScalef(2.0, 2.0f, 0.4);
	glDisable(GL_LIGHTING);
	ventanas.prisma(1.0, 1.0, 1.0, textventana.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	//ventana sup
	glPushMatrix();
	glTranslatef(-19.5, 8.0, 5.0);
	glScalef(2.0, 2.0f, 0.4);
	glDisable(GL_LIGHTING);
	ventanas.prisma(1.0, 1.0, 1.0, textventana.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();



	/*tejado*/
	glPushMatrix();
	glTranslatef(-5, 16.0, 10);
	glRotatef(90, 0, 0, 1);
	glScalef(4.0, 6.65f, 7.0);
	tejado.cilindro(1.0, 3.0, 3.0, texttejado.GLindex);
	glPopMatrix();


	//pared costado derecho
	glPushMatrix();
	glTranslatef(-24.9, 6.5f, 10.1);
	glScalef(0.2, 15.0f, 10.0);
	glDisable(GL_LIGHTING);
	maderacasa.prisma(1.0, 1.0, 1.0, textcasamadera.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	//pared costado derecho interior
	glPushMatrix();
	glTranslatef(-24.7, 6.5f, 10.1);
	glScalef(0.2, 14.9f, 10.0);
	glDisable(GL_LIGHTING);
	paredint.prisma(1.0, 1.0, 1.0, textparedint.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();




	//pared costado izquierdo
	glPushMatrix();
	glTranslatef(-5.2, 6.5f, 10.1);
	glScalef(0.2, 15.0f, 10.0);
	glDisable(GL_LIGHTING);
	maderacasa.prisma(1.0, 1.0, 1.0, textcasamadera.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	//pared costado izquierdo interior
	glPushMatrix();
	glTranslatef(-5.4, 6.5f, 10.1);
	glScalef(0.2, 14.9f, 10.0);
	glDisable(GL_LIGHTING);
	paredint.prisma(1.0, 1.0, 1.0, textparedint.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	//piso
	glPushMatrix();
	glTranslatef(-15.0, -0.9f, 10.1);
	glScalef(19.4, .2f, 10);
	glDisable(GL_LIGHTING);
	piso1.prisma(1.0, 1.0, 1.0, textpiso1.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	//techolego
	glPushMatrix();
	glTranslatef(-15.0, 13.8f, 10.1);
	glScalef(19.4, .2f, 10);
	glDisable(GL_LIGHTING);
	techo.prisma(1.0, 1.0, 1.0, texttecho.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	

	glPushMatrix();



	glPopMatrix();


	glPopMatrix();//pop final






}






//Funcion para camino de carros
void caminodecarros(void) {
	//BLOQUE PARA CAMINO DE CARROS (VERTICALES)//
	//BLOQUE 1//
	glPushMatrix();
	glTranslatef(-59, -59.9, -50.0);
	glScalef(12, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-37.5, -59.9, -50.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-12.5, -59.9, -50.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(12.5, -59.9, -50.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(38.5, -59.9, -50.0);
	glScalef(21, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(60, -59.9, -50.0);
	glScalef(10, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 1//

	//BLOQUE 2//
	glPushMatrix();
	glTranslatef(-59, -59.9, -30.0);
	glScalef(12, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-37.5, -59.9, -30.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-12.5, -59.9, -30.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(12.5, -59.9, -30.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(38.5, -59.9, -30.0);
	glScalef(21, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(60, -59.9, -30.0);
	glScalef(10, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 2//

	//BLOQUE 3//
	glPushMatrix();
	glTranslatef(-59, -59.9, -10.0);
	glScalef(12, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-37.5, -59.9, -10.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-12.5, -59.9, -10.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(12.5, -59.9, -10.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(38.5, -59.9, -10.0);
	glScalef(21, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(60, -59.9, -10.0);
	glScalef(10, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 3//

	//BLOQUE 4//
	glPushMatrix();
	glTranslatef(-59, -59.9, 10.0);
	glScalef(12, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-37.5, -59.9, 10.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-12.5, -59.9, 10.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(12.5, -59.9, 10.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(38.5, -59.9, 10.0);
	glScalef(21, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(60, -59.9, 10.0);
	glScalef(10, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 4//

	//BLOQUE 5//
	glPushMatrix();
	glTranslatef(-59, -59.9, 30.0);
	glScalef(12, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-37.5, -59.9, 30.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-12.5, -59.9, 30.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(12.5, -59.9, 30.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(38.5, -59.9, 30.0);
	glScalef(21, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(60, -59.9, 30.0);
	glScalef(10, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 5//

	//BLOQUE 6//
	glPushMatrix();
	glTranslatef(-59, -59.9, 50.0);
	glScalef(12, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-37.5, -59.9, 50.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-12.5, -59.9, 50.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(12.5, -59.9, 50.0);
	glScalef(19, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(38.5, -59.9, 50.0);
	glScalef(21, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(60, -59.9, 50.0);
	glScalef(10, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 6//
	//FIN BLOQUE PARA CAMINO DE CARRITOS (HORIZONTALES)//

	//BLOQUE PARA CAMINO DE CARRITOS (VERTICALES) //

	//BLOQUE 1//
	glPushMatrix();
	glTranslatef(-50, -60, -55);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();


	glPushMatrix();
	glTranslatef(-50, -60, -32);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-50, -60, -9);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-50, -60, 14);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-50, -60, 37);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-50, -60, 56);
	glRotatef(90, 0, 1, 0);
	glScalef(18, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 1//

	//BLOQUE 2//
	glPushMatrix();
	glTranslatef(-25, -60, -55);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-25, -60, -32);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-25, -60, -9);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-25, -60, 14);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-25, -60, 37);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-25, -60, 56);
	glRotatef(90, 0, 1, 0);
	glScalef(18, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 2//

	//BLOQUE 3//
	glPushMatrix();
	glTranslatef(0, -60, -55);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(0, -60, -32);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(0, -60, -9);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(0, -60, 14);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(0, -60, 37);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(0, -60, 56);
	glRotatef(90, 0, 1, 0);
	glScalef(18, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 3//

	//BLOQUE 4//
	glPushMatrix();
	glTranslatef(25, -60, -55);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(25, -60, -32);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(25, -60, -9);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(25, -60, 14);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(25, -60, 37);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(25, -60, 56);
	glRotatef(90, 0, 1, 0);
	glScalef(18, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 4//

	//BLOQUE 5
	glPushMatrix();
	glTranslatef(52, -60, -55);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(52, -60, -32);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(52, -60, -9);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(52, -60, 14);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(52, -60, 37);
	glRotatef(90, 0, 1, 0);
	glScalef(23, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();
	glTranslatef(52, -60, 56);
	glRotatef(90, 0, 1, 0);
	glScalef(18, 0.1, 6);
	glDisable(GL_LIGHTING);
	figCaminoCarritos.prisma2CaminosH(textCaminoCarritos.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	//FIN BLOQUE 5//
	//FIN CAMINO DE CARRITOS (HORIZONTALES)//
}

//Funcion para pasto de lego
void pastolego(void) {
	//pisolego
	glPushMatrix();
	glTranslatef(-37.6, -59.98, 6);
	//glRotatef(90, 0, 1, 0);
	glScalef(19.5, 0.04, 2);
	glDisable(GL_LIGHTING);
	figpisoverde.prisma2CaminosH(pisoverde.GLindex, 6);
	glEnable(GL_LIGHTING);
	glPopMatrix();


}


void buros(GLfloat xMadera, GLfloat yMadera, GLfloat zMadera, GLfloat xPosBuro, GLfloat yPosBuro, GLfloat zPosBuro) {
	glPushMatrix();
	glScalef(xMadera, yMadera, zMadera);
	glTranslatef(xPosBuro, yPosBuro, zPosBuro);
	//base buro		 
	glPushMatrix();
	glScalef(3, .2f, 3);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	//pared
	glPushMatrix();
	glTranslatef(0.0, -0.8f, 1.2);
	glScalef(3, 1.5f, 0.2);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	//frente
	glPushMatrix();
	glTranslatef(0.0, -0.8f, -1.2);
	glScalef(3, 1.5f, 0.2);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);

	//cajon
	glPushMatrix();
	glTranslatef(0.0, 0.0f, -1.0);
	glScalef(0.5, 0.5f, 0.5);
	cajon.prisma(1.0, 1.0, 1.0, textCajon.GLindex);

	//jaladera
	glPushMatrix();
	glTranslatef(0.0, 0.0f, -1.0);
	glScalef(1.0, 1.0f, 1.0);
	manija.esfera(0.1, 10.0, 10.0, textManija.GLindex);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//base abajo
	glPushMatrix();
	glTranslatef(0.0, -1.4f, 0.0);
	glScalef(2.8, 0.2f, 2.5);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	//patas
	glPushMatrix();
	glTranslatef(1.0, -2.0f, -1.0);
	glScalef(0.3, 1.0f, 0.3);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, -2.0f, -1.0);
	glScalef(0.3, 1.0f, 0.3);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, -2.0f, 1.0);
	glScalef(0.3, 1.0f, 0.3);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, -2.0f, 1.0);
	glScalef(0.3, 1.0f, 0.3);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	//costados
	glPushMatrix();
	glTranslatef(1.38, -0.8f, 0.0);
	glScalef(0.2, 1.5f, 2.5);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.38, -0.8f, 0.0);
	glScalef(0.2, 1.5f, 2.5);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glPopMatrix();

	//pecera
	glPushMatrix();
	glTranslatef(0.0, 1.0f, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	pecera.esfera(1.0, 10.0, 10.0, textPez.GLindex);
	glPopMatrix();




	glPopMatrix();




}



void librero(GLfloat xMadera, GLfloat yMadera, GLfloat zMadera, GLfloat xPosLib, GLfloat yPosLib, GLfloat zPosLib) {
	glPushMatrix();

	glScalef(xMadera, yMadera, zMadera);
	glTranslatef(xPosLib, yPosLib, zPosLib);

	//base superior
	glPushMatrix();
	glScalef(8, .2f, 3);
	glDisable(GL_LIGHTING);
	libre.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	//pared
	glPushMatrix();
	glTranslatef(0.0, -2.0f, 1.4);
	glScalef(8, 4.0f, 0.2);
	glDisable(GL_LIGHTING);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	//base inferior
	glPushMatrix();
	glTranslatef(0.0, -4.1f, 0.0);
	glScalef(8, .2f, 3);
	glDisable(GL_LIGHTING);
	libre.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	//repisa
	glPushMatrix();
	glTranslatef(0.0, -2.1f, 0.0);
	glScalef(8, .2f, 2.5);
	glDisable(GL_LIGHTING);
	libre.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	//costados
	glPushMatrix();
	glTranslatef(4.1, -2.0f, 0.0);
	glScalef(0.2, 4.3f, 3.0);
	glDisable(GL_LIGHTING);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.1, -2.0f, 0.0);
	glScalef(0.2, 4.3f, 3.0);
	glDisable(GL_LIGHTING);
	maderaBuro.prisma(1.0, 1.0, 1.0, textburo.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	//libros
	glPushMatrix();
	glTranslatef(-1.0, -1.2f, -1.0);
	glScalef(6.0, 1.9f, 0.1);
	glDisable(GL_LIGHTING);
	libros.prisma(1.0, 1.0, 1.0, textlibros.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -3.0f, -1.0);
	glScalef(7.8, 1.9f, 0.1);
	glDisable(GL_LIGHTING);
	libros.prisma(1.0, 1.0, 1.0, textlibros.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	//portada
	glPushMatrix();
	glTranslatef(2.0, -1.0f, 0.0);
	glScalef(0.1, 1.85f, 2.0);
	glDisable(GL_LIGHTING);
	portada.prisma(1.0, 1.0, 1.0, textportada.GLindex);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glPopMatrix();
}






//Funcion crear arbol1
void arbolito(void) {
	glPushMatrix();

	glPushMatrix();
	glTranslatef(0, 0,0);
	arbol1.prisma(2,.2, .2, tronco.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 0);
	arbol1.prisma(.2, 1.5, 1.5, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.2, 0);
	arbol1.prisma(.2, 1, 1, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.4, 0);
	arbol1.prisma(.2, .7, .7, arbolt.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.6, 0);
	arbol1.prisma(.2, .2, .2, arbolt.GLindex);
	glPopMatrix();

	glPopMatrix();
}

void dibujar(void)   // Creamos la funcion donde se dibuja
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Limpiamos pantalla y Depth Buffer

	glLoadIdentity();


	glPushMatrix();

		glRotatef(g_lookupdown, 1.0f, 0, 0);

		gluLookAt(objCamera.mPos.x, objCamera.mPos.y, objCamera.mPos.z,
			objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
			objCamera.mUp.x, objCamera.mUp.y, objCamera.mUp.z);


		glPushMatrix();
			glPushMatrix(); //Creamos cielo
				glDisable(GL_LIGHTING);
				//glTranslatef(0,-60,0);
				//figSkybox.skybox(130.0, 130.0, 130.0,textSkyboxUnica.GLindex);
				figSkybox.skyboxH(130.0, 130.0, 130.0, textSkyboxFrontal.GLindex, textSkyboxAtras.GLindex, textSkyboxIzquierda.GLindex, textSkyboxDerecha.GLindex, textSkyboxArriba.GLindex);
				glEnable(GL_LIGHTING);
			glPopMatrix();
			
			
			//
			glPushMatrix();	
			glTranslated(-2, -59, 49);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			//balon
			//glPushMatrix();
			glTranslatef(0, bota, 0);
			/*glTranslatef(-5, 0.5, 15.0);*/
			
			glTranslated(-7, -0.45, -31);
			glScalef(0.4, 0.4, 0.4);
			balon.esfera(1, 20, 20, textbalon.GLindex);
			glPopMatrix();
			if (bota > 1)
			{
				p_animacion1 = false;
				p_animacion2 = true;
			}
			if (bota < 0)
			{
				p_animacion2 = false;
				p_animacion1 = true;
			}



			if (p_animacion1)
			{
				bota += 0.3;
			}

			if (p_animacion2)
			{
				bota -= 0.03;
			}
			//glPopMatrix();

			glPushMatrix();
			glTranslated(-2, -59, 30);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);

			

			glTranslated(-23, -0.5, -10);
			cama(1, 0.7, 0.8, 1, 1, 1);
			
			
			
			
			glTranslatef(1, -0.5, 0);
			glRotatef(180, 0, 1, 0);
			tele(0.8, 0.8, 0.8, 1, 1, 1);
			
			glTranslatef(-3.5, 1, -4.0);
			glRotatef(180, 0, 1, 0);
			glScalef(0.7, 0.7, 0.7);
			buros(1, 1, 1, 1, 1, 1);

			glTranslatef(15.5, 1, -4.0);
			glRotatef(90, 0, 1, 0);
			glScalef(0.7,0.7,0.7);
			librero(1, 1, 1, 1, 1, 1);
			
			glPopMatrix();

			glPushMatrix();
			glTranslated(-2, -59, 11);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();
			

			glPushMatrix();
			glTranslated(-2, -59, -50);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();
			
			
			
			//
			
			//
			glPushMatrix();
			glTranslated(-27, -59, 49);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();


			glPushMatrix();
			glTranslated(-27, -59, 30);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();


			glPushMatrix();
			glTranslated(-27, -59, 11);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();


			glPushMatrix();
			glTranslated(-27, -59, -9);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();



			glPushMatrix();
			glTranslated(-27, -59, -30);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslated(-27, -59, -50);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();
			//
			
			
			
			//
			glPushMatrix();
			glTranslated(23, -59, 49);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();


			glPushMatrix();
			glTranslated(23, -59, 30);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslated(23, -59, 11);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();


			glPushMatrix();
			glTranslated(23, -59, -50);
			glScalef(.8, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();
			//
			
			
			
			
			//
			glPushMatrix();
			glTranslated(49, -59, 49);
			glScalef(.7, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslated(51, -59, 30);
			glScalef(.8, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslated(51, -59, 11);
			glScalef(.8, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslated(51, -59, -9);
			glScalef(.8, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslated(51, -59, -30);
			glScalef(.8, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslated(51, -59, -50);
			glScalef(.8, 1, 1);
			casa(1, 1, 1, 0, 0, 0);
			glPopMatrix();
			//





			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
				glColor3f(1, 1, 1);
				//glScalef(0.5, 0.5, 0.5);
				//monito();
				glCallList(ciudad_display_list);
				//glTranslatef(posX, posY, posZ);
				//glRotatef(giroMonito, 0, 1, 0);
				//monito();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();

			//Parque
			glPushMatrix();
			glTranslatef(0, -59.8, -10);
			glScalef(45, 0.1, 36);
			figpisoverde.prisma2CaminosH(pisoverde.GLindex, 6);
			glPopMatrix();
			//Fin parque
			
			//Llamada a camino para carros//
			glPushMatrix();
			caminodecarros();
			glPopMatrix();
			/////////////////////////////////

			{
				////Inicio bloques lego pasto horizontales//////////
				glPushMatrix();
				glTranslatef(-60, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-60, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-60, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-60, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-60, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-60, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();
				glPushMatrix();

				glTranslatef(-60, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();



				//segundo bloque////
				glPushMatrix();
				glTranslatef(-52, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-52, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-52, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-52, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-52, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-52, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-52, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				///////////////////

				//tercerbloque////
				glPushMatrix();
				glTranslatef(-35, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-35, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-35, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-35, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-35, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-35, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-35, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				///////////////////


			//cuarto bloque bloque////
				glPushMatrix();
				glTranslatef(-27, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-27, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-27, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-27, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-27, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-27, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-27, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				///////////////////

			//quinto bloque////
				glPushMatrix();
				glTranslatef(-10, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-10, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-10, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-10, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-10, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-10, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-10, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				///////////////////
				//sexto bloque////
				glPushMatrix();
				glTranslatef(-2, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-2, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-2, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-2, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-2, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-2, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-2, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				///////////////////

				//septimo bloque////
				glPushMatrix();
				glTranslatef(15, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(15, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(15, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(15, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(15, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(15, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(15, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();
				///////////////

				//octavo bloque////
				glPushMatrix();
				glTranslatef(23, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(23, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(23, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(23, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(23, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(23, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(23, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				/////////////////

				//noveno bloque////
				glPushMatrix();
				glTranslatef(42, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(42, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(42, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(42, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(42, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(42, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(42, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				/////////////////

				//decimo bloque////
				glPushMatrix();
				glTranslatef(50, 0, 18);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(50, 0, -1);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(50, 0, -20);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(50, 0, -39);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(50, 0, -58);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(50, 0, -77);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				glPushMatrix();
				glTranslatef(50, 0, -93);
				glRotatef(90, 0, 1, 0);
				pastolego();
				glPopMatrix();

				/////////////////

				//////Fin bloques lego pasto horizontales/////////////

			}
			///Creacion de Arboles/////
			// llamada a arbol de cono
			glPushMatrix;
			glPushMatrix();
			glScalef(.5, .5, .5);
			//trasladar en x la misma distancia que arbol cono asi como escalar
			glTranslatef(-58, -120, 11);
			cilindro.cilindro(.1, 10, 60, arbolt.GLindex);
			glPopMatrix();
			glScalef(.5, .5, .5);
			glTranslatef(-58, -60, 11);
			arbolcono();
			glPopMatrix();
		//Fin Creacion de Arboles/////////		

		//Inicio creacion arboles cuadrados///
			glPushMatrix();
			glTranslatef(-29.5, -59.2, -55);
			arbolito();
			glPopMatrix();


			glPushMatrix();
			glTranslatef(-29.5, -59.2, -40);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-29.5, -59.2, -20);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-29.5, -59.2, 0);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-29.5, -59.2, 20);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-29.5, -59.2, 40);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-29.5, -59.2, 60);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(3.5, -59.2, -55);
			arbolito();
			glPopMatrix();


			glPushMatrix();
			glTranslatef(3.5, -59.2, -40);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(3.5, -59.2, -20);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(3.5, -59.2, 0);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(3.5, -59.2, 20);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(3.5, -59.2, 40);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(3.5, -59.2, 60);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(56.5, -59.2, -55);
			arbolito();
			glPopMatrix();


			glPushMatrix();
			glTranslatef(56.5, -59.2, -40);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(56.5, -59.2, -20);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(56.5, -59.2, 0);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(56.5, -59.2, 20);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(56.5, -59.2, 40);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(56.5, -59.2, 60);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -59.2, 0);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(7, -59.2, -15);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-8, -59.2, -17);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-20, -59.2, -1);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-15, -59.2, -25);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(16, -59.2, 5);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(7, -59.2, 4);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-7, -59.2, 2);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-2, -59.2, -12);
			arbolito();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-9, -59.2, -11);
			arbolito();
			glPopMatrix();

		//Fin creacion arboles cuadrdos//

			//llamda a objetos 3d//
			glPushMatrix();
			objetos3d();
			glPopMatrix();
			///////////////////////


		/*	glPushMatrix();
			piso1.prisma(10, 10, 10, texttecho.GLindex);

			glPopMatrix();
*/

			glPopMatrix();
			glPopMatrix();

			
			

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);
	pintaTexto(-11, 12.0, -14.0, (void *)font, "Poyecto Final Computacion Grafica");
	pintaTexto(-11, 8.5, -14, (void *)font, s);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glFlush();
	glutSwapBuffers();

}


void remodelar(int width, int height)
{
	if (height == 0)	// Prevenir división entre cero
	{
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);	// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista
	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);		// Seleccionamos Modelview Matrix
	glLoadIdentity();
}

void teclado(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w': case 'W':
		objCamera.Move_Camera(CAMERASPEED + 0.2);
		break;

	case 's': case 'S':
		objCamera.Move_Camera(-(CAMERASPEED + 0.2));
		break;

	case 'a': case 'A':
		objCamera.Strafe_Camera(-(CAMERASPEED + 0.4));
		break;

	case 'd': case 'D':
		objCamera.Strafe_Camera(CAMERASPEED + 0.4);
		break;
	case 'p':
		engine->play2D(audioPikachu);
		break;


	case 'X':
	case 'x':
		p_animacion = true;
		break;

	case 'C':
	case 'c':
		p_animacion1 = true;
		break;


	case 'O':		//  
	case 'o':
		g_fanimacion ^= true; //Activamos/desactivamos la animacíon
		//circuito = false;
		break;

	case 27:  //Si presiona tecla ESC (ASCII 27) sale
		exit(0);
		break;
	default:  //Si es cualquier otra tecla no hace nada
		break;
	}

	glutPostRedisplay();
}

void teclasFlechasFunc(int a_keys, int x, int y)
{
	switch (a_keys) {
	case GLUT_KEY_PAGE_UP:	//tecla avance de página
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:	//tecla retroceso de página
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

	case GLUT_KEY_UP:	//tecla ARRIBA
		g_lookupdown -= 1.0f;
		break;

	case GLUT_KEY_DOWN:	//tecla ABAJO
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:	//tecla izquierda
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:	//tecla derecha
		objCamera.Rotate_View(CAMERASPEED);
		break;


	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)   // Main Function
{
	//inicializa GLUT con el sistema de ventanas pasando los argumentos del main
	glutInit(&argc, argv);

	//Buffer simple para dibujar
	//Colores RGB y alpha
	//Buffer de profundidad
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//Define ventana de 800 pixeles de ancho por 800 de alto
	glutInitWindowSize(650, 650);

	//Posiciona la ventana de izquierda a derecha 400 pixeles y de arriba a abajo 100
	glutInitWindowPosition(200, 0);

	//Crea y abre la ventana y recibe el nombre que va en su barra de título
	glutCreateWindow("Town Seed");

	//Llamada a función propia para inicializar estados de opengl
	inicializar();

	//Llamada a la función a ser dibujada y redibujada
	glutDisplayFunc(dibujar);

	//Llamada a función para cambio de tamaño de ventana
	glutReshapeFunc(remodelar);

	//Lamada a función que maneja eventos del teclado con teclas normales
	glutKeyboardFunc(teclado);

	//Lamada a función que maneja eventos del teclado con teclas de flechas de dirección y f1, f2 .. fn
	glutSpecialFunc(teclasFlechasFunc);

	//Llamada a función que cede el control a GLUT y procesa eventos de ventana, ratón
	glutMainLoop();

	//termina la ejecución devolviendo cero
	return 0;

}
