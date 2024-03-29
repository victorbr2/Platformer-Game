// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__


#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

struct Collider;

class j1App;

class j1Module
{
private:
	bool enabled = true;

public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	bool IsEnabled() const { return enabled; }

	void Enable()
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}

	
	// Callbacks ---
	virtual void OnCollision(Collider*, Collider*) {}

	enum update_status
	{
		UPDATE_CONTINUE = 1,
		UPDATE_STOP,
		UPDATE_ERROR
	};

public:

	p2SString	name;
	bool		active;
	
};

#endif // __j1MODULE_H__