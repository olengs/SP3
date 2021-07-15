#include "SceneBasic.h"
#include "GL/glew.h"
#include "Utility.h"

void SceneBasic::PassLightInfoToShader()
{
	for (int i = 0; i < NUM_OF_LIGHTS; ++i)
	{
		// passing the light direction if it is a direction light	
		if (lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[13 + i * 11], 1, &lightDirection_cameraspace.x);
		}
		// if it is spot lights, pass in position and direction 
		else if (lights[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[13 + i * 11], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[i].spotDirection;
			glUniform3fv(m_parameters[19 + i * 11], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			// default is point lights (only position since point lights is 360 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[13 + i * 11], 1, &lightPosition_cameraspace.x);
		}
	}
}