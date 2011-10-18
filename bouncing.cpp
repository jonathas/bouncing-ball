#include <gl/glut.h>
#include <math.h>

#define PI 3.1415926535897932

void verificaFronteira();

// Tamanho do plano cartesiano
GLfloat tam_x;
GLfloat tam_y;

GLfloat escala = 1.0f;

// Deslocamento do circulo nos eixos X e Y
GLint dx = 5;
GLint dy = 5;

// Centro atual do circulo.  Inicialmente e a origem.
GLfloat xc = 0;
GLfloat yc = 0;

// Raio do circilo
GLfloat raio = 10.0f;

// Altura e largura atual da janela
GLint largura;
GLint altura;

GLint lados = 60;

//Flags
bool fill = false, stopit = false;

// Desenha um circulo com centro (xc, yc) e raio
// Pode desenhar preenchido ou nao
void circulo(GLfloat xc, GLfloat yc, GLfloat raio) {
	
  // Se o parametro indica para preencher o circulo usa a primitiva GL_TRIANGLE_FAN
  // Se e para desenhar so o contorno entao usa a primitiva GL_LINE_LOOP
  glBegin(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
  
  // Desenha o circulo como uma sequencia de segmentos de reta
  for (int i = 0; i < lados + 1; i++) {  // +1 para fechar
	glVertex2f( xc + raio * cos(2.0 * PI * i / lados),
		yc + raio * sin(2.0 * PI * i / lados) );
  } 
  
  glEnd();
  glLoadIdentity();
}
    
// Função callback chamada para fazer o desenho da tela
void desenha(void) {
	//Limpa a janela de visualização com a cor de fundo especificada 
	glClear(GL_COLOR_BUFFER_BIT);

    // Define a cor vermelha e desenha um circulo
    glColor3f(1.0f, 0.0f, 0.0f);
    circulo(xc, yc, raio);

	//Executa os comandos OpenGL 
	glFlush();
	glLoadIdentity();
}

// Funcao callback chamada quando redimensiona a tela
void redimensiona(GLsizei larg, GLsizei alt) {
     // Salva nas variaveis a largura e altura da janela
     largura = larg;
     altura = alt;
     
     // Redimensiona a viewport para ficar com o mesmo tamanho da janela
     glViewport(0, 0, largura, altura);
     
     // Define o tamanho do plano cartesiano
     // Se a tela tem largura 300, entao o plano no eixo X será de (-150, 150)
     // Se a tela tem altura 250, entao o plano no eixo Y será de (-125, 125)
     tam_x = largura / 2;
     tam_y = altura / 2;    
   
    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Passa para o OpenGL o tamanho do plano cartesiano
    gluOrtho2D(-tam_x, tam_x, -tam_y, tam_y);
       
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Essa funcao e chamada de 100ms em 100ms para mover o circulo
void move(int valor) {
	//Flag para saber se a animação vai ficar parada ou se movimentar
    if(stopit == false) {
		// Desloca o raio do circulo de acordo com os deslocamentos DX e DY
		xc += dx;
		yc += dy;         
		 
		// Verifica se o circulo se chocou com uma das bordas da viewport
		verificaFronteira();
	}
    // Redesenha a tela e pede ao OpenGL que chame essa funcao novamente 
    // dentro de 100 ms
    glutPostRedisplay();
    
	glutTimerFunc(100, move, 0);
}

// Essa funcao verifica se o circulo ultrapassou as fronteiras da viewport
// e ajusta os deslocamentos DX e DY
void verificaFronteira() {
   // Se o circulo bateu no limite à esquerda entao 
   // passa a andar para a direita
   if (xc-raio <= -tam_x) {
     xc = -tam_x + raio;
     dx = -dx;
   }
   else
       // Se o circulo bateu no limite à direita entao 
       // passa a andar para a esquerda
       if (xc+raio >= tam_x) {
         xc = tam_x - raio;
         dx = -dx;
       }

   // Se o circulo bateu no limite superior entao 
   // passa a andar para baixo
   if (yc-raio <= -tam_y) {
     yc = -tam_y + raio;
     dy = -dy;
   }
   else 
       // Se o circulo bateu no limite inferior entao 
       // passa a andar para cima
       if (yc+raio >= tam_y) {
         yc = tam_y - raio;
         dy = -dy;
       }
}

void teclado(unsigned char tecla, int x, int y) {
	switch(tecla) {
		case 's':
		case 'S':
			stopit = ! stopit;
			break;
		case '+':
			lados += 1;
			break;
		case '-':
			lados -= 1;
			break;
		case ' ':
			fill = ! fill;
			break;
		default:;
	}
	
	// Limite inferior e superior de lados
	if(lados <= 4) {
		lados = 4;
	} else if(lados >= 60) {
		lados = 60;
	}
	
	glutPostRedisplay();
}

void tecladoEspecial(int tecla, int x, int y) {
	switch(tecla) {
		case GLUT_KEY_UP:
			raio += 1;
			break;
		case GLUT_KEY_DOWN:
			raio -= 1;
			break;
		default:;
		break;
	}
	
	// Limite de raio
	if (raio >= 50)
		raio = 50;
	else if(raio <= 5)
		raio = 5;

     glutPostRedisplay();
}

void mouse(int botao, int estado, int x, int y) {
    if (botao == GLUT_LEFT_BUTTON)
		if (estado == GLUT_DOWN) {
			xc = ( (2 * tam_x * x) / largura) - tam_x;
			yc = ( ( (2 * tam_y) * (y - altura) ) / - altura) - tam_y;
		}

     glutPostRedisplay();
}

// Inicializa parâmetros de renderizacao
void inicializa (void) {   
    // Define a cor de fundo da janela de visualização (RGB)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(300,400);
    glutInitWindowPosition(10,10);
	glutCreateWindow("Bouncing Ball - Jonathas Rodrigues");
	glutDisplayFunc(desenha);
	glutSpecialFunc(tecladoEspecial);
	glutKeyboardFunc(teclado);
	glutMouseFunc(mouse);
	glutReshapeFunc(redimensiona);
	glutTimerFunc(100, move, 0);
	inicializa();
	glutMainLoop();
	return 0;
}
