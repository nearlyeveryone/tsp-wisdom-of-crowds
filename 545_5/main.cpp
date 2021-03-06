/*
 * CECS 545
 * Fall 2017
 * Samual Chesak
 * 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <thread>

#include "City.hpp"
#include "SalesmanRoute.hpp"
#include "TsmHelper.hpp"
#include "WisdomOfCrowdsHelper.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "TrueTypeHelper.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
vector<City> ReadCities(string filePath);
void PrintRoute(SalesmanRoute route);
void PrintTravsversedRoute(SalesmanRoute route);
void PrintGraphRoute(SalesmanRoute route);
SalesmanRoute GeneticAlgorithm(vector<City> cities, int populationSize, int generations, bool reverseCrossover, bool adaptiveMutator);
void drawRoute(SalesmanRoute route, Shader shader, float offSetX, float offSety, float scaleFactor, float pointSize);
void drawGraph(vector<GraphPoint> graph, Shader shader, int generations, float offSetX, float offSety, float scaleFactor);
GLFWwindow* initalizeGlfw();
void runGeneticStatistics(vector<City> cities);
void generateSuperRoute(vector<City> cities);

int main(int argc, char* argv[])
{
	srand(time(NULL));
	//parse file and populate cities
	vector<City> cities;
	cities = ReadCities(argv[1]);
	
	//runGeneticStatistics(cities);
	generateSuperRoute(cities);
	
	return 0;
}

GLFWwindow* initalizeGlfw()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TSP: Genetic Algorithm", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	return window;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void drawRoute(SalesmanRoute route, Shader shader, float offSetX, float offSety, float scaleFactor, float pointSize)
{
	vector<float> vertices;
	
	for(unsigned int i = 0; i < route.GetRoute().size(); i++)
	{
		vertices.push_back((float)route.GetRoute()[i]._latitude/200*scaleFactor + offSetX - 0.25f);
		vertices.push_back((float)route.GetRoute()[i]._longitude/200*scaleFactor + offSety - 0.25f);
		vertices.push_back(0.0f);
	}
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	
	glUseProgram(shader.Id);
	shader.setFloat("myPointSize", pointSize);
	
	shader.set4f("myColor", 0.498f, 1.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, vertices.size()/3);
	
	shader.set4f("myColor", 0.275f, 0.519f, 0.706f, 1.0f);
	glDrawArrays(GL_POINTS, 0, vertices.size()/3);
	glDisableVertexAttribArray(0);
}

void drawGraph(vector<GraphPoint> graph, Shader shader, int generations, float offSetX, float offSety, float scaleFactor)
{
	vector<float> vertices;
	double normalSize = graph[0].distance;
	for(unsigned int i = 0; i < graph.size(); i++)
	{
		vertices.push_back((float)graph[i].generation/generations * scaleFactor + offSetX);
		vertices.push_back((float)graph[i].distance/normalSize * scaleFactor + offSety);
		vertices.push_back(0.0f);
	}
	
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	
	shader.set4f("myColor", 0.498f, 1.0f, 0.0f, 1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	shader.setFloat("myPointSize", 1.0f);
	
	glUseProgram(shader.Id);
	glDrawArrays(GL_POINTS, 0, vertices.size()/3);
}

vector<City> ReadCities(string filePath)
{
	vector<City> cities;
	ifstream inFile(filePath);
	string currentLine;
	if(!inFile)
	{
		cerr << "Unable to open system Cities file path.";
	}
	size_t currentPosition;
	bool sectionFound = false;
	while(getline(inFile, currentLine))
	{
		if(sectionFound)
		{
			vector<string> splitLine;
			
			istringstream iss(currentLine);
			string token;
			while(getline(iss, token, ' '))
			{
				splitLine.push_back(token);
			}
			
			City newCity(stod(splitLine[1]),stod(splitLine[2]),stoi(splitLine[0])); 
			cities.push_back(newCity);
		} else {
			currentPosition = currentLine.find("NODE_COORD_SECTION");
			if(currentPosition != string::npos)
			{
				sectionFound = true;
			}
		}
	}
	
	return cities;
}

void PrintRoute(SalesmanRoute route)
{
	vector<City> cities = route.GetRoute();
	
	cout << "Route: ";
	
	for(unsigned int i = 0; i < cities.size(); i++)
	{
		cout << cities[i].CityId << "->";
	}
	
	cout << cities[0].CityId << endl;
}

void generateSuperRoute(vector<City> cities)
{
	GLFWwindow* window = initalizeGlfw();
	Shader shader("../vshader.glsl", "../fshader.glsl");
	
	Shader fontShader("../fontvShader.glsl", "../fontfShader.glsl");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	fontShader.use();
	glUniformMatrix4fv(glGetUniformLocation(fontShader.Id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	TrueTypeHelper font;
	font.initalizeFace();
	
	vector<thread> algorithmThreads;
	vector<TsmHelper*> tsmHelpers;
	
	
	unsigned int populationSize = 40;
	unsigned int maxThreads = 4;
	
	int gaPopulation = 100;
	int generations = 300;
	
	WisdomOfCrowdsHelper* wisemanHelper;
	thread superThread;
	double avgGADist = 0;
	
	for(unsigned int i = 0; i < populationSize; i++)
	{
		TsmHelper* tsmHelper = new TsmHelper(cities);
		tsmHelpers.push_back(tsmHelper);
	}
	
	vector<unsigned int> currentThreads;
	
	bool doneProcessing = false;
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		string threadsText = "# of ga runs: " + to_string(algorithmThreads.size()) + "/" + to_string(tsmHelpers.size()); 
		font.RenderText(fontShader,threadsText, 25.0f, 500.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f)); 
		string popText = "ga population size: " + to_string(gaPopulation); 
		font.RenderText(fontShader,popText, 25.0f, 470.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f)); 
		string genText = "generations per ga: " + to_string(generations); 
		font.RenderText(fontShader,genText, 25.0f, 440.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));;
		
		if(!doneProcessing)
		{
			//remove finished threads
			for(unsigned int i = 0; i < currentThreads.size(); i++)
			{
				if((*tsmHelpers[currentThreads[i]]).IsJobFinished())
				{
					currentThreads.erase(currentThreads.begin()+i);
				}
			}
			
			//add new tsmHelpers that aren't finished and aren't processing
			while(currentThreads.size() < maxThreads)
			{
				unsigned int tempThreadCount = currentThreads.size();
				for(unsigned int i = 0; i < populationSize; i++)
				{
					bool isProcessing = false;
					for(unsigned int j = 0; j < currentThreads.size(); j++)
					{
						if(i == currentThreads[j])
						{
							isProcessing = true;
							break;
						}
					}
					
					if( !(*tsmHelpers[i]).IsJobFinished() && !isProcessing)
					{
						currentThreads.push_back(i);
						algorithmThreads.push_back(thread(&TsmHelper::GeneticAlgorithm, tsmHelpers[i], gaPopulation, generations));
						cout << "thread vector size: " << algorithmThreads.size() << endl;
					}
					if(currentThreads.size() == maxThreads)
					{
						break;
					}
				}
				if(tempThreadCount == currentThreads.size())
				{
					break;
				}
			}
			
			for(int i = 0; i < 4; i++)
			{
				drawRoute((*tsmHelpers[i+algorithmThreads.size() - 4]).GetBestRoute(), shader, -0.5 + i*0.40, 0.0, 0.75, 3.0);
				glm::vec3 color(0.5, 0.8f, 0.2f);
				font.RenderText(fontShader,
					"gen: " + to_string((*tsmHelpers[i+algorithmThreads.size() - 4]).GetCurrentGeneration()) + ", length: " + to_string((*tsmHelpers[i+algorithmThreads.size() - 4]).GetBestRoute().GetTotalDistance()),
					130.0f + i*150, 200.0f - 30*i, 0.3f, color);
			}
			
			//check if done
			doneProcessing = true;
			for(unsigned int i = 0; i < populationSize; i++)
			{
				if(!(*tsmHelpers[i]).IsJobFinished())
				{
					doneProcessing = false;
					break;
				}
			}
			
			if(doneProcessing)
			{
				//if done, create wiseman route
				vector<SalesmanRoute> routes;
				for(unsigned int i = 0; i < populationSize; i++)
				{
					avgGADist += (*tsmHelpers[i]).GetBestRoute().GetTotalDistance();
					vector<SalesmanRoute> popToInsert = (*tsmHelpers[i]).GetPopulation();
					routes.insert(routes.end(), popToInsert.begin(), popToInsert.end());
				}
				avgGADist = avgGADist/populationSize;
				wisemanHelper = new WisdomOfCrowdsHelper(routes);
				
				//superThread = thread(&WisdomOfCrowdsHelper::GenerateWiseman, wisemanHelper);
				superThread = thread(&WisdomOfCrowdsHelper::WisdomStatistics, wisemanHelper, 10000);
			}
		} else {
			if((*wisemanHelper).wiseStats.stdDeviation == -1)
			{
				drawRoute((*wisemanHelper).GetWiseman(), shader, -0.25f, -0.25f, 2.00f, 5.0f);
				string threadsText = "distance: " + to_string((*wisemanHelper).GetWiseman().GetTotalDistance());
				font.RenderText(fontShader,threadsText, 100.0f, 100.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			} else {
				drawRoute((*wisemanHelper)._wisemans[0], shader, -0.25f, -0.25f, 2.00f, 5.0f);
				glm::vec3 color(0.5, 0.8f, 0.2f);
				
				font.RenderText(fontShader,
					"average best GA distance: " + to_string(avgGADist),
					25.0f, 130.0f, 0.3f, color);
				font.RenderText(fontShader,
					"average distance: " + to_string((*wisemanHelper).wiseStats.average),
					25.0f, 100.0f, 0.3f, color);
				font.RenderText(fontShader,
					"std. dev. distance: " + to_string((*wisemanHelper).wiseStats.stdDeviation),
					25.0f, 70.0f, 0.3f, color);
				font.RenderText(fontShader,
					"max distance: " + to_string((*wisemanHelper).wiseStats.max),
					25.0f, 40.0f, 0.3f, color);
				font.RenderText(fontShader,
					"min distance: " + to_string((*wisemanHelper).wiseStats.min),
					25.0f, 10.0f, 0.3f, color);
			}
			
		}
		
		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		chrono::milliseconds dura(1000);
		this_thread::sleep_for(dura);
	}
	
	glfwTerminate();
}