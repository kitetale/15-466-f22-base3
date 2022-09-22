#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// load mesh 
GLuint hitpad_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > hitpad_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("hitpad.pnct"));
	hitpad_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > hitpad_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("hitpad.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = hitpad_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = hitpad_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});

// load sounds 
Load< Sound::Sample > sound1_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("sound1.wav"));
});
Load< Sound::Sample > sound2_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("sound2.wav"));
});
Load< Sound::Sample > sound3_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("sound3.wav"));
});
Load< Sound::Sample > sound4_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("sound4.wav"));
});
Load< Sound::Sample > sound5_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("sound5.wav"));
});
Load< Sound::Sample > sound6_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("sound6.wav"));
});
Load< Sound::Sample > sound7_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("sound7.wav"));
});

PlayMode::PlayMode() : scene(*hitpad_scene) {
	//get pointers to leg for convenience:
	// for (auto &transform : scene.transforms) {
	// 	if (transform.name == "Hip.FL") hip = &transform;
	// 	else if (transform.name == "UpperLeg.FL") upper_leg = &transform;
	// 	else if (transform.name == "LowerLeg.FL") lower_leg = &transform;
	// }
	// if (hip == nullptr) throw std::runtime_error("Hip not found.");
	// if (upper_leg == nullptr) throw std::runtime_error("Upper leg not found.");
	// if (lower_leg == nullptr) throw std::runtime_error("Lower leg not found.");

	for (auto &transform : scene.transforms) {
		if (transform.name == "pad.1") pad1 = &transform;
		if (transform.name == "pad.2") pad2 = &transform;
		if (transform.name == "pad.3") pad3 = &transform;
		if (transform.name == "pad.4") pad4 = &transform;
		if (transform.name == "pad.5") pad5 = &transform;
		if (transform.name == "pad.6") pad6 = &transform;
		if (transform.name == "pad.7") pad7 = &transform;
		if (transform.name == "Torus") reverse = &transform;
	}
	pad1_position = pad1->position;
	pad2_position = pad2->position;
	pad3_position = pad3->position;
	pad4_position = pad4->position;
	pad5_position = pad5->position;
	pad6_position = pad6->position;
	pad7_position = pad7->position;
	reverse_position = reverse->position;

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	//game starts
	game = true;
	demo = false;
	addPad();
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (!game) return false;
	if (demo) return true;

	// e s x g m k i 
	// blue,red,green,center,purple,yellow,navy

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_e) {
			blue.downs += 1;
			blue.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			red.downs += 1;
			red.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_x) {
			green.downs += 1;
			green.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_m) {
			purple.downs += 1;
			purple.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_k) {
			yellow.downs += 1;
			yellow.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_i) {
			navy.downs += 1;
			navy.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_g) {
			center.downs += 1;
			center.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_e) {
			hitPad1();
			player_enter.push_back(1);
			++pressCount;
			blue.pressed = false;
			checkAnswer();
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			hitPad2();
			player_enter.push_back(2);
			++pressCount;
			red.pressed = false;
			checkAnswer();
			return true;
		} else if (evt.key.keysym.sym == SDLK_x) {
			hitPad3();
			player_enter.push_back(3);
			++pressCount;
			green.pressed = false;
			checkAnswer();
			return true;
		} else if (evt.key.keysym.sym == SDLK_m) {
			hitPad4();
			player_enter.push_back(4);
			++pressCount;
			purple.pressed = false;
			checkAnswer();
			return true;
		} else if (evt.key.keysym.sym == SDLK_k) {
			hitPad5();
			player_enter.push_back(5);
			++pressCount;
			yellow.pressed = false;
			checkAnswer();
			return true;
		} else if (evt.key.keysym.sym == SDLK_i) {
			hitPad6();
			player_enter.push_back(6);
			++pressCount;
			navy.pressed = false;
			checkAnswer();
			return true;
		} else if (evt.key.keysym.sym == SDLK_g) {
			hitPad7();
			player_enter.push_back(7);
			++pressCount;
			center.pressed = false;
			checkAnswer();
			return true;
		}
	}

	return false;
}

void PlayMode::hitPad1(){
	Sound::play(*sound1_sample, 1.0f);
	pad7->position.z -= 1;
}
void PlayMode::hitPad2(){
	Sound::play(*sound2_sample, 1.0f);
	pad1->position.z -= 1;
}
void PlayMode::hitPad3(){
	Sound::play(*sound3_sample, 1.0f);
	pad2->position.z -= 1;
}
void PlayMode::hitPad4(){
	Sound::play(*sound4_sample, 1.0f);
	pad3->position.z -= 1;
}
void PlayMode::hitPad5(){
	Sound::play(*sound5_sample, 1.0f);
	pad4->position.z -= 1;
}
void PlayMode::hitPad6(){
	Sound::play(*sound6_sample, 1.0f);
	pad5->position.z -= 1;
}
void PlayMode::hitPad7(){
	Sound::play(*sound7_sample, 1.0f);
	pad6->position.z -= 1;
}

void PlayMode::addPad() {
	demo = true;
	++answerCount;
	int next = rand() % 7 + 1; //pick number btwn 1-7
	std::cout<<"next : "<<next<<std::endl;
	answer.push_back(next);
	playAnswer();
}

void PlayMode::playAnswer() {
	// reset player enter info
	player_enter.clear();
	pressCount = 0;
	// debug purpose print what's being played
	std::cout<<"[ ";
	for (int n : answer) {
		while(mutex){}; // busy waiting but can't think of other way to make it play one at a time
		std::cout<< n << ", ";

		// lock it
		mutex = true;
		switch(n) {
			case 1 :
				hitPad1();
				mutex = false;
				break;
			case 2 :
				hitPad2();
				mutex = false;
				break;
			case 3 :
				hitPad3();
				mutex = false;
				break;
			case 4 :
				hitPad4();
				mutex = false;
				break;
			case 5 :
				hitPad5();
				mutex = false;
				break;
			case 6 :
				hitPad6();
				mutex = false;
				break;
			case 7 :
				hitPad7();
				mutex = false;
				break;
			default :
				std::cout << "\n number "<< n <<" not in 1-7?!??"<<std::endl;
				break;
		}
	}
	std::cout<<" ]"<<std::endl;

	demo = false;
}

void PlayMode::checkAnswer() {
	std::cout<<answerCount<< " | "<<pressCount<<std::endl;
	if (answerCount > pressCount) return;
	if (answerCount == pressCount){
		int i = 0;
		for (int n : player_enter) {
			std::cout<<"player: [ "<<n<<", ";
			if (n != answer[i]) {
				std::cout<<"\n n:"<<n<<" answer:"<<answer[i]<<std::endl;
				--health;
				if (health <= 0) {
					std::cout << " GAME OVER! "<<std::endl;
					game = false;
				}
				// show answer again
				playAnswer();
				return;
			}
			++i;
		}
		std::cout<<"]"<<std::endl;
		++score;
		std::cout<< " Correct! "<< std::endl;
		addPad();
		return;
		
	}
}

void PlayMode::update(float elapsed) {
	{ //return position of pads if they're not in position
		if (pad7->position.z < 0.6) { //e
			pad7->position.z += 0.1;
		}
		if (pad1->position.z < -0.8) { //s
			pad1->position.z += 0.1;
		}
		if (pad2->position.z < -1) { //x
			pad2->position.z += 0.1;
		}
		if (pad3->position.z < -1) { //m
			pad3->position.z += 0.1;
		}
		if (pad4->position.z < -0.8) { //k
			pad4->position.z += 0.1;
		}
		if (pad5->position.z < 0.6) { //i
			pad5->position.z += 0.1;
		}
		if (pad6->position.z < 0.6) { //g
			pad6->position.z += 0.1;
		}
	}

	//reset button press counters:
	blue.downs = 0;
	red.downs = 0;
	green.downs = 0;
	purple.downs = 0;
	yellow.downs = 0;
	navy.downs = 0;
	center.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		lines.draw_text("Play in the order shown!",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Play in the order shown!",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
	GL_ERRORS();
}
