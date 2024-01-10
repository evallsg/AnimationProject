#include "Application.h"
#include "quat.h"
#include "Transform.h"

Application::Application() {
	//allocate memory
	fov = 60.f;
	cameraPos = vec3(0, 4, 7);
	cameraTarget = vec3(0, 4, 0);
}

void Application::Orbit(float angle, vec3 axis) {
	vec3 front = cameraPos - cameraTarget;
	//normalize(front);
	quat rotation = angleAxis(angle, axis);
	Transform m;
	m.rotation = rotation;

	vec3 newFront = transformVector(m, front);
	cameraPos = newFront + cameraTarget;

}