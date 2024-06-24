#pragma once
#include "Engine/GameObject.h"
class Camera :
    public GameObject
{
public:
    Camera(GameObject* parent);
    ~Camera() {}

    void Update();

    void SetValueX(int v) { valueX = v; };
    int GetValueX() { return valueX; }
    void SetValueY(int v) { valueY = v; };
    int GetValueY() { return valueY; }

    /// <summary>
    /// �J�����c�U�����J�n����
    /// </summary>
    /// <param name="v">�Ռ��̑傫��</param>
    void VibrationY(float v);
private:
    int valueX;
    int valueY;
    //VECTOR lookTarget;
    float vibTimer; //���Ԃ𑪂�ϐ�
    float vibParam;//�����Ŏw�肳�ꂽ�l
    float vibValue;//�U���ʁA�U����
};

