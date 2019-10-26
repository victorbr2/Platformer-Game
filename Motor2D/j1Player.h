#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__


#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"
#include "j1App.h"

struct SDL_Texture;
struct Collider;

enum player_status
{
	PLAYER_IDLE,
	PLAYER_FORWARD,
	PLAYER_BACKWARD,
	PLAYER_JUMP,
	PLAYER_IN_JUMP_FINISH,
};

class j1Player : public j1Module
{


public:
	j1Player();
	~j1Player();



	bool Start();
	bool Update(float dt) override;
	bool CleanUp() override;
	

public:

	bool jumpEnable = true;
	bool input = true;
	SDL_Texture* player_text = nullptr;
	Animation* current_animation = &idle;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation jump;
	iPoint position;
	Collider* colPlayer = nullptr;
	int health = 1;
	player_status status = PLAYER_IDLE;
	Uint32 jump_timer = 0;
	SDL_Rect r;

};

#endif // _j1PLAYER_H_