#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void EmulaGravidade();
void ImprimeInicializacao();



struct TEXTURE_MAP{
    float Xi;
    float Yi;
    float Xf;
    float Yf;
};


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f,  0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  0.0f);





// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float cameraSpeed = 0.02f; // adjust accordingly

// valores variáveis da camera
float sensitivity = 0.3f;
float yaw = -90.0f;
float pitch = 0.0f;
float zoom = 45.0f;


float speedRateVelocidade = 0.04f;

// valores espaciais
const float limite_chao = -0.01f;
const float limite_ceu = 20.0f;
const float limite_mapa = 15.0f;
const float gravidade = 9.98f; // m/s^2
const float size_peca = 1.0f;


// valores do personagem
float altura_personagem = 1.0f;
float massa = 70.0f; // Quilogramas
float forca_pulo = 200.0f; // Newton
float velocidade_inicial_pulo; // m/s^2
float velocidade_y; // m/s^2



// texturas espaciais

/*
TEXTURE_MAP texturaChao = {
    .Xi = 0.0f,
    .Yi = 0.75f,
    .Xf = 0.25f,
    .Yf = 1.0f
    };
*/

TEXTURE_MAP texturaChao = {
    .Xi = 0.01f,
    .Yi = 0.76f,
    .Xf = 0.24f,
    .Yf = 0.99f
    };

/*
TEXTURE_MAP texturaCeu = {
    .Xi = 0.25f,
    .Yi = 0.75f,
    .Xf = 0.50f,
    .Yf = 1.0f
    };
*/

TEXTURE_MAP texturaCeu = {
    .Xi = 0.26f,
    .Yi = 0.76f,
    .Xf = 0.49f,
    .Yf = 0.99f
    };

/*
TEXTURE_MAP texturaParede = {
    .Xi = 0.5f,
    .Yi = 0.75f,
    .Xf = 0.75f,
    .Yf = 1.0f
    };
*/

TEXTURE_MAP texturaParede = {
    .Xi = 0.51f,
    .Yi = 0.76f,
    .Xf = 0.74f,
    .Yf = 0.99f
    };

/*
TEXTURE_MAP texturaPiso = {
    .Xi = 0.75f,
    .Yi = 0.75f,
    .Xf = 1.0f,
    .Yf = 1.0f
    };
*/

TEXTURE_MAP texturaPiso = {
    .Xi = 0.76f,
    .Yi = 0.76f,
    .Xf = 0.99f,
    .Yf = 0.99f
    };

TEXTURE_MAP texturaTelhado1 = {
    .Xi = 0.01f,
    .Yi = 0.51f,
    .Xf = 0.24f,
    .Yf = 0.74f
    };

TEXTURE_MAP texturaForo = {
    .Xi = 0.26f,
    .Yi = 0.51f,
    .Xf = 0.49f,
    .Yf = 0.74f
    };

TEXTURE_MAP texturaTeto = {
    .Xi = 0.51f,
    .Yi = 0.51f,
    .Xf = 0.74f,
    .Yf = 0.74f
    };

TEXTURE_MAP texturaVidraca = {
    .Xi = 0.76f,
    .Yi = 0.51f,
    .Xf = 0.89f,
    .Yf = 0.74f
    };

TEXTURE_MAP texturaPorta = {
    .Xi = 0.01f,
    .Yi = 0.26f,
    .Xf = 0.24f,
    .Yf = 0.49f
    };

TEXTURE_MAP texturaTelhado2 = {
    .Xi = 0.26f,
    .Yi = 0.26f,
    .Xf = 0.49f,
    .Yf = 0.49f
    };

TEXTURE_MAP texturaMadeira = {
    .Xi = 0.51f,
    .Yi = 0.26f,
    .Xf = 0.74f,
    .Yf = 0.49f
    };


long qtdeVertices = 0;


void ImprimeInicializacao(){

     //imprime opções de jogabilidade
    printf("\n_____________________________________________________\n\n");

    printf("Controles de jogabilidade\n\n");

    printf("\tW      ->  Andar para FRENTE\n");
    printf("\tS      ->  Andar para TRAS\n");
    printf("\tA      ->  Andar para ESQUERDA\n");
    printf("\tD      ->  Andar para DIREITA\n");
    printf("\tESPACO ->  PULAR\n\n");

    printf("\tSHIFT  ->  Ativar JETPACK para SUBIR\n");
    printf("\tCTRL   ->  Ativar JETPACK para DESCER\n\n");

    printf("\t^      ->  Olhar para CIMA\n");
    printf("\tv      ->  Olhar para BAIXO\n");
    printf("\t<      ->  Olhar para o lado ESQUERDO\n");
    printf("\t>      ->  Olhar para o lado DIREITO\n\n");

    printf("\tE      ->  Aumentar Zoon\n");
    printf("\tC      ->  Diminuir Zoon\n\n");

    printf("\tR      ->  Resetar\n");



    //imprime valores internos
    printf("_____________________________________________________\n\n");


    printf("Valores Internos\n\n");

    printf("\tSCR_WIDTH -> %d\n", SCR_WIDTH);
    printf("\tSCR_WIDTH -> %d\n", SCR_HEIGHT);
    printf("\tcameraSpeed -> %f\n", cameraSpeed);
    printf("\tyaw -> %f\n", yaw);
    printf("\tpitch -> %f\n", pitch);
    printf("\tzoom -> %f\n", zoom);
    printf("\tspeedRateVelocidade -> %f\n", speedRateVelocidade);
    printf("\tlimite_chao -> %f\n", limite_chao);
    printf("\tlimite_ceu -> %f\n", limite_ceu);
    printf("\tlimite_mapa -> %f\n", limite_mapa);
    printf("\tsize_peca -> %f\n", size_peca);
    printf("\tgravidade -> %f\n", gravidade);
    printf("\taltura_personagem -> %f\n", altura_personagem);
    printf("\tmassa -> %f\n", massa);
    printf("\tforca_pulo -> %f\n", forca_pulo);
    printf("\tvelocidade_inicial_pulo -> %f\n", forca_pulo / massa);


    printf("_____________________________________________________\n");



}


void reset() {

    cameraPos   = glm::vec3(-4.5f, limite_chao + altura_personagem,  15.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    cameraSpeed = 0.01f;

    sensitivity = 0.2f;
    yaw = -75.0f;
    pitch = 0.0f;
    zoom = 45.0f;

    speedRateVelocidade = 0.02f;

    altura_personagem = 1.0f;
    massa = 70.0f;
    forca_pulo = 200.0f;
    velocidade_inicial_pulo = 0.0f;
    velocidade_y = 0.0f;

    ImprimeInicializacao();

}



void zoomControl(float z) {

    zoom += z;
    if (zoom <1.0f)
        zoom = 1.0f;
    if (zoom> 100.0f)
        zoom = 100.0f;
}


void viraCamera(float x, float y)
{

        yaw   += x * sensitivity;
        pitch += y * sensitivity;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
}


void ConcatArray(float vet1[], int vet1_size, float vet2[], int vet2_size, float vet3[]){

    int index = 0;
    int index_vet;

    for(index_vet = 0; index_vet < vet1_size; index_vet++){
        vet3[index] = vet1[index_vet];
        index++;
    }

    for(index_vet = 0; index_vet < vet2_size; index_vet++){
        vet3[index] = vet2[index_vet];
        index++;
    }

}


void CriaBanco(float igreja_size, float x, float z, float banco[], int banco_size){

    float temp[] = {

        x * igreja_size, 0.02f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        x * igreja_size, 0.02f * igreja_size, z * igreja_size, texturaMadeira.Xi, texturaMadeira.Yf,
        (x+0.15f) * igreja_size, 0.02f * igreja_size, z * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,

        x * igreja_size, 0.02f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        (x+0.15f) * igreja_size, 0.02f * igreja_size, z * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,
        (x+0.15f) * igreja_size, 0.02f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yi,




        x * igreja_size, 0.00f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        x * igreja_size, 0.04f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xi, texturaMadeira.Yf,
        (x+0.15f) * igreja_size, 0.04f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,

        x * igreja_size, 0.00f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        (x+0.15f) * igreja_size, 0.04f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,
        (x+0.15f) * igreja_size, 0.00f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yi,





        x * igreja_size, 0.00f * igreja_size, z * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        x * igreja_size, 0.02f * igreja_size, z * igreja_size, texturaMadeira.Xi, texturaMadeira.Yf,
        x * igreja_size, 0.02f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,

        x * igreja_size, 0.00f * igreja_size, z * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        x * igreja_size, 0.02f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,
        x * igreja_size, 0.00f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yi,



        (x+0.15f) * igreja_size, 0.00f * igreja_size, z * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        (x+0.15f) * igreja_size, 0.02f * igreja_size, z * igreja_size, texturaMadeira.Xi, texturaMadeira.Yf,
        (x+0.15f) * igreja_size, 0.02f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,

        (x+0.15f) * igreja_size, 0.00f * igreja_size, z * igreja_size, texturaMadeira.Xi, texturaMadeira.Yi,
        (x+0.15f) * igreja_size, 0.02f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yf,
        (x+0.15f) * igreja_size, 0.00f * igreja_size, (z+0.03) * igreja_size, texturaMadeira.Xf, texturaMadeira.Yi,

    };

    for(int i = 0; i < banco_size; i++){
        banco[i] = temp[i];
    }

}

int main()
{
    reset();


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Caravaggio Church", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if(glewInit()!=GLEW_OK) {
        std::cout << "Ocorreu um erro iniciando GLEW!" << std::endl;
    } else {
        std::cout << "GLEW OK!" << std::endl;
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertex.glsl", "fragment.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    qtdeVertices = 0;



    // CONSTRUÇÂO DO MAPA
    //_______________________________________________________________________
    int qtde_pecas = limite_mapa * 2 / size_peca;
    qtde_pecas *= qtde_pecas;

    int arraySize = qtde_pecas * 30; // 30 valores para cada quadrado

    // CONSTRUÇÂO DO CHAO
    //_______________________________________________________________________

    qtdeVertices += qtde_pecas * 6; // 6 vertices por quadrado

    float chao[arraySize];
    int index = 0;
    for(float Z = -limite_mapa; Z < limite_mapa; Z += size_peca){

        for(float X = -limite_mapa; X < limite_mapa; X += size_peca){

            //triangulo 1
            //_______________________________________________
            chao[index] = X; index++; // x
            chao[index] = limite_chao; index++;// y
            chao[index] = Z + size_peca; index++;// z
            chao[index] = texturaChao.Xi; index++; // mapeamento em x
            chao[index] = texturaChao.Yi; index++; // mpaeamento em y


            chao[index] = X; index++; // x
            chao[index] = limite_chao; index++; // y
            chao[index] = Z; index++; // z
            chao[index] = texturaChao.Xi; index++; // mapeamento em x
            chao[index] = texturaChao.Yf; index++; // mpaeamento em y


            chao[index] = X + size_peca; index++; // x
            chao[index] = limite_chao; index++; // y
            chao[index] = Z; index++; // z
            chao[index] = texturaChao.Xf; index++; // mapeamento em x
            chao[index] = texturaChao.Yf; index++; // mpaeamento em y


            //triangulo 2
            //_______________________________________________
            chao[index] = X; index++; // x
            chao[index] = limite_chao; index++;// y
            chao[index] = Z + size_peca; index++;// z
            chao[index] = texturaChao.Xi; index++; // mapeamento em x
            chao[index] = texturaChao.Yi; index++; // mpaeamento em y


            chao[index] = X + size_peca; index++; // x
            chao[index] = limite_chao; index++; // y
            chao[index] = Z; index++; // z
            chao[index] = texturaChao.Xf; index++; // mapeamento em x
            chao[index] = texturaChao.Yf; index++; // mpaeamento em y


            chao[index] = X + size_peca; index++; // x
            chao[index] = limite_chao; index++; // y
            chao[index] = Z + size_peca; index++; // z
            chao[index] = texturaChao.Xf; index++; // mapeamento em x
            chao[index] = texturaChao.Yi; index++; // mpaeamento em y

        }

    }



    // CONSTRUÇÂO DO CEU
    //_______________________________________________________________________

    /*
    qtdeVertices += qtde_pecas * 6;// 6 vertices por quadrado

    float ceu[arraySize];
    index = 0;
    for(float Z = -limite_mapa; Z < limite_mapa; Z += size_peca){

        for(float X = -limite_mapa; X < limite_mapa; X += size_peca){

            //triangulo 1
            //_______________________________________________
            ceu[index] = X; index++; // x
            ceu[index] = limite_ceu; index++;// y
            ceu[index] = Z + size_peca; index++;// z
            ceu[index] = texturaCeu.Xi; index++; // mapeamento em x
            ceu[index] = texturaCeu.Yi; index++; // mpaeamento em y


            ceu[index] = X; index++; // x
            ceu[index] = limite_ceu; index++; // y
            ceu[index] = Z; index++; // z
            ceu[index] = texturaCeu.Xi; index++; // mapeamento em x
            ceu[index] = texturaCeu.Yf; index++; // mpaeamento em y


            ceu[index] = X + size_peca; index++; // x
            ceu[index] = limite_ceu; index++; // y
            ceu[index] = Z; index++; // z
            ceu[index] = texturaCeu.Xf; index++; // mapeamento em x
            ceu[index] = texturaCeu.Yf; index++; // mpaeamento em y


            //triangulo 2
            //_______________________________________________
            ceu[index] = X; index++; // x
            ceu[index] = limite_ceu; index++;// y
            ceu[index] = Z + size_peca; index++;// z
            ceu[index] = texturaCeu.Xi; index++; // mapeamento em x
            ceu[index] = texturaCeu.Yi; index++; // mpaeamento em y


            ceu[index] = X + size_peca; index++; // x
            ceu[index] = limite_ceu; index++; // y
            ceu[index] = Z; index++; // z
            ceu[index] = texturaCeu.Xf; index++; // mapeamento em x
            ceu[index] = texturaCeu.Yf; index++; // mpaeamento em y


            ceu[index] = X + size_peca; index++; // x
            ceu[index] = limite_ceu; index++; // y
            ceu[index] = Z + size_peca; index++; // z
            ceu[index] = texturaCeu.Xf; index++; // mapeamento em x
            ceu[index] = texturaCeu.Yi; index++; // mpaeamento em y

        }

    }
    */


    qtdeVertices += 6;// 6 vertices por quadrado

    float ceu[] = {

        -limite_mapa, limite_ceu, -limite_mapa, texturaCeu.Xi, texturaCeu.Yi,
        -limite_mapa, limite_ceu,  limite_mapa, texturaCeu.Xi, texturaCeu.Yf,
         limite_mapa, limite_ceu,  limite_mapa, texturaCeu.Xf, texturaCeu.Yf,




        -limite_mapa, limite_ceu, -limite_mapa, texturaCeu.Xi, texturaCeu.Yi,
         limite_mapa, limite_ceu,  limite_mapa, texturaCeu.Xf, texturaCeu.Yf,
         limite_mapa, limite_ceu, -limite_mapa, texturaCeu.Xf, texturaCeu.Yi,

    };

    qtdeVertices += sizeof(ceu) / sizeof(float) / 5;








    // CONSTRUÇÂO DA IGREJA
    //_______________________________________________________________________

    float igreja_size = 14.0f;
    float igreja_altura = 0.5f;
    float igreja_altura_telhado = 0.2f;
    float igreja_altura_cupola = 0.4f;
    float igreja_altura_porta = 0.2f;

    float igreja_altura_parede_cupola_2= 0.2f;
    float igreja_altura_cupola_2= 0.1f;

    float igreja[] = {

        // Piso
        //________________________________________________________________________________________________
        -0.5f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.5f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
        -0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,

        -0.5f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
        -0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,






        -0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,

        -0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
         0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,






         0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.5f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,

         0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.5f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
         0.5f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,




        -0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.2f * igreja_size, 0.0f * igreja_size,  0.6f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.2f * igreja_size, 0.0f * igreja_size,  0.6f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,

        -0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.2f * igreja_size, 0.0f * igreja_size,  0.6f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
         0.2f * igreja_size, 0.0f * igreja_size,  0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,





        -0.2f * igreja_size, 0.0f * igreja_size, -0.6f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,

        -0.2f * igreja_size, 0.0f * igreja_size, -0.6f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.2f * igreja_size, 0.0f * igreja_size, -0.2f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
         0.2f * igreja_size, 0.0f * igreja_size, -0.6f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,







        -0.35f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
        -0.20f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
        -0.20f * igreja_size, 0.0f * igreja_size, -0.35f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,



        -0.35f * igreja_size, 0.0f * igreja_size,  0.20f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.20f * igreja_size, 0.0f * igreja_size,  0.35f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
        -0.20f * igreja_size, 0.0f * igreja_size,  0.20f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,




         0.20f * igreja_size, 0.0f * igreja_size,  0.20f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.20f * igreja_size, 0.0f * igreja_size,  0.35f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.35f * igreja_size, 0.0f * igreja_size,  0.20f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,



         0.20f * igreja_size, 0.0f * igreja_size, -0.35f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.20f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.35f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,















        // Foro
        //________________________________________________________________________________________________
        -0.5f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
        -0.5f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yf,
        -0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,

        -0.5f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
        -0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,
        -0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yi,






        -0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaForo.Xi, texturaForo.Yi,
        -0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaForo.Xi, texturaForo.Yf,
         0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaForo.Xf, texturaForo.Yf,

        -0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaForo.Xi, texturaForo.Yi,
         0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaForo.Xf, texturaForo.Yf,
         0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaForo.Xf, texturaForo.Yi,






         0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
         0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yf,
         0.5f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,

         0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
         0.5f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,
         0.5f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yi,




        -0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
        -0.2f * igreja_size, igreja_altura * igreja_size,  0.6f * igreja_size, texturaTeto.Xi, texturaTeto.Yf,
         0.2f * igreja_size, igreja_altura * igreja_size,  0.6f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,

        -0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
         0.2f * igreja_size, igreja_altura * igreja_size,  0.6f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,
         0.2f * igreja_size, igreja_altura * igreja_size,  0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yi,





        -0.2f * igreja_size, igreja_altura * igreja_size, -0.6f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
        -0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xi, texturaTeto.Yf,
         0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,

        -0.2f * igreja_size, igreja_altura * igreja_size, -0.6f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
         0.2f * igreja_size, igreja_altura * igreja_size, -0.2f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,
         0.2f * igreja_size, igreja_altura * igreja_size, -0.6f * igreja_size, texturaTeto.Xf, texturaTeto.Yi,








        // Paredes
        //________________________________________________________________________________________________


        -0.20f * igreja_size, 0.0f * igreja_size,  0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,


        -0.20f * igreja_size, 0.0f * igreja_size,  0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         0.20f * igreja_size, 0.0f * igreja_size,  0.60f * igreja_size, texturaParede.Xf, texturaParede.Yi,



        -0.20f * igreja_size, 0.0f * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,


        -0.20f * igreja_size, 0.0f * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         0.20f * igreja_size, 0.0f * igreja_size, -0.60f * igreja_size, texturaParede.Xf, texturaParede.Yi,





        -0.50f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
        -0.50f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,


        -0.50f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.50f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        -0.50f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,





         0.50f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         0.50f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,


         0.50f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.50f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         0.50f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,







        -0.20f * igreja_size, 0.0f * igreja_size, 0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.20f * igreja_size, igreja_altura * igreja_size, 0.25f * igreja_size, texturaParede.Xi, texturaParede.Yf,
        -0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,


        -0.20f * igreja_size, 0.0f * igreja_size, 0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        -0.20f * igreja_size, 0.0f * igreja_size, 0.60f * igreja_size, texturaParede.Xf, texturaParede.Yi,






         0.20f * igreja_size, 0.0f * igreja_size, 0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.20f * igreja_size, igreja_altura * igreja_size, 0.25f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,


         0.20f * igreja_size, 0.0f * igreja_size, 0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         0.20f * igreja_size, 0.0f * igreja_size, 0.60f * igreja_size, texturaParede.Xf, texturaParede.Yi,








        -0.20f * igreja_size, 0.0f * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yf,
        -0.20f * igreja_size, igreja_altura * igreja_size, -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,


        -0.20f * igreja_size, 0.0f * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.20f * igreja_size, igreja_altura * igreja_size, -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        -0.20f * igreja_size, 0.0f * igreja_size, -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yi,






         0.20f * igreja_size, 0.0f * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         0.20f * igreja_size, igreja_altura * igreja_size,  -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,


         0.20f * igreja_size, 0.0f * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.20f * igreja_size, igreja_altura * igreja_size,  -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         0.20f * igreja_size, 0.0f * igreja_size, -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yi,








         -0.50f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         -0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         -0.25f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,


         -0.50f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         -0.25f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         -0.25f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,



         -0.50f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         -0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         -0.25f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,


         -0.50f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         -0.25f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         -0.25f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,









          0.25f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
          0.25f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
          0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,


          0.25f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
          0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
          0.50f * igreja_size, 0.0f * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,



          0.25f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
          0.25f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
          0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,


          0.25f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
          0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
          0.50f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,







        -0.25f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.25f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
        -0.20f * igreja_size, igreja_altura * igreja_size, -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,

        -0.25f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        -0.20f * igreja_size, igreja_altura * igreja_size, -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        -0.20f * igreja_size, 0.0f * igreja_size, -0.25f * igreja_size, texturaParede.Xf, texturaParede.Yi,








         0.20f * igreja_size, 0.0f * igreja_size,  0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.20f * igreja_size, igreja_altura * igreja_size,  0.25f * igreja_size, texturaParede.Xi, texturaParede.Yf,
         0.25f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,

         0.20f * igreja_size, 0.0f * igreja_size,  0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
         0.25f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
         0.25f * igreja_size, 0.0f * igreja_size,  0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,



       -0.25f * igreja_size, 0.0f * igreja_size,  0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
       -0.25f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaParede.Xi, texturaParede.Yf,
       -0.20f * igreja_size, igreja_altura * igreja_size,  0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,

       -0.25f * igreja_size, 0.0f * igreja_size,  0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
       -0.20f * igreja_size, igreja_altura * igreja_size,  0.25f * igreja_size, texturaParede.Xf, texturaParede.Yf,
       -0.20f * igreja_size, 0.0f * igreja_size,  0.25f * igreja_size, texturaParede.Xf, texturaParede.Yi,





        0.20f * igreja_size, 0.0f * igreja_size, -0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.20f * igreja_size, igreja_altura * igreja_size, -0.25f * igreja_size, texturaParede.Xi, texturaParede.Yf,
        0.25f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,

        0.20f * igreja_size, 0.0f * igreja_size, -0.25f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.25f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        0.25f * igreja_size, 0.0f * igreja_size, -0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,







       -0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, -0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaParede.Xf, texturaParede.Yi,


       -0.20f * igreja_size, igreja_altura * igreja_size, 0.60f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.60f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        0.20f * igreja_size, igreja_altura * igreja_size, 0.60f * igreja_size, texturaParede.Xf, texturaParede.Yi,


       -0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
       -0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaParede.Xf, texturaParede.Yf,
       -0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,

        0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaParede.Xf, texturaParede.Yi,





       //TELHADO
       //____________________________________________________________________________________________________________________________________

       -0.20f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        0.00f, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.10f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.60f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,

        -0.20f * igreja_size, igreja_altura * igreja_size,  0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.60f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
        -0.20f * igreja_size, igreja_altura * igreja_size,  0.60f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,





        -0.20f * igreja_size, igreja_altura * igreja_size,  -0.60f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.00f, (igreja_altura + igreja_altura_telhado) * igreja_size, -0.60f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
         0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, -0.10f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,

        -0.20f * igreja_size, igreja_altura * igreja_size,  -0.60f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, -0.10f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
        -0.20f * igreja_size, igreja_altura * igreja_size,  -0.20f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,







        0.20f * igreja_size, igreja_altura * igreja_size, 0.60f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        0.00f, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.60f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.10f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,

        0.20f * igreja_size, igreja_altura * igreja_size, 0.60f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.10f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
        0.20f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,



        0.20f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        0.00f, (igreja_altura + igreja_altura_telhado) * igreja_size, -0.10f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, -0.60f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,

        0.20f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, -0.60f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
        0.20f * igreja_size, igreja_altura * igreja_size, -0.60f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,







        -0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        -0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
        -0.10f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,


        -0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        -0.10f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
        -0.20f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,






         0.20f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.10f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
         0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,


         0.20f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
         0.50f * igreja_size, igreja_altura * igreja_size, 0.20f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,





        -0.20f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        -0.10f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
        -0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,


        -0.20f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
        -0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
        -0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,





         0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.50f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yf,
         0.10f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,


         0.50f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.10f * igreja_size, (igreja_altura + igreja_altura_telhado) * igreja_size, 0.00f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yf,
         0.20f * igreja_size, igreja_altura * igreja_size, -0.20f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,


        //CUPOLA
        //____________________________________________________________________________________________________________________________________
        -0.45f * igreja_size, igreja_altura * igreja_size, 0.00f, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.00f * igreja_size, (igreja_altura + igreja_altura_cupola) * igreja_size, 0.00f, texturaTelhado1.Xi + (texturaTelhado1.Xf - texturaTelhado1.Xi)/2, texturaTelhado1.Yf,
         0.00f * igreja_size, igreja_altura* igreja_size, 0.45f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,


         0.45f * igreja_size, igreja_altura * igreja_size, 0.00f, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.00f * igreja_size, (igreja_altura + igreja_altura_cupola) * igreja_size, 0.00f, texturaTelhado1.Xi + (texturaTelhado1.Xf - texturaTelhado1.Xi)/2, texturaTelhado1.Yf,
         0.00f * igreja_size, igreja_altura* igreja_size,-0.45f * igreja_size, texturaTelhado1.Xf, texturaTelhado1.Yi,



         0.00f * igreja_size, igreja_altura * igreja_size, 0.45f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.00f * igreja_size, (igreja_altura + igreja_altura_cupola) * igreja_size, 0.00f, texturaTelhado1.Xi + (texturaTelhado1.Xf - texturaTelhado1.Xi)/2, texturaTelhado1.Yf,
         0.45f * igreja_size, igreja_altura* igreja_size, 0.00f, texturaTelhado1.Xf, texturaTelhado1.Yi,


         0.00f * igreja_size, igreja_altura * igreja_size,-0.45f * igreja_size, texturaTelhado1.Xi, texturaTelhado1.Yi,
         0.00f * igreja_size, (igreja_altura + igreja_altura_cupola) * igreja_size, 0.00f, texturaTelhado1.Xi + (texturaTelhado1.Xf - texturaTelhado1.Xi)/2, texturaTelhado1.Yf,
         -0.45f * igreja_size, igreja_altura* igreja_size, 0.00f, texturaTelhado1.Xf, texturaTelhado1.Yi,





        //VIDRAÇAS
        //____________________________________________________________________________________________________________________________________





         -0.10f * igreja_size, igreja_altura_porta * igreja_size, 0.601 * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
         -0.10f * igreja_size, igreja_altura * igreja_size, 0.601f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yf,
          0.10f * igreja_size, igreja_altura * igreja_size, 0.601f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,

         -0.10f * igreja_size, igreja_altura_porta * igreja_size, 0.601 * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
          0.10f * igreja_size, igreja_altura * igreja_size, 0.601f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,
          0.10f * igreja_size, igreja_altura_porta * igreja_size, 0.601 * igreja_size, texturaVidraca.Xf, texturaVidraca.Yi,





         -0.10f * igreja_size, igreja_altura_porta * igreja_size, -0.601 * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
         -0.10f * igreja_size, igreja_altura * igreja_size, -0.601f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yf,
          0.10f * igreja_size, igreja_altura * igreja_size, -0.601f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,

         -0.10f * igreja_size, igreja_altura_porta * igreja_size, -0.601 * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
          0.10f * igreja_size, igreja_altura * igreja_size, -0.601f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,
          0.10f * igreja_size, igreja_altura_porta * igreja_size, -0.601 * igreja_size, texturaVidraca.Xf, texturaVidraca.Yi,





         -0.501f * igreja_size, igreja_altura_porta * igreja_size, -0.10f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
         -0.501f * igreja_size, igreja_altura * igreja_size, -0.10f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yf,
         -0.501f * igreja_size, igreja_altura * igreja_size,  0.10f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,

         -0.501f * igreja_size, igreja_altura_porta * igreja_size, -0.10f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
         -0.501f * igreja_size, igreja_altura * igreja_size,  0.10f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,
         -0.501f * igreja_size, igreja_altura_porta * igreja_size,  0.10f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yi,






          0.501f * igreja_size, igreja_altura_porta * igreja_size, -0.10f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
          0.501f * igreja_size, igreja_altura * igreja_size, -0.10f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yf,
          0.501f * igreja_size, igreja_altura * igreja_size,  0.10f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,

          0.501f * igreja_size, igreja_altura_porta * igreja_size, -0.10f * igreja_size, texturaVidraca.Xi, texturaVidraca.Yi,
          0.501f * igreja_size, igreja_altura * igreja_size,  0.10f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yf,
          0.501f * igreja_size, igreja_altura_porta * igreja_size,  0.10f * igreja_size, texturaVidraca.Xf, texturaVidraca.Yi,





        // PORTAS
        //____________________________________________________________________________________________________________________________________


        -0.10f * igreja_size, 0.0f, 0.601f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
        -0.10f * igreja_size, igreja_altura_porta * igreja_size, 0.601f * igreja_size, texturaPorta.Xi, texturaPorta.Yf,
         0.10f * igreja_size, igreja_altura_porta * igreja_size,  0.601f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,

        -0.10f * igreja_size, 0.0f, 0.601f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
         0.10f * igreja_size, igreja_altura_porta * igreja_size,  0.601f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,
         0.10f * igreja_size, 0.0f,  0.601f * igreja_size, texturaPorta.Xf, texturaPorta.Yi,




        -0.10f * igreja_size, 0.0f, -0.601f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
        -0.10f * igreja_size, igreja_altura_porta * igreja_size, -0.601f * igreja_size, texturaPorta.Xi, texturaPorta.Yf,
         0.10f * igreja_size, igreja_altura_porta * igreja_size, -0.601f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,

        -0.10f * igreja_size, 0.0f, -0.601f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
         0.10f * igreja_size, igreja_altura_porta * igreja_size,  -0.601f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,
         0.10f * igreja_size, 0.0f,  -0.601f * igreja_size, texturaPorta.Xf, texturaPorta.Yi,




        -0.501f * igreja_size, 0.0f, -0.10f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
        -0.501f * igreja_size, igreja_altura_porta * igreja_size, -0.10f * igreja_size, texturaPorta.Xi, texturaPorta.Yf,
        -0.501f * igreja_size, igreja_altura_porta * igreja_size,  0.10f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,

        -0.501f * igreja_size, 0.0f, -0.10f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
        -0.501f * igreja_size, igreja_altura_porta * igreja_size,  0.10f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,
        -0.501f * igreja_size, 0.0f,  0.10f * igreja_size, texturaPorta.Xf, texturaPorta.Yi,





         0.501f * igreja_size, 0.0f, -0.10f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
         0.501f * igreja_size, igreja_altura_porta * igreja_size, -0.10f * igreja_size, texturaPorta.Xi, texturaPorta.Yf,
         0.501f * igreja_size, igreja_altura_porta * igreja_size,  0.10f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,

         0.501f * igreja_size, 0.0f, -0.10f * igreja_size, texturaPorta.Xi, texturaPorta.Yi,
         0.501f * igreja_size, igreja_altura_porta * igreja_size,  0.10f * igreja_size, texturaPorta.Xf, texturaPorta.Yf,
         0.501f * igreja_size, 0.0f,  0.10f * igreja_size, texturaPorta.Xf, texturaPorta.Yi,






        // CUPOLA 2
        //____________________________________________________________________________________________________________________________________

        0.00f, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.10f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.00f, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.10f * igreja_size, texturaParede.Xi, texturaParede.Yf,
        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaParede.Xf, texturaParede.Yf,

        0.00f, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.10f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaParede.Xf, texturaParede.Yf,
        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.00f, texturaParede.Xf, texturaParede.Yi,





        0.00f, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, -0.10f * igreja_size, texturaParede.Xi, texturaParede.Yi,
        0.00f, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, -0.10f * igreja_size, texturaParede.Xi, texturaParede.Yf,
       -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaParede.Xf, texturaParede.Yf,

        0.00f, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, -0.10f * igreja_size, texturaParede.Xi, texturaParede.Yi,
       -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaParede.Xf, texturaParede.Yf,
       -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.00f, texturaParede.Xf, texturaParede.Yi,




       -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.00f, texturaParede.Xi, texturaParede.Yi,
       -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaParede.Xi, texturaParede.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.10f * igreja_size, texturaParede.Xf, texturaParede.Yf,

        -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.00f, texturaParede.Xi, texturaParede.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.10f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.10f * igreja_size, texturaParede.Xf, texturaParede.Yi,



        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.00f, texturaParede.Xi, texturaParede.Yi,
        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaParede.Xi, texturaParede.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, -0.10f * igreja_size, texturaParede.Xf, texturaParede.Yf,

        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, 0.00f, texturaParede.Xi, texturaParede.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, -0.10f * igreja_size, texturaParede.Xf, texturaParede.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola - 0.1f) * igreja_size, -0.10f * igreja_size, texturaParede.Xf, texturaParede.Yi,





        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.10f * igreja_size, texturaTelhado2.Xi, texturaTelhado2.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 + igreja_altura_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaTelhado2.Xi + (texturaTelhado2.Xf - texturaTelhado2.Xi)/2, texturaTelhado2.Yf,
        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaTelhado2.Xf, texturaTelhado2.Yi,


        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, -0.10f * igreja_size, texturaTelhado2.Xi, texturaTelhado2.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 + igreja_altura_cupola_2 - 0.1f) * igreja_size, 0.0f, texturaTelhado2.Xi + (texturaTelhado2.Xf - texturaTelhado2.Xi)/2, texturaTelhado2.Yf,
       -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.0f, texturaTelhado2.Xf, texturaTelhado2.Yi,


       -0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaTelhado2.Xi, texturaTelhado2.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 + igreja_altura_cupola_2 - 0.1f) * igreja_size, 0.0f, texturaTelhado2.Xi + (texturaTelhado2.Xf - texturaTelhado2.Xi)/2, texturaTelhado2.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.10f * igreja_size, texturaTelhado2.Xf, texturaTelhado2.Yi,



        0.10f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, 0.00f, texturaTelhado2.Xi, texturaTelhado2.Yi,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 + igreja_altura_cupola_2 - 0.1f) * igreja_size, 0.0f, texturaTelhado2.Xi + (texturaTelhado2.Xf - texturaTelhado2.Xi)/2, texturaTelhado2.Yf,
        0.00f * igreja_size, (igreja_altura + igreja_altura_cupola + igreja_altura_parede_cupola_2 - 0.1f) * igreja_size, -0.10f * igreja_size, texturaTelhado2.Xf, texturaTelhado2.Yi,





        // ALTAR
        //____________________________________________________________________________________________________________________________________


        -0.2f * igreja_size, 0.02f * igreja_size, -0.25f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.2f * igreja_size, 0.02f * igreja_size, -0.60f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.2f * igreja_size, 0.02f * igreja_size, -0.60f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,

        -0.2f * igreja_size, 0.02f * igreja_size, -0.25f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.2f * igreja_size, 0.02f * igreja_size, -0.60f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
         0.2f * igreja_size, 0.02f * igreja_size, -0.25f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,




        -0.2f * igreja_size, 0.00f * igreja_size, -0.25f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
        -0.2f * igreja_size, 0.02f * igreja_size, -0.25f * igreja_size, texturaPiso.Xi, texturaPiso.Yf,
         0.2f * igreja_size, 0.02f * igreja_size, -0.25f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,

        -0.2f * igreja_size, 0.00f * igreja_size, -0.25f * igreja_size, texturaPiso.Xi, texturaPiso.Yi,
         0.2f * igreja_size, 0.02f * igreja_size, -0.25f * igreja_size, texturaPiso.Xf, texturaPiso.Yf,
         0.2f * igreja_size, 0.00f * igreja_size, -0.25f * igreja_size, texturaPiso.Xf, texturaPiso.Yi,





        -0.01f * igreja_size, igreja_altura/4 * igreja_size, -0.599f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
        -0.01f * igreja_size, igreja_altura/1.3f * igreja_size, -0.599f * igreja_size, texturaTeto.Xi, texturaTeto.Yf,
         0.01f * igreja_size, igreja_altura/1.3f * igreja_size, -0.599f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,

         -0.01f * igreja_size, igreja_altura/4 * igreja_size, -0.599f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
          0.01f * igreja_size, igreja_altura/1.3f * igreja_size, -0.599f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,
          0.01f * igreja_size, igreja_altura/4 * igreja_size, -0.599f * igreja_size, texturaTeto.Xf, texturaTeto.Yi,




        -0.1f * igreja_size, igreja_altura/1.5 * igreja_size, -0.599f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
        -0.1f * igreja_size, (igreja_altura/1.5 + 0.02) * igreja_size, -0.599f * igreja_size, texturaTeto.Xi, texturaTeto.Yf,
         0.1f * igreja_size, (igreja_altura/1.5 + 0.02) * igreja_size, -0.599f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,

         -0.1f * igreja_size, igreja_altura/1.5 * igreja_size, -0.599f * igreja_size, texturaTeto.Xi, texturaTeto.Yi,
         0.1f * igreja_size, (igreja_altura/1.5 + 0.02) * igreja_size, -0.599f * igreja_size, texturaTeto.Xf, texturaTeto.Yf,
         0.1f * igreja_size, (igreja_altura/1.5) * igreja_size, -0.599f * igreja_size, texturaTeto.Xf, texturaTeto.Yi,

    };

    qtdeVertices += sizeof(igreja)/sizeof(float) / 5;


    float banco1[120];
    CriaBanco(igreja_size, -0.20f, 0.17f, banco1, 120);
    qtdeVertices += 24;

    float banco2[120];
    CriaBanco(igreja_size, -0.20f, 0.1034f, banco2, 120);
    qtdeVertices += 24;


    float banco3[120];
    CriaBanco(igreja_size, -0.20f, 0.0368f, banco3, 120);
    qtdeVertices += 24;


    float banco4[120];
    CriaBanco(igreja_size, -0.20f, -0.0298f, banco4, 120);
    qtdeVertices += 24;


    float banco5[120];
    CriaBanco(igreja_size, -0.20f, -0.0964f, banco5, 120);
    qtdeVertices += 24;

    float banco6[120];
    CriaBanco(igreja_size, -0.20f, -0.163f, banco6, 120);
    qtdeVertices += 24;










    float banco7[120];
    CriaBanco(igreja_size, 0.05f, 0.17f, banco7, 120);
    qtdeVertices += 24;

    float banco8[120];
    CriaBanco(igreja_size, 0.05f, 0.1034f, banco8, 120);
    qtdeVertices += 24;


    float banco9[120];
    CriaBanco(igreja_size, 0.05f, 0.0368f, banco9, 120);
    qtdeVertices += 24;


    float banco10[120];
    CriaBanco(igreja_size, 0.05f, -0.0298f, banco10, 120);
    qtdeVertices += 24;


    float banco11[120];
    CriaBanco(igreja_size, 0.05f, -0.0964f, banco11, 120);
    qtdeVertices += 24;

    float banco12[120];
    CriaBanco(igreja_size, 0.05f, -0.163f, banco12, 120);
    qtdeVertices += 24;






    // JUNCAO DOS ELEMENTOS
    //_______________________________________________________________________

    int newSize = (sizeof(chao) + sizeof(ceu))/sizeof(float);

    float temp[newSize];
    ConcatArray(chao, sizeof(chao)/sizeof(float), ceu, sizeof(ceu)/sizeof(float), temp);


    newSize += sizeof(banco1)/sizeof(float);
    float temp2[newSize];
    ConcatArray(temp, sizeof(temp)/sizeof(float), banco1, sizeof(banco1)/sizeof(float), temp2);


    newSize += sizeof(banco2)/sizeof(float);
    float temp3[newSize];
    ConcatArray(temp2, sizeof(temp2)/sizeof(float), banco2, sizeof(banco2)/sizeof(float), temp3);


    newSize += sizeof(banco3)/sizeof(float);
    float temp4[newSize];
    ConcatArray(temp3, sizeof(temp3)/sizeof(float), banco3, sizeof(banco3)/sizeof(float), temp4);


    newSize += sizeof(banco4)/sizeof(float);
    float temp5[newSize];
    ConcatArray(temp4, sizeof(temp4)/sizeof(float), banco4, sizeof(banco4)/sizeof(float), temp5);

    newSize += sizeof(banco5)/sizeof(float);
    float temp6[newSize];
    ConcatArray(temp5, sizeof(temp5)/sizeof(float), banco5, sizeof(banco5)/sizeof(float), temp6);

    newSize += sizeof(banco6)/sizeof(float);
    float temp7[newSize];
    ConcatArray(temp6, sizeof(temp6)/sizeof(float), banco6, sizeof(banco6)/sizeof(float), temp7);

    newSize += sizeof(banco7)/sizeof(float);
    float temp8[newSize];
    ConcatArray(temp7, sizeof(temp7)/sizeof(float), banco7, sizeof(banco7)/sizeof(float), temp8);


    newSize += sizeof(banco8)/sizeof(float);
    float temp9[newSize];
    ConcatArray(temp8, sizeof(temp8)/sizeof(float), banco8, sizeof(banco8)/sizeof(float), temp9);

    newSize += sizeof(banco9)/sizeof(float);
    float temp10[newSize];
    ConcatArray(temp9, sizeof(temp9)/sizeof(float), banco9, sizeof(banco9)/sizeof(float), temp10);

    newSize += sizeof(banco10)/sizeof(float);
    float temp11[newSize];
    ConcatArray(temp10, sizeof(temp10)/sizeof(float), banco10, sizeof(banco10)/sizeof(float), temp11);

    newSize += sizeof(banco11)/sizeof(float);
    float temp12[newSize];
    ConcatArray(temp11, sizeof(temp11)/sizeof(float), banco11, sizeof(banco11)/sizeof(float), temp12);

    newSize += sizeof(banco12)/sizeof(float);
    float temp13[newSize];
    ConcatArray(temp12, sizeof(temp12)/sizeof(float), banco12, sizeof(banco12)/sizeof(float), temp13);



    newSize += sizeof(igreja)/sizeof(float);
    float vertices[newSize];
    ConcatArray(temp13, sizeof(temp13)/sizeof(float), igreja, sizeof(igreja)/sizeof(float), vertices);







    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture
    // -------------------------
    unsigned int texture1;

    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.


    //unsigned char *data = stbi_load("res/images/textura.png", &width, &height, &nrChannels, 0);
    //unsigned char *data = stbi_load("res/images/mapa.png", &width, &height, &nrChannels, STBI_rgb);
    //unsigned char *data = stbi_load("res/images/mapa.png", &width, &height, &nrChannels, STBI_rgb_alpha);


    //unsigned char *data = stbi_load("res/images/mapa_teste.png", &width, &height, &nrChannels, STBI_rgb_alpha);
    unsigned char *data = stbi_load("res/images/mapa.png", &width, &height, &nrChannels, STBI_rgb_alpha);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------

    /*
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("res/images/opengl.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    */

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    //ourShader.setInt("texture2", 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        EmulaGravidade();



        // render
        // ------
        glClearColor(0.6f, 0.7f, 1.0f, 1.0f); // azul
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // preto
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
       // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, -0.5f, 0.0f));
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 0.5f, -0.5f));

        model = glm::rotate(model, 0.3f, glm::vec3(0.0f, -0.5f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, -0.5f, 0.0f));


        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        /*
        const float radius = 2.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        view = glm::lookAt(glm::vec3(camX, 1.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        */

        viraCamera(0.0f, 0.0f);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
  		//   glm::vec3(0.0f, 0.0f, 0.0f),
  		//   glm::vec3(0.0f, 1.0f, 0.0f));

        projection = glm::perspective(glm::radians(zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");

        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        // render box
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawArrays(GL_TRIANGLES, 0, qtdeVertices);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){ // para frente
        //cameraPos += cameraSpeed * cameraFront;

        if( velocidade_inicial_pulo == 0 ){ // só permite se não estiver pulando
            float x = cameraPos.x + cameraFront.x * cameraSpeed;
            float z = cameraPos.z + cameraFront.z * cameraSpeed;

            if(x >= -limite_mapa && x <= limite_mapa && z >= -limite_mapa && z <= limite_mapa){
                cameraPos.x = x;
                cameraPos.z = z;
            }

        }

    }else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){ // para trás
        //cameraPos -= cameraSpeed * cameraFront;
        //cameraPos -= glm::vec3(cameraFront.x, 0, cameraFront.z) * cameraSpeed;

        if( velocidade_inicial_pulo == 0 ){ // só permite se não estiver pulando
            float x = cameraPos.x - cameraFront.x * cameraSpeed;
            float z = cameraPos.z - cameraFront.z * cameraSpeed;

            if(x >= -limite_mapa && x <= limite_mapa && z >= -limite_mapa && z <= limite_mapa){
                cameraPos.x = x;
                cameraPos.z = z;
            }
        }

    }else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){ // para esquerda

        if( velocidade_inicial_pulo == 0 ){ // só permite se não estiver pulando
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

    }else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){ // para direita

        if( velocidade_inicial_pulo == 0 ){ // só permite se não estiver pulando
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

    }


    if (glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS){ // pula
        // GLFW_KEY_LEFT_SHIFT
        // GLFW_KEY_SPACE

        if( cameraPos.y <= limite_chao + altura_personagem ){ // só permite se estiver no chão
            printf("Personagem Pulou\n");

            velocidade_inicial_pulo = forca_pulo / massa; //sempre reseta, pois massa e forca pode3m mudar;
                                                      // F = m.a -> a = F/m;
            velocidade_y = velocidade_inicial_pulo;

        }

    }else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){ // para cima

        if( velocidade_inicial_pulo == 0 ){ // só permite se nãop estiver pulando

            float temp = 1.0f * cameraSpeed;
            if(cameraPos.y + temp + 0.1f <= limite_ceu){ // + 0.1f serve para o campo de visão não ultrapassar o nível do céu
                cameraPos.y += temp;
                printf("Personagem ativou JETPACK para SUBIR. Altura: %f\n", cameraPos.y);
            }


        }

    }else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS){ // para baixo

        if( velocidade_inicial_pulo == 0 ){ // só permite se não estiver pulando

            float temp = 1.0f * cameraSpeed;
            float y = limite_chao + altura_personagem;
            if((cameraPos.y - temp) >= y){
                cameraPos.y -= temp;
                printf("Personagem ativou JETPACK para DESCER. Altura: %f\n", cameraPos.y);
            }else{
                cameraPos.y = y;
            }
        }

    }


    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){ // move camera para cima
        viraCamera(0.0f,1.0f);

    }else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){ // move camera para baixo
        viraCamera(0.0f,-1.0f);

    }else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){ // move camera para esquerda
        viraCamera(-1.0f,0.0f);

    }else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){ // move camera para direita
        viraCamera(1.0f,0.0f);

    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){ // aumenta zoon
        zoomControl(0.5f);

    }else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){ // diminui zoon
        zoomControl(-0.5f);

    }else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){ // reseta camera
        printf("\n\n\n\n\n\n\n\nAmbiente Resetado!\n");
        reset();
    }


}

void EmulaGravidade(){

    // Vf^2 = Vi^2 - 2aH
    //
    // Vf -> velocidade atual
    // Vi -> velocidade inicial
    // a -> aceleração(pode ser grtavidade)
    // H -> distancia relativa entre os objetos(pode ser Y)

    // Vf^2 = Vi^2 - 2 * g * h

    // Vf^2 - Vi^2 = - 2 * g * h

    // h = (Vf^2 - Vi^2)/(- 2 * g)

    if(velocidade_inicial_pulo > 0.0f){ // esse é necessário para não calcular a altura zero de forma continua sem necessidade

        velocidade_y -= speedRateVelocidade;
        float y = (velocidade_y * velocidade_y - velocidade_inicial_pulo * velocidade_inicial_pulo) / (-2 * gravidade);

        float temp = limite_chao + altura_personagem;
        y += temp;// incrementa a altura da cabeça em relação ao solo;

        if(y > temp){
            cameraPos.y = y;
        }else{
            cameraPos.y = temp;// pode haver algum aredondamento, então é melhor setar;
            velocidade_y = 0.0f;
            velocidade_inicial_pulo = 0.0f;
        }

    }


}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

