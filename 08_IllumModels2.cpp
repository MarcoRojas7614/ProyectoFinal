/*
*
* 08 - Modelos de Iluminación
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>
#include <animatedmodel.h>
#include <irrKlang.h>
using namespace irrklang;
//mensaje
// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 720;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

glm::vec3 position(0.0f, 0.0f, 0.0f);
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);
float     trdpersonOffset = 1.5f;
float     scaleV = 0.025f;
float     rotateCharacter = 0.0f;
float	  door_offset = 0.0f;
float	  door_neg = 0.0f;
float	  door_rotation = 0.0f;

// Shaders
Shader* cubemapShader;
Shader* mLightsShader;
Shader* basicShader;
Shader* dynamicShader;
Shader* wavesShader;
Shader* proceduralShader;
// Carga la información del modelo
Model* house;
Model* lightDummy;
Model* sphere;
Model* Puertas;
Model* TiendaRopa;
Model* TiendaComida;
Model* banos;
Model* acuario;
Model* teko;
Model* alien;
Model* Estascionamineto;
Model* fachada;
Model* Banqueta;
Model* puertaizq;
Model* puertader;
Model* puertaFD;
Model* moon;


AnimatedModel* kirbi;
float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

float proceduralTime = 0.0f;
float wavesTime = 0.0f;
// Cubemap
CubeMap* mainCubeMap;

// Materiales
Material material;
Material facha;
Material tiendar;
Material tiendac;
Material bano;
Material acuar;
Material tek;
Material alie;
Material parking;
Material banque;

// Luces
std::vector<Light> gLights;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Illumination Models", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad house
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	mLightsShader = new Shader("shaders/11_PhongShaderMultLights.vs", "shaders/11_PhongShaderMultLights.fs");
	basicShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	TiendaRopa = new Model("models/IllumModels/TiendaRopaF.fbx");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");
	Puertas = new Model("models/IllumModels/PUERTASF.fbx");
	dynamicShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	TiendaComida = new Model("models/IllumModels/TiendaComidaF.fbx");
	banos = new Model("models/IllumModels/BANOF.fbx");
	acuario = new Model("models/IllumModels/acuarioF.fbx");
	teko = new Model("models/IllumModels/TEKOF.fbx");
	alien = new Model("models/IllumModels/TiendaAlienF.fbx");
	//Estascionamineto = new Model("models/IllumModels/parkingF.fbx");
	fachada = new Model("models/IllumModels/fachadaF.fbx");
	//Banqueta = new Model("models/IllumModels/banquetaF.fbx");
	//kirbi = new AnimatedModel("models/IllumModels/Pea.fbx");
	puertaizq= new Model("models/IllumModels/puertaizq.fbx");
	puertader = new Model("models/IllumModels/puertader.fbx");
	puertaFD = new Model("models/IllumModels/PUERTASFD.fbx");
	moon = new Model("models/IllumModels/moon.fbx");
	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/03/posx.jpg",
		"textures/cubemap/03/negx.jpg",
		"textures/cubemap/03/posy.jpg",
		"textures/cubemap/03/negy.jpg",
		"textures/cubemap/03/posz.jpg",
		"textures/cubemap/03/negz.jpg"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	// Configuración de luces
	//luz teko
	Light light01;
	light01.Position = glm::vec3(-16.0f, 8.0f, -15.0f);
	light01.Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	light01.Power = glm::vec4(4.0f, 4.0f, 4.0f, 1.0f);
	gLights.push_back(light01);
	//luz ropa
	Light light02;
	light02.Position = glm::vec3(17.0f, 8.0f, -15.0f);
	light02.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light02.Power = glm::vec4(6.0f, 6.0f, 6.0f, 1.0f);
	gLights.push_back(light02);
	//luz ropa
	Light light03;
	light03.Position = glm::vec3(0.0f, 250.0f, 0.0f);
	light03.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light03.Power = glm::vec4(4.0f, 4.0f, 4.0f, 1.0f);
	gLights.push_back(light03);
	
	Light light04;
	light04.Position = glm::vec3(0.0f, 20.0f, -15.0f);
	light04.Color = glm::vec4(0.2f, 0.2f, 0.0f, 1.0f);
	light04.Power = glm::vec4(6.0f, 6.0f, 6.0f, 1.0f);
	gLights.push_back(light04);
	//banos
	Light light05;
	light05.Position = glm::vec3(-18.0f, 10.0f, -78.0f);
	light05.Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	light05.Power = glm::vec4(2.0f, 1.0f, 1.0f, 1.0f);
	gLights.push_back(light05);
	//comida
	Light light06;
	light06.Position = glm::vec3(17.0f, 8.0f, -73.0f);
	light06.Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	light06.Power = glm::vec4(6.0f, 6.0f, 6.0f, 1.0f);
	gLights.push_back(light06);

	lightDummy = new Model("models/IllumModels/lightDummy.fbx");

	// Configuración de propiedades cristal
	// Tabla: http://devernay.free.fr/cours/opengl/materials.html
	material.ambient = glm::vec4(0.0f, 0.1f, 0.06f, 0.5f); // Color ambiental oscuro
	material.diffuse = glm::vec4(0.0f, 0.50980392f, 0.50980392f, 0.5f); // Color difuso cian
	material.specular = glm::vec4(0.50196078f, 0.50196078f, 0.50196078f, 0.5f); // Color especular blanco brillante
	material.transparency = 0.5f;//transparencia media
	
	//tiendaropa
	tiendar.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // colores mate
	tiendar.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // 
	tiendar.specular = glm::vec4(0.1f, 0.1f, 0.1f, 0.5f); //
	tiendar.transparency = 1.0f;//transparencia normal
	

	//tiendacomida
	tiendac.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // colores brilloso
	tiendac.diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f); // Apariencia de limpieza 
	tiendac.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f); //brillo moderado
	tiendac.transparency = 1.0f;//transparencia media
	
	//banos
	bano.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // opaco
	bano.diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f); // Apariencia de limpieza 
	bano.specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); //pocos reflejos
	bano.transparency = 1.0f;//transparencia normal
	// SoundEngine->play2D("sound/EternalGarden.mp3", true);
	
	//acuario
	acuar.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // opaco
	acuar.diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f); // Apariencia de limpieza 
	acuar.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // reflejos medios
	acuar.transparency = 1.0f;//transparencia normal
	
	//Teko
	tek.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // opaco
	tek.diffuse = glm::vec4(0.4f, 0.3f, 0.2f, 1.0f); // Apariencia de limpieza 
	tek.specular = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f); // reflejos medios
	tek.transparency = 1.0f;//transparencia normal
	
	//alien
	alie.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // opaco
	alie.diffuse = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f); // Apariencia de limpieza 
	alie.specular = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f); // reflejos medios
	alie.transparency = 1.0f;//transparencia normal
	/*
	//estacionmamineto
	parking.ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f); // opaco
	parking.diffuse = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f); // Apariencia de limpieza 
	parking.specular = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f); // reflejos altos
	parking.transparency = 1.0f;//transparencia normal
	*/
	//fachada
	parking.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // opaco
	parking.diffuse = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f); // Apariencia de limpieza 
	parking.specular = glm::vec4(0.3f, 0.3f, 0.2f, 1.0f); // reflejos altos
	parking.transparency = 1.0f;//transparencia normal

	//banqueta
	//banque.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // opaco
	//banque.diffuse = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f); // Apariencia de limpieza 
	//banque.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // reflejos altos
	//banque.transparency = 1.0f;//transparencia normal*/
	return true;
}


void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}


bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	//alpha
	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
	glm::mat4 view = camera.GetViewMatrix();

	// Cubemap (fondo)
	{
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}

	// Objeto estático (casa)
	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", camera.Position);

		//lightDummy->Draw(*basicShader);

		
		
		/*
		glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		//puerta 2
		Puertas->Draw(*mLightsShader);
		*/
		
		mLightsShader->setVec4("MaterialAmbientColor", tiendar.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", tiendar.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", tiendar.specular);
		mLightsShader->setFloat("transparency", tiendar.transparency);

		TiendaRopa->Draw(*mLightsShader);
		
		//tienda de comida
		mLightsShader->setVec4("MaterialAmbientColor", tiendac.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", tiendac.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", tiendac.specular);
		mLightsShader->setFloat("transparency", tiendac.transparency);
		TiendaComida->Draw(*mLightsShader);

		//banos
		mLightsShader->setVec4("MaterialAmbientColor", bano.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", bano.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", bano.specular);
		mLightsShader->setFloat("transparency", bano.transparency);
		banos->Draw(*mLightsShader);

		//acuario
		mLightsShader->setVec4("MaterialAmbientColor", acuar.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", acuar.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", acuar.specular);
		mLightsShader->setFloat("transparency", acuar.transparency);
		acuario->Draw(*mLightsShader);


		//teko
		mLightsShader->setVec4("MaterialAmbientColor", tek.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", tek.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", tek.specular);
		mLightsShader->setFloat("transparency", tek.transparency);
		teko->Draw(*mLightsShader);
		//puerta iz	
		
		//alien
		mLightsShader->setVec4("MaterialAmbientColor", alie.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", alie.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", alie.specular);
		mLightsShader->setFloat("transparency", alie.transparency);
		alien->Draw(*mLightsShader);

		//estacionamiento
		//mLightsShader->setVec4("MaterialAmbientColor", parking.ambient);
		//mLightsShader->setVec4("MaterialDiffuseColor", parking.diffuse);
		//mLightsShader->setVec4("MaterialSpecularColor", parking.specular);
		//mLightsShader->setFloat("transparency", parking.transparency);
		//Estascionamineto->Draw(*mLightsShader);


		//fachada
		mLightsShader->setVec4("MaterialAmbientColor", facha.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", facha.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", facha.specular);
		mLightsShader->setFloat("transparency", facha.transparency);
		fachada->Draw(*mLightsShader);

		// Aplicamos propiedades materiales de cristal
		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		// ubicacion de puerta EIZQ
		model = glm::translate(model, glm::vec3(-5.0771f, 0.0f, 0.0f));
		// Efecto de puerta con bisagra
		model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(00.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		Puertas->Draw(*mLightsShader);

		model = glm::mat4(1.0f);//reinicio de matriz 
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla
		// Aplicamos propiedades materiales de cristal
		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		model = glm::translate(model, glm::vec3(5.0771f, 0.0f, 0.0f));
		// Efecto de puerta con bisagra
		model = glm::rotate(model, glm::radians(door_neg), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(00.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		puertaFD->Draw(*mLightsShader);
			
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		//tineda TEKO
		model = glm::mat4(1.0f);//reinicio de matriz 
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla
		
		model = glm::translate(model, glm::vec3(6.956f, 19.231f, 0.05f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertaizq->Draw(*mLightsShader);

		model = glm::mat4(1.0f);//reinicio de matriz 
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla
		model = glm::translate(model, glm::vec3(6.956f, 11.041f, 0.05f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_neg), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		// Aplicamos propiedades materiales de cristal
		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);
		puertader->Draw(*mLightsShader);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0f);//reinicio de matriz acuario
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(-4.018f, 61.099f, 17.439f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
		mLightsShader->setMat4("model", model);
		puertaizq->Draw(*mLightsShader);

		model = glm::mat4(1.0f);//reinicio de matriz  Comida der
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(4.017f, 61.099f,17.439f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_neg), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertader->Draw(*mLightsShader);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0f);//reinicio de matriz  ropa
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(-6.913f, 11.231f, 0.097f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertaizq->Draw(*mLightsShader);
		model = glm::mat4(1.0f);//reinicio de matriz  
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(-6.913f, 19.502f, 0.097f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_neg), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertader->Draw(*mLightsShader);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0f);//reinicio de matriz  Comida izq
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(6.893f, 78.98f, 0.009f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertaizq->Draw(*mLightsShader);

		model = glm::mat4(1.0f);//reinicio de matriz  Comida der
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(6.893f, 70.89f, 0.009f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_neg ), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertader->Draw(*mLightsShader);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0f);//reinicio de matriz  alien
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(4.4402f, 29.314f, 17.238f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertaizq->Draw(*mLightsShader);

		model = glm::mat4(1.0f);//reinicio de matriz  Comida der
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));//rotacion para vista adecuada
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	//gurdamos escla

		model = glm::translate(model, glm::vec3(-3.5216f,29.314f, 17.238f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(door_neg), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		puertader->Draw(*mLightsShader);






//banqueta
		//mLightsShader->setVec4("MaterialAmbientColor", banque.ambient);
		//mLightsShader->setVec4("MaterialDiffuseColor", banque.diffuse);
		//mLightsShader->setVec4("MaterialSpecularColor", banque.specular);
		//mLightsShader->setFloat("transparency", banque.transparency);
		//Banqueta->Draw(*mLightsShader);
		

	}
	// Deplegamos los indicadores auxiliares de cada fuente de iluminación
	{
		basicShader->use();

		basicShader->setMat4("projection", projection);
		basicShader->setMat4("view", view);

		glm::mat4 model;

		for (size_t i = 0; i < gLights.size(); ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, gLights[i].Position);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			basicShader->setMat4("model", model);

			lightDummy->Draw(*basicShader);
		}

	}

	glUseProgram(0);

	glUseProgram(0);
	
	{
		// Activamos el shader de Phong
		proceduralShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(17.0f, 8.50f, -15.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 4.0f);
		proceduralShader->setFloat("height", 1.0f);

		moon->Draw(*proceduralShader);
		proceduralTime += 0.001;

	}

	glUseProgram(0);
	

	// Deplegamos los indicadores auxiliares de cada fuente de iluminación
	
	/*
	{
		kirbi->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, kirbi->gBones);

		// Dibujamos el modelo
		kirbi->Draw(*dynamicShader);
	}
	glUseProgram(0);
	*/
	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {


	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		gLights[0].Position.y += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		gLights[0].Position.y -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		gLights[0].Position.x += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		gLights[0].Position.x -= 0.01f;
		//.Position.x -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

	}//rotacion de puertas
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		//door_rotation += 0.1f;
		door_neg = -90.0f;
		door_rotation = 90.0f;
		//door_neg -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
		//door_rotation -= 0.1f;
		door_rotation =0.0f;
		//door_neg += 0.1f;
		door_neg = 0.0f;
		}
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
//draw
// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}