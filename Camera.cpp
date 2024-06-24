#include "Camera.h"

Camera::Camera(GameObject* parent)
{
	valueX = 0;
	valueY = 0;
}

void Camera::Update()
{
	if (vibTimer > 0.0f) {
		vibTimer -= 0.1f;
		if (vibTimer <= 0.0f)
			vibTimer = 0.0f;
	}
	float val = sinf(vibTimer * 2.0f) * vibTimer * 2.0f;//���Ԃ�0.5�ɐݒ肵�Ă��邩��1����X�^�[�g���Đ��邽�߂�2��������
	if (val > vibTimer / 2.0f)
		val = vibTimer;
	else if (val < -vibTimer / 2.0f)
		val = -vibTimer;
	else
		val = 0;
	vibValue = vibParam * val;
	SetValueY(vibValue);
}

void Camera::VibrationY(float v)
{
	vibParam = v;
	//���Ԃ𑪂鎞�͌��炷������{�@���₵�������������R������Ƃ��͑��₷
	vibTimer = 0.5f;//�h�ꎞ��
}
