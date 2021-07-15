#ifndef OBJ_MANAGER
#define OBJ_MANAGER
#include "Obj.h"
#include "Camera3.h"
#include <vector>
#include "Player.h"

using namespace std;

class ObjManager{
	static ObjManager* objManager;
	ObjManager();
	~ObjManager();
	vector<Obj*> EntityList;

public:
	Camera3* camera;
	static ObjManager* getInstance();
	void Add(Obj* obj);
	vector<Obj*>& getList(void);
	void CheckCollision();
	void Destroy();
	void SetToDestroy(Obj* obj);
	void Update();
};
#endif

