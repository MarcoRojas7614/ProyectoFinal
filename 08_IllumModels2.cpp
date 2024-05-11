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

// Shaders
Shader* cubemapShader;
Shader* mLightsShader;
Shader* basicShader;

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
	//TiendaRopa = new Model("models/IllumModels/TiendaRopaF.fbx");
	Puertas = new Model("models/IllumModels/PUERTASF.fbx");
	//TiendaComida = new Model("models/IllumModels/TiendaComidaF.fbx");
	//banos = new Model("models/IllumModels/BANOF.fbx");
	////acuario= new Model("models/IllumModels/ACUARIOF.fbx");
	//teko= new Model("models/IllumModels/TEKOF.fbx");
	alien=new Model("models/IllumModels/TiendaAlienF.fbx");
	//Estascionamineto = new Model("models/IllumModels/parkingF.fbx");
	fachada = new Model("models/IllumModels/fachadaF.fbx");


	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/01/posx.png",
		"textures/cubemap/01/negx.png",
		"textures/cubemap/01/posy.png",
		"textures/cubemap/01/negy.png",
		"textures/cubemap/01/posz.png",
		"textures/cubemap/01/negz.png"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	// Configuración de luces
	/*
	Light light01;
	light01.Position = glm::vec3(5.0f, 0.0f, -5.0f);
	light01.Color = glm::vec4(0.80f, 0.80f, 0.80f, 1.0f);
	light01.Power = glm::vec4(40.0f, 40.0f, 40.0f, 1.0f);
	gLights.push_back(light01);
	*/
	Light light02;
	light02.Position = glm::vec3(5.0f, 5.0f, -5.0f);
	light02.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light02.Power = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);
	gLights.push_back(light02);
	/*
	Light light03;
	light03.Position = glm::vec3(-5.0f, 0.0f, -5.0f);
	light03.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	light03.Power = glm::vec4(100.0f, 100.0f, 100.0f, 1.0f);
	gLights.push_back(light03);
	/*
	Light light04;
	light04.Position = glm::vec3(-5.0f, 2.0f, -5.0f);
	light04.Color = glm::vec4(0.2f, 0.2f, 0.0f, 1.0f);
	light04.Power = glm::vec4(60.0f, 60.0f, 60.0f, 1.0f);
	gLights.push_back(light04);*/

	lightDummy = new Model("models/IllumModels/lightDummy.fbx");

	// Configuración de propiedades cristal
	// Tabla: http://devernay.free.fr/cours/opengl/materials.html
	material.ambient = glm::vec4(0.0f, 0.1f, 0.06f, 0.5f); // Color ambiental oscuro
	material.diffuse = glm::vec4(0.0f, 0.50980392f, 0.50980392f, 0.5f); // Color difuso cian
	material.specular = glm::vec4(0.50196078f, 0.50196078f, 0.50196078f, 0.5f); // Color especular blanco brillante
	material.transparency = 0.5f;//transparencia media
	/*
	//tiendaropa/*
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
	*/
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

		// Aplicamos propiedades materiales de cristal
		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);
		
		Puertas->Draw(*mLightsShader);
		/*
		glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);
		/*
		//puerta 2
		Puertas->Draw(*mLightsShader);
		mLightsShader->setVec4("MaterialAmbientColor", tiendar.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", tiendar.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", tiendar.specular);
		mLightsShader->setFloat("transparency", tiendar.transparency);

		//TiendaRopa->Draw(*mLightsShader);
		//tienda de comida
		mLightsShader->setVec4("MaterialAmbientColor", tiendac.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", tiendac.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", tiendac.specular);
		mLightsShader->setFloat("transparency", tiendac.transparency);
		//TiendaComida->Draw(*mLightsShader);
		//banos
		mLightsShader->setVec4("MaterialAmbientColor", bano.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", bano.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", bano.specular);
		mLightsShader->setFloat("transparency", bano.transparency);
		//banos->Draw(*mLightsShader);

		//acuario
		mLightsShader->setVec4("MaterialAmbientColor", acuar.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", acuar.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", acuar.specular);
		mLightsShader->setFloat("transparency", acuar.transparency);
		//acuario->Draw(*mLightsShader);


		//teko
		mLightsShader->setVec4("MaterialAmbientColor", tek.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", tek.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", tek.specular);
		mLightsShader->setFloat("transparency", tek.transparency);
		//teko->Draw(*mLightsShader);
		*/
		//alien
		mLightsShader->setVec4("MaterialAmbientColor", alie.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", alie.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", alie.specular);
		mLightsShader->setFloat("transparency", alie.transparency);
		alien->Draw(*mLightsShader);
		/*
		//estacionamiento
		mLightsShader->setVec4("MaterialAmbientColor", parking.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", parking.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", parking.specular);
		mLightsShader->setFloat("transparency", parking.transparency);
		Estascionamineto->Draw(*mLightsShader);
		*/
		
		//fachada
		mLightsShader->setVec4("MaterialAmbientColor", facha.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", facha.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", facha.specular);
		mLightsShader->setFloat("transparency", facha.transparency);
		fachada->Draw(*mLightsShader);




	}

	glUseProgram(0);

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
