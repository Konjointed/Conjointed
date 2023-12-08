#include <stb_image.h>

#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Texture.h"

void AddObject(Scene* scene) {
	auto blankObject = std::make_shared<GameObject>();
	blankObject->type = STATICMESH;
	blankObject->name = "New Object";
	scene->sceneObjects.push_back(blankObject);
}

void InitScene(Scene* scene) {
	scene->debugSphere = std::make_shared<Model>(Model::GenerateSphere());

	auto testObject = std::make_shared<GameObject>();
	testObject->type = STATICMESH;
	testObject->name = "Workspace";
	scene->sceneObjects.push_back(testObject);

	/*
	auto sphereModel = std::make_shared<Model>(Model::GenerateSphere());
	auto sphereObject = std::make_unique<GameObject>(sphereModel);
	sphereObject->type = STATICMESH;
	sphereObject->name = "Sphere";
	scene->sceneObjects.push_back(std::move(sphereObject));
	*/

	auto cubeModel = std::make_shared<Model>(Model::GenerateCube());
	auto cubeObject = std::make_unique<GameObject>(cubeModel);
	cubeObject->type = STATICMESH;
	cubeObject->name = "Cube1";
	scene->sceneObjects.push_back(std::move(cubeObject));
	//testObject->AddChild(std::move(cubeObject));

	auto cubeObject2 = std::make_unique<GameObject>(cubeModel);
	cubeObject2->type = STATICMESH;
	cubeObject2->name = "Cube2";
	scene->sceneObjects.push_back(std::move(cubeObject2));

	auto cubeObject3 = std::make_unique<GameObject>(cubeModel);
	cubeObject3->type = STATICMESH;
	cubeObject3->name = "Cube3";
	scene->sceneObjects.push_back(std::move(cubeObject3));

	auto planeModel = std::make_shared<Model>(Model::GeneratePlane());
	auto planeObject = std::make_unique<GameObject>(planeModel);
	planeObject->type = STATICMESH;
	planeObject->name = "Plane";
	scene->sceneObjects.push_back(std::move(planeObject));
	//testObject->AddChild(std::move(planeObject));

	/*
	auto playerModel = std::make_shared<Model>("Resources/Models/Maria/Maria J J Ong.dae", false);
	auto playerObject = std::make_shared<GameObject>(playerModel);
	playerObject->type = STATICMESH;
	playerObject->name = "Player";
	scene->sceneObjects.push_back(playerObject);

	scene->idleAnimation = new Animation("Resources/Animations/Idle.dae", playerModel.get());
	scene->animator = new Animator(scene->idleAnimation);
	*/

	auto cameraObject = std::make_shared<Camera>();
	cameraObject->type = STATICMESH;
	cameraObject->name = "Camera";
	scene->sceneObjects.push_back(cameraObject);
	scene->camera = cameraObject;

	stbi_set_flip_vertically_on_load(false);

	auto skyboxObject = std::make_shared<Skybox>();
	skyboxObject->type = SKYBOX;
	skyboxObject->name = "Skybox";
	scene->sceneObjects.push_back(skyboxObject);
	//scene->skybox = skyboxObject;
}

void UpdateScene(Scene* scene, float deltaTime) {
	// Remove any objects waiting to be destroyed from the scene
	for (auto it = scene->sceneObjects.begin(); it != scene->sceneObjects.end();) {
		if ((*it)->IsPendingDesturction()) {
			it = scene->sceneObjects.erase(it);
		}
		else {
			it++;
		}
	}

	if (Viewport::FOCUSED) {
		scene->camera->Update(deltaTime);
	}
}