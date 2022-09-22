#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} blue,red,green,center,purple,yellow,navy;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	Scene::Transform *pad1 = nullptr;
	Scene::Transform *pad2 = nullptr;
	Scene::Transform *pad3 = nullptr;
	Scene::Transform *pad4 = nullptr;
	Scene::Transform *pad5 = nullptr;
	Scene::Transform *pad6 = nullptr;
	Scene::Transform *pad7 = nullptr;
	Scene::Transform *reverse = nullptr;
	
	glm::quat pad1_position;
	glm::quat pad2_position;
	glm::quat pad3_position;
	glm::quat pad4_position;
	glm::quat pad5_position;
	glm::quat pad6_position;
	glm::quat pad7_position;
	glm::quat reverse_position;


	std::shared_ptr< Sound::PlayingSample > sound1;
	std::shared_ptr< Sound::PlayingSample > sound2;
	std::shared_ptr< Sound::PlayingSample > sound3;
	std::shared_ptr< Sound::PlayingSample > sound4;
	std::shared_ptr< Sound::PlayingSample > sound5;
	std::shared_ptr< Sound::PlayingSample > sound6;
	std::shared_ptr< Sound::PlayingSample > sound7;

	
	//camera:
	Scene::Camera *camera = nullptr;

	// player score
	uint32_t score = 0;
	// player health
	uint8_t health = 3;

	// list of correct generated pad orders (Absolute truth)
	std::vector<int> answer;
	// Player's guess per round
	std::vector<int> player_enter;

	void addPad(); // generate a random number from 1-7 and add to answer vector
	void playAnswer(); // plays what's in answer vector

	void hitPad1();
	void hitPad2();
	void hitPad3();
	void hitPad4();
	void hitPad5();
	void hitPad6();
	void hitPad7();

	void checkAnswer();

	// game ongoing
	bool game = true;
	// answer playing
	bool demo = false;
	// mutex to play one note at a time 
	bool mutex = false;

	uint16_t answerCount = 0;
	uint16_t pressCount = 0;
};
