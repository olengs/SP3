#include "../SceneBasic.h"
#include "GL/glew.h"

void SceneBasic::InitLightSettings()
{
	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 5;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	for (int i = 1; i < NUM_OF_LIGHTS; ++i)
	{
		lights[i].type = Light::LIGHT_SPOT;
		lights[i].position.Set(0, 0, 0);
		if(i < 4)
			lights[i].color.Set(1, 0, 0);
		else if(i > 13)
			lights[i].color.Set(0, 1, 0);
		else
			lights[i].color.Set(1, 1, 1);
		if(i > 13)
			lights[i].power = 10.f;
		else
			lights[i].power = 10.f;

		lights[i].kC = 1.f;
		lights[i].kL = 0.01f;
		lights[i].kQ = 0.001f;
		lights[i].cosCutoff = cos(Math::DegreeToRadian(30));
		lights[i].cosInner = cos(Math::DegreeToRadian(15));
		lights[i].exponent = 3.f;
		if (i == 14)
			lights[i].spotDirection.Set(0.f, 1.f, 1.f);
		else if(i == 15)
			lights[i].spotDirection.Set(1.f, 1.f, 0.f);
		else
			lights[i].spotDirection.Set(0.f, 1.f, 0.f);
	}

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_OF_LIGHTS);

	for (int i = 0; i < NUM_OF_LIGHTS; ++i)
	{
		glUniform1i(m_parameters[12 + i * 11], lights[i].type);
		glUniform3fv(m_parameters[14 + i * 11], 1, &lights[i].color.r);
		glUniform1f(m_parameters[15+ i * 11], lights[i].power);
		glUniform1f(m_parameters[16+ i * 11], lights[i].kC);
		glUniform1f(m_parameters[17+ i * 11], lights[i].kL);
		glUniform1f(m_parameters[18+ i * 11], lights[i].kQ);
		glUniform1f(m_parameters[20+ i * 11], lights[i].cosCutoff);
		glUniform1f(m_parameters[21+ i * 11], lights[i].cosInner);
		glUniform1f(m_parameters[22+ i * 11], lights[i].exponent);
	}
}