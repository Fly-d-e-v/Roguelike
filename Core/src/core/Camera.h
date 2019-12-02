#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Camera
{
public:
	void UpdateProjection(const float width, const float height)
	{
		_projectionMatrix = glm::ortho(0.f, width, 0.f, height);
	}

	//Sets the position and updates the view matrix
	void SetPosition(const glm::vec2 position)
	{
		_position = position;
		UpdateViewMat();
	}
	
	//Sets clockwise camera rotation in radians and updates the view matrix
	void SetRotation(const float rotation)
	{
		_rotation = rotation;
		UpdateViewMat();
	}

	[[nodiscard]] /*const*/ glm::mat4/*&*/ GetProjMat() const
	{
		return _projectionMatrix;
	}
	
	[[nodiscard]] /*const*/ glm::mat4/*&*/ GetViewMat() const
	{
		return _viewMatrix;
	}
	
	[[nodiscard]] /*const*/ glm::vec2/*&*/ GetPosition() const {
		return _position;
	}

	//Returns clockwise camera rotation in radians
	[[nodiscard]] /*const*/ float/*&*/ GetRotation() const
	{
		return _rotation;
	}
	
private:
	float _rotation = 0.0f;
	glm::vec2 _position = glm::vec2(0.0f, 0.0f);

	glm::mat4 _projectionMatrix = glm::mat4(1.0f);
	glm::mat4 _viewMatrix = glm::mat4(1.0f);

	void UpdateViewMat()
	{
		_viewMatrix = glm::mat4(1);
		_viewMatrix = glm::rotate(_viewMatrix, _rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		_viewMatrix = glm::translate(_viewMatrix, glm::vec3(_position, 0));
	}
};
