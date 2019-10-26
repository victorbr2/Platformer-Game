#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "p2Defs.h"


j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_BOX] = true;
	matrix[COLLIDER_WALL][COLLIDER_LAVA] = false;
	matrix[COLLIDER_WALL][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_WALL][COLLIDER_COLUMN] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_BOX] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_LAVA] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_COLUMN] = true;

	matrix[COLLIDER_BOX][COLLIDER_BOX] = false;
	matrix[COLLIDER_BOX][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_BOX][COLLIDER_WALL] = true;
	matrix[COLLIDER_BOX][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_BOX][COLLIDER_LAVA] = true;
	matrix[COLLIDER_BOX][COLLIDER_COLUMN] = true;

	matrix[COLLIDER_LAVA][COLLIDER_WALL] = false;
	matrix[COLLIDER_LAVA][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_LAVA][COLLIDER_BOX] = true;
	matrix[COLLIDER_LAVA][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_LAVA][COLLIDER_LAVA] = false;
	matrix[COLLIDER_LAVA][COLLIDER_COLUMN] = false;

	matrix[COLLIDER_SPIKES][COLLIDER_WALL] = true;
	matrix[COLLIDER_SPIKES][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SPIKES][COLLIDER_BOX] = true;
	matrix[COLLIDER_SPIKES][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_LAVA] = true;
	matrix[COLLIDER_SPIKES][COLLIDER_COLUMN] = true;

	matrix[COLLIDER_COLUMN][COLLIDER_WALL] = true;
	matrix[COLLIDER_COLUMN][COLLIDER_COLUMN] = false;
	matrix[COLLIDER_COLUMN][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_COLUMN][COLLIDER_BOX] = true;
	matrix[COLLIDER_COLUMN][COLLIDER_LAVA] = false;
	matrix[COLLIDER_COLUMN][COLLIDER_PLAYER] = true;



}


// Destructor
j1Collision::~j1Collision()
{}
bool j1Collision::Update(float dt)
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return UPDATE_CONTINUE;
}

bool j1Collision::PostUpdate()
{

	DebugDraw();

	return UPDATE_CONTINUE;
}
void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);

		case COLLIDER_LAVA: // orange
			App->render->DrawQuad(colliders[i]->rect, 255, 144, 51, alpha);

		case COLLIDER_BOX: // yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 230, 51, alpha);

		case COLLIDER_COLUMN: // purple
			App->render->DrawQuad(colliders[i]->rect, 184, 51, 255, alpha);

		case COLLIDER_SPIKES: // light blue
			App->render->DrawQuad(colliders[i]->rect, 51, 233, 255, alpha);
	
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------


bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool ret = true;

	if (r.x + r.w < rect.x || rect.x + rect.w < r.x || r.y - r.h > rect.y || rect.y - rect.h > r.y) ret = false;

	return ret;
}