##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=545_5
ConfigurationName      :=Debug
WorkspacePath          :=/home/nearlyeveryone/CECS545/project5
ProjectPath            :=/home/nearlyeveryone/CECS545/project5/545_5
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=nearlyeveryone
Date                   :=22/10/17
CodeLitePath           :=/home/nearlyeveryone/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="545_5.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -lGL -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)/usr/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)glfw 
ArLibs                 :=  "libglfw.so" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/lib/x86_64-linux-gnu/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall -std=c++11 $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/City.cpp$(ObjectSuffix) $(IntermediateDirectory)/glad.c$(ObjectSuffix) $(IntermediateDirectory)/SalesmanRoute.cpp$(ObjectSuffix) $(IntermediateDirectory)/Shader.cpp$(ObjectSuffix) $(IntermediateDirectory)/TsmHelper.cpp$(ObjectSuffix) $(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nearlyeveryone/CECS545/project5/545_5/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/City.cpp$(ObjectSuffix): City.cpp $(IntermediateDirectory)/City.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nearlyeveryone/CECS545/project5/545_5/City.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/City.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/City.cpp$(DependSuffix): City.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/City.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/City.cpp$(DependSuffix) -MM City.cpp

$(IntermediateDirectory)/City.cpp$(PreprocessSuffix): City.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/City.cpp$(PreprocessSuffix) City.cpp

$(IntermediateDirectory)/glad.c$(ObjectSuffix): glad.c $(IntermediateDirectory)/glad.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/nearlyeveryone/CECS545/project5/545_5/glad.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/glad.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glad.c$(DependSuffix): glad.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/glad.c$(ObjectSuffix) -MF$(IntermediateDirectory)/glad.c$(DependSuffix) -MM glad.c

$(IntermediateDirectory)/glad.c$(PreprocessSuffix): glad.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glad.c$(PreprocessSuffix) glad.c

$(IntermediateDirectory)/SalesmanRoute.cpp$(ObjectSuffix): SalesmanRoute.cpp $(IntermediateDirectory)/SalesmanRoute.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nearlyeveryone/CECS545/project5/545_5/SalesmanRoute.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SalesmanRoute.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SalesmanRoute.cpp$(DependSuffix): SalesmanRoute.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SalesmanRoute.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SalesmanRoute.cpp$(DependSuffix) -MM SalesmanRoute.cpp

$(IntermediateDirectory)/SalesmanRoute.cpp$(PreprocessSuffix): SalesmanRoute.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SalesmanRoute.cpp$(PreprocessSuffix) SalesmanRoute.cpp

$(IntermediateDirectory)/Shader.cpp$(ObjectSuffix): Shader.cpp $(IntermediateDirectory)/Shader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nearlyeveryone/CECS545/project5/545_5/Shader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Shader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Shader.cpp$(DependSuffix): Shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Shader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Shader.cpp$(DependSuffix) -MM Shader.cpp

$(IntermediateDirectory)/Shader.cpp$(PreprocessSuffix): Shader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Shader.cpp$(PreprocessSuffix) Shader.cpp

$(IntermediateDirectory)/TsmHelper.cpp$(ObjectSuffix): TsmHelper.cpp $(IntermediateDirectory)/TsmHelper.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nearlyeveryone/CECS545/project5/545_5/TsmHelper.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TsmHelper.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TsmHelper.cpp$(DependSuffix): TsmHelper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TsmHelper.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TsmHelper.cpp$(DependSuffix) -MM TsmHelper.cpp

$(IntermediateDirectory)/TsmHelper.cpp$(PreprocessSuffix): TsmHelper.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TsmHelper.cpp$(PreprocessSuffix) TsmHelper.cpp

$(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(ObjectSuffix): WisdomOfCrowdsHelper.cpp $(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nearlyeveryone/CECS545/project5/545_5/WisdomOfCrowdsHelper.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(DependSuffix): WisdomOfCrowdsHelper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(DependSuffix) -MM WisdomOfCrowdsHelper.cpp

$(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(PreprocessSuffix): WisdomOfCrowdsHelper.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WisdomOfCrowdsHelper.cpp$(PreprocessSuffix) WisdomOfCrowdsHelper.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


