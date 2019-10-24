#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "SDL_image/include/SDL_image.h"


 Player :: Player()
{
	position.x = 16;
	position.y = 160;

	// idle animation
	idle.PushBack({ 28, 208, 62, 105 });
	idle.PushBack({ 97, 208, 60, 106 });
	idle.PushBack({ 163, 208, 60, 105 });
	idle.speed = 0.1f;

	//forward
	forward.PushBack({ 15, 337, 60, 106 });
	forward.PushBack({ 90, 337, 70, 107 });
	forward.PushBack({ 170, 337, 62, 108 });
	forward.PushBack({ 247, 337, 60, 107 });
	forward.speed = 0.125f;

	//backwards
	backward.PushBack({ 16, 460, 58, 107 });
	backward.PushBack({ 96, 457, 62, 107 });
	backward.PushBack({ 179, 457, 62, 107 });
	backward.speed = 0.125f;

	
	//Jump animation
	jump.PushBack({ 15, 337, 60, 106 });
	jump.PushBack({ 468, 163, 52, 170 });
	jump.PushBack({ 528, 156, 65, 185 });
	jump.PushBack({ 593, 185, 59, 158 });
	jump.speed = 0.12f;



}
Player::~Player()
{}

// Load assets
bool Player::Start()
{
	LOG("Loading player");
	
	player_text = App->tex->Load("Assets/Sprites/Terry Bogard/Terry Sprites.png");
	//jumpfx = App->audio->Load_effects("Assets/Audio/Fx/SFX_Landing.wav");
	//colPlayer = App->collision->AddCollider({ position.x, position.y, 34, 106 }, COLLIDER_PLAYER);
	health = 1;
	return true;
}

// Unload assets
bool Player::CleanUp()
{
	/*LOG("Unloading player");
	if (!IsEnabled()) {
		App->collision->Disable();
		SDL_DestroyTexture(graphicsTerry);
		//Audio
		App->sounds->Unload_effects(punchfx);
		App->sounds->Unload_effects(kickfx);
		App->sounds->Unload_effects(jumpfx);
		App->sounds->Unload_effects(specialfx);
		App->sounds->Unload_effects(winfx);
		App->sounds->Unload_effects(defeatfx);
		//Disable
		App->player->Disable();
	}
	*/
	return true;
}