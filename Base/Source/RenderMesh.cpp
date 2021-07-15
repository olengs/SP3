#include "Scene.h"
#include "SceneMenu/SceneMenu.h"
#include "SceneBasic.h"
#include "SceneWin/SceneWin.h"
#include "SceneLose/SceneLose.h"
#include "GL/glew.h"

void SceneMenu::RenderMesh(Mesh* mesh, bool enableLight, bool isMoving, bool isMVP, bool isWave)
{
	Mtx44 modelView, modelView_inverse_transpose;

	Mtx44 MVP;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniform1f(m_parameters[U_ISMVP], 1.f);
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glUniform1f(m_parameters[U_ISMVP], 1.f);
}

void SceneMenu::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); // Size of screen

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity(); // No need camera for ortho mode

	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);

	RenderMesh(mesh, false);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneBasic::RenderMesh(Mesh* mesh, bool enableLight, bool isMoving, bool isMVP, bool isWave)
{
	Mtx44 modelView, modelView_inverse_transpose;
	Mtx44 oldview = viewStack.Top();
	if (mRenderPass == RENDERPASS_PRE) {
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

		for (int i = 0; i < MAX_TEXTURES; ++i) {
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 0);
			mesh->Render();
			return;
		}
	}
	if (mRenderPass == RENDERPASS_MINIMAP) {
		Mtx44 MVP;
		MVP = m_minimapProj * m_minimapView * modelStack.Top();
		glUniform1f(m_parameters[U_ISMVP], 1.f);
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		modelView = m_minimapView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
			}
			else
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
			}
		}
		mesh->Render();
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		return;
	}

	if (isMVP) {
		Mtx44 MVP;
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniform1f(m_parameters[U_ISMVP], 1.f);
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	}
	else {
		Mtx44 M, VP;
		M = modelStack.Top();
		VP = projectionStack.Top() * viewStack.Top();
		glUniform1f(m_parameters[U_ISMVP], 0.f);
		glUniformMatrix4fv(m_parameters[U_M], 1, GL_FALSE, &M.a[0]);
		glUniformMatrix4fv(m_parameters[U_VP], 1, GL_FALSE, &VP.a[0]);
	}
	if (isMoving) {
		glUniform1f(m_parameters[U_ISMOVE], 1.f);
		glUniform1f(m_parameters[U_MOVEVAL], mesh->moveVal);
	}
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glUniform1f(m_parameters[U_ISMOVE], 0.f);
	glUniform1f(m_parameters[U_ISMVP], 1.f);
}

void SceneBasic::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); // Size of screen

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity(); // No need camera for ortho mode

	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);

	RenderMesh(mesh, false);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Scene::RenderMeshIn2D(Mesh* mesh, bool enableLight, float size, float x, float y, float z)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureArray[0] > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureArray[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneWin::RenderMesh(Mesh* mesh, bool enableLight, bool isMoving, bool isMVP, bool isWave)
{
	Mtx44 modelView, modelView_inverse_transpose;

	Mtx44 MVP;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniform1f(m_parameters[U_ISMVP], 1.f);
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1i(m_parameters[U_LIGHTENABLED], 0);

	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glUniform1f(m_parameters[U_ISMVP], 1.f);
}

void SceneWin::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); // Size of screen

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity(); // No need camera for ortho mode

	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);

	RenderMesh(mesh, false);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneLose::RenderMesh(Mesh* mesh, bool enableLight, bool isMoving, bool isMVP, bool isWave)
{
	Mtx44 modelView, modelView_inverse_transpose;

	Mtx44 MVP;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniform1f(m_parameters[U_ISMVP], 1.f);
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1i(m_parameters[U_LIGHTENABLED], 0);

	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glUniform1f(m_parameters[U_ISMVP], 1.f);
}

void SceneLose::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); // Size of screen

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity(); // No need camera for ortho mode

	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);

	RenderMesh(mesh, false);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}