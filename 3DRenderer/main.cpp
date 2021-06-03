#include "Game/Game.h"


int main() {
	std::shared_ptr<Game> Program = Object::SpawnRoot<Game>();
	
	Program->Run();
	
	//Benchmark
	std::cout << "Number of frames: " << Program->numOfFrames << std::endl;
	std::cout << "Average fps: " << 1000.0f / (Program->totalTime / Program->numOfFrames) << std::endl;
}