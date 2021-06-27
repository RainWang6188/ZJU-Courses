#pragma once

/*draw_solar*/
static double Earth_year = 0, Mercury_year = 0;
static double month = 0;
static double Mercury_day = 0, Earth_day = 0, Sun_day = 0;//88 360

void draw_xz_circle(double r) { //��xyƽ�滭��̬���
	int n = 100, i = 0;
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	while (i++ < n) {
		glVertex3d(r*cos(i * 2 * PI / n), 0, r*sin(i * 2 * PI / n));//x,y,z
	}
	glEnd();
	glPopMatrix();
}
void draw_xy_circle(double r) {
	int n = 1000, i = 0;
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	while (i++ < n) {
		glVertex3d(r*cos(i * 2 * PI / n), r*sin(i * 2 * PI / n), 0);//x,y,z
	}
	glEnd();
	glPopMatrix();
}
void draw_solar() {
	glPushMatrix(); //�����ʱ��״̬
	glColor3f(1, 0, 0);
	glRotated(Sun_day, 0, 1, 0);//Sun���Դ�
	glutWireSphere(25, 20, 20);//Sun

	glPopMatrix();//̫������ת������޹�
	glPushMatrix();
	glRotated(Earth_year, 0, 1, 0);//����y����ת������Ĺ�ת
	glColor3f(0, 0, 1);
	draw_xz_circle(100);//������Ĺ�ת��� 
	glTranslated(100, 0, 0);//�ƶ���x=100

	glPushMatrix();//
	glRotated(Earth_day, 0, 1, 0); //�������ת
	glColor3f(0, 0, 1);
	glutWireSphere(10, 10, 10); //�����ǵ���
	glPopMatrix();//�������ת�������޹�

	glRotated(month, 0, 0, 1);//����Χ�Ƶ���ת
	glColor3f(0.3, 0.3, 0.3);
	draw_xy_circle(25);//����Ĺ��
	glTranslated(25, 0, 0);//�ƶ�
	glutSolidSphere(1, 20, 20);//�������ƶ�(25,0,0)�����ŵ���ת�����������̫��ת	

	glPopMatrix();
	glRotated(8, 0, 0, 1); //8�ȱ�ʾˮ�ǹ��ƫ�Ƶ�һ���Ƕ�
	glRotated(Mercury_year, 0, 1, 0);//��ת����y����ת
	glColor3f(0, 1, 0);
	draw_xz_circle(65);//��ˮ�ǵĹ�ת���
	glTranslated(65, 0, 0);//
	glRotated(Mercury_day, 0, 1, 0);//��ת
	glColor3f(0, 1, 0);
	glutWireSphere(5, 10, 10); //����ˮ��



	glPopMatrix();
}

