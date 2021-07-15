#include "Scene.h"
#include "SceneBasic.h"
#include "GL/glew.h"

void Scene::InitShaderUniforms()
{
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_M] = glGetUniformLocation(m_programID, "M");
	m_parameters[U_VP] = glGetUniformLocation(m_programID, "VP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");

	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
	m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");

	m_parameters[U_ISMVP] = glGetUniformLocation(m_programID, "isMVP");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");

	m_parameters[U_ISMOVE] = glGetUniformLocation(m_programID, "ismove");
	m_parameters[U_MOVEVAL] = glGetUniformLocation(m_programID, "Val");
}

void SceneBasic::InitShadowShaderUniforms()
{
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");

	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");

	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");
}

void SceneBasic::InitLightShaderUniforms()
{
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	std::string name = "lights[";
	for (int i = 0; i < NUM_OF_LIGHTS; ++i)
	{
		m_parameters[12 + i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].type").c_str());
		name = "lights[";
		m_parameters[13+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].position_cameraspace").c_str());
		name = "lights[";
		m_parameters[14+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].color").c_str());
		name = "lights[";
		m_parameters[15+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].power").c_str());
		name = "lights[";
		m_parameters[16+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].kC").c_str());
		name = "lights[";
		m_parameters[17+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].kL").c_str());
		name = "lights[";
		m_parameters[18+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].kQ").c_str());
		name = "lights[";
		m_parameters[19+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].spotDirection").c_str());
		name = "lights[";
		m_parameters[20+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].cosCutoff").c_str());
		name = "lights[";
		m_parameters[21+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].cosInner").c_str());
		name = "lights[";
		m_parameters[22+ i * 11] = glGetUniformLocation(m_programID, name.append(std::to_string(i)).append("].exponent").c_str());
		name = "lights[";
	}
}