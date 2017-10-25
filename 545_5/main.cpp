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
	
	shader.set4f("myColor", 0.498f, 1.0f, 0.0f, 1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	shader.setFloat("myPointSize", pointSize);
	
	glUseProgram(shader.Id);
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
	
	vector<thread> algorithmThreads;
	vector<TsmHelper*> tsmHelpers;
	
	
	unsigned int populationSize = (int)cities.size()*2;
	unsigned int maxThreads = 4;
	
	int gaPopulation = (int)cities.size()*2;
	int generations = 30*(int)cities.size();
	
	WisdomOfCrowdsHelper* wisemanHelper;
	thread superThread;
	
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
			
			/*//draw graphs
			int k = 0;
			for(unsigned int i = 0; i < 10; i++)
			{
				for(unsigned int j = 0; j < 10; j++)
				{
					drawRoute((*tsmHelpers[k]).GetBestRoute(), shader, (15.0f*j/populationSize)-.75, (15.0f*i/populationSize)-.5, (0.3f), 3.0f);
					k++;
				}
			}*/
			
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
					cout << "Population " << i << ":" << (*tsmHelpers[i]).GetBestRoute().GetTotalDistance() << endl;
					vector<SalesmanRoute> popToInsert = (*tsmHelpers[i]).GetPopulation();
					routes.insert(routes.end(), popToInsert.begin(), popToInsert.end());
				}
				wisemanHelper = new WisdomOfCrowdsHelper(routes);
				
				superThread = thread(&WisdomOfCrowdsHelper::GenerateWiseman, wisemanHelper);
			}
		} else {
			drawRoute((*wisemanHelper).GetWiseman(), shader, 0.0f, 0.0f, 2.00f, 5.0f);
			//cout << "Generation " << (*superRoute).GetCurrentGeneration() << ": " << (*superRoute).GetBestRoute().GetTotalDistance() << endl;
		}
		
		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		chrono::milliseconds dura(500);
		this_thread::sleep_for(dura);
	}
	
	glfwTerminate();
}