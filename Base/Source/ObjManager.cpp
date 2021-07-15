#include "ObjManager.h"

ObjManager* ObjManager::objManager = nullptr;

ObjManager::ObjManager()
{

}

ObjManager::~ObjManager()
{
	while (EntityList.size() > 0) {
		Obj* obj = EntityList.back();
		delete obj;
		EntityList.pop_back();
	}
}

ObjManager* ObjManager::getInstance()
{
	if (!objManager) {
		objManager = new ObjManager();
	}
	return objManager;
}

void ObjManager::Add(Obj* obj)
{
	EntityList.push_back(obj);
}

vector<Obj*>& ObjManager::getList(void)
{
	// TODO: insert return statement here
	return this->EntityList;
}

void ObjManager::CheckCollision()
{
	int size = EntityList.size();
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (EntityList[i]->type == Obj::O_PLAYER)
			{
				switch (EntityList[j]->type)
				{
				case Obj::O_OBSTACLES:
				case Obj::O_ENEMY:
					return;
				}
			}
		}
	}
}

void ObjManager::Destroy()
{
	delete objManager;
}

void ObjManager::SetToDestroy(Obj* obj)
{
	obj->toDestroy = 1;
}

void ObjManager::Update()
{
	int size = EntityList.size();

	for (int i = 0; i < size; ++i) {
		if (EntityList[i]->toDestroy) {
			EntityList.erase(EntityList.begin() + i);
			--size;
		}
	}
}