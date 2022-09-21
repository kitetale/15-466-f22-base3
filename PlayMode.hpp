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

	//hexapod leg to wobble:
	// Scene::Transform *hip = nullptr;
	// Scene::Transform *upper_leg = nullptr;
	// Scene::Transform *lower_leg = nullptr;
	// glm::quat hip_base_rotation;
	// glm::quat upper_leg_base_rotation;
	// glm::quat lower_leg_base_rotation;
	float wobble = 0.0f;

	Scene::Transform *pad1 = nullptr;
	Scene::Transform *pad2 = nullptr;
	Scene::Transform *pad3 = nullptr;
	Scene::Transform *pad4 = nullptr;
	Scene::Transform *pad5 = nullptr;
	Scene::Transform *pad6 = nullptr;
	Scene::Transform *pad7 = nullptr;
	Scene::Transform *reverse = nullptr;
	
	glm::quat pad1_rotation;
	glm::quat pad2_rotation;
	glm::quat pad3_rotation;
	glm::quat pad4_rotation;
	glm::quat pad5_rotation;
	glm::quat pad6_rotation;
	glm::quat pad7_rotation;
	glm::quat reverse_rotation;

	// glm::vec3 get_leg_tip_position();

	//music coming from the tip of the leg (as a demonstration):
	std::shared_ptr< Sound::PlayingSample > leg_tip_loop;

	std::shared_ptr< Sound::PlayingSample > sound1_loop;
	std::shared_ptr< Sound::PlayingSample > sound1;
	std::shared_ptr< Sound::PlayingSample > sound2;
	std::shared_ptr< Sound::PlayingSample > sound3;
	std::shared_ptr< Sound::PlayingSample > sound4;
	std::shared_ptr< Sound::PlayingSample > sound5;
	std::shared_ptr< Sound::PlayingSample > sound6;
	std::shared_ptr< Sound::PlayingSample > sound7;

	
	//camera:
	Scene::Camera *camera = nullptr;

};
