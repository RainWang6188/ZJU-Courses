#pragma once

GLfloat Camera_x, Camera_y, Camera_z;//
GLfloat Gaze_x, Gaze_y, Gaze_z;
GLfloat Camera_radius=grid_size/2;

const double PI = 2 * acos(0);
static GLfloat move_angle = -90.0;   
//�����ʼ�Ƕ�����Ϊ0���ʼ����X������������Ϊ-90���� 
//Z�Ḻ���򣬷���Ĭ������µ����ϰ�ߡ� 

double cameraDistanceX = 0;
double cameraDistanceY = 0;
double cameraAngleX = 0;
double cameraAngleY = 0;
double move_y_angle = -atan(40 / 150);//-90;
double times = 1; //���ű���


//������¼�
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;