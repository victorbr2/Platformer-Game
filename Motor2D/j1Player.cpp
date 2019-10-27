#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Module.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "SDL_image/include/SDL_image.h"


 j1Player :: j1Player()
{
	position.x = 32;
	position.y = 384;

	// idle animation
	idle.PushBack({ 2, 8, 21, 34 });
	idle.PushBack({ 23, 8, 21, 34 });
	idle.PushBack({ 44, 8, 21, 34 });
	idle.PushBack({ 65, 8, 21, 34 });
	idle.PushBack({ 86, 7, 21, 35 });
	idle.PushBack({ 107, 7, 21, 35 });
	idle.PushBack({ 128, 7, 21, 35 });
	idle.PushBack({ 149, 7, 21, 35 });
	idle.PushBack({ 170, 8, 21, 34 });
	idle.PushBack({ 191, 9, 21, 33 });
	idle.PushBack({ 212, 9, 21, 33 });
	idle.PushBack({ 233, 9, 21, 33 });
	idle.speed = 0.075f;

	//forward
	forward.PushBack({ 2, 43, 23, 34 });
	forward.PushBack({ 26, 45, 20, 32 });
	forward.PushBack({ 47, 45, 17, 32 });
	forward.PushBack({ 64, 45, 20, 32 });
	forward.PushBack({ 86, 43, 20, 34 });
	forward.PushBack({ 106, 45, 19, 32 });
	forward.PushBack({ 126, 45, 20, 32 });
	forward.PushBack({ 147, 45, 23, 32 });
	forward.speed = 0.1f;

	//backwards
	backward.PushBack({ 191, 46, 19, 36 });
	backward.PushBack({ 217, 45, 20, 36 });

	backward.speed = 0.05f;

	
	//Jump animation
	jump.PushBack({ 191, 83, 22, 36 });
	jump.PushBack({ 215, 83, 22, 37 });
	jump.speed = 0.12f;

	

}
j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");
	App->col->Enable();
	player_text = App->tex->Load("textures/Character.png");
	//jumpfx = App->audio->Load_effects("Assets/Audio/Fx/SFX_Landing.wav");
	colPlayer = App->col->AddCollider({ position.x, position.y, 21, 34 }, COLLIDER_PLAYER);
	health = 1;
	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	if (!IsEnabled()) {
		App->col->Disable();
		SDL_DestroyTexture(player_text);
		//Audio
		//App->sounds->Unload_effects(jumpfx);
		
		//Disable
		App->play->Disable();
	}
	
	return true;
}





bool j1Player::Update(float dt) {
		
	float speed = 1;

	if (input) {

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			status = PLAYER_BACKWARD;

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			status = PLAYER_FORWARD;

		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			status = PLAYER_JUMP;

		else {
			status = PLAYER_IDLE;

		}
	}
		switch (status)
		{
		case PLAYER_IDLE:
			current_animation = &idle;
			position.y = 384;

			colPlayer->type = COLLIDER_PLAYER;
			break;

		case PLAYER_BACKWARD:
			
				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
					if (jumpEnable == true) {
						jumpEnable = false;
						jump.Reset();
						/*if (App->sounds->Play_chunk(jumpfx))
						{
							LOG("Could not play select sound. Mix_PlayChannel: %s", Mix_GetError());
						}*/
						jump_timer = 1;
					}
				if (position.x < 10) { position.x -= 0; }
				else position.x -= speed;
				current_animation = &backward;
				colPlayer->type = COLLIDER_PLAYER;
				
		
			break;

		case PLAYER_FORWARD:

				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
					if (jumpEnable == true) {
						jumpEnable = false;
						jump.Reset();
						/*if (App->sounds->Play_chunk(jumpfx))
						{
							LOG("Could not play select sound. Mix_PlayChannel: %s", Mix_GetError());
						}*/
						jump_timer = 1;
					}
				if (position.x > 590) { position.x -= 0; }
				else position.x += speed;
				current_animation = &forward;
				colPlayer->type = COLLIDER_PLAYER;
				
			
			break;

		case PLAYER_JUMP:
			if (jumpEnable == true) {
				jumpEnable = false;
				jump.Reset();
				/*if (App->sounds->Play_chunk(jumpfx))
				{
					LOG("Could not play select sound. Mix_PlayChannel: %s", Mix_GetError());
				}*/
				jump_timer = 1;
			}
			break;

		case PLAYER_IN_JUMP_FINISH:
			status = PLAYER_IDLE;
			jump.Reset();
			break;

		if (jump_timer > 0)
			{
				jump_timer = jump_timer + 1;
				current_animation = &jump;
				if (jump_timer < 8) { colPlayer->SetPos(position.x + 12, position.y - 140); }
				else if (jump_timer < 29) { colPlayer->SetPos(position.x + 12, position.y - 180); }
				else if (jump_timer < 38) { colPlayer->SetPos(position.x + 12, position.y - 165); }

				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { position.x--; }
				if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { position.x++; }

				if (jump_timer > 38)
				{
					jumpEnable = true;
					status = PLAYER_IN_JUMP_FINISH;
					jump_timer = 0;
				}
			}

	}

		if (jump_timer == 0) {
			//Normal collider position
			colPlayer->SetPos(position.x + 12, position.y - 107);
			
		}

		// Draw everything --------------------------------------
		r = current_animation->GetCurrentFrame();

		App->render->Blit(player_text, position.x, position.y - r.h, &r, 1, SDL_FLIP_HORIZONTAL);


		r.x = position.x;
		r.y = position.y;
		

		return UPDATE_CONTINUE;

}