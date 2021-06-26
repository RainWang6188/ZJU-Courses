#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;


clock_t start_time;
clock_t end_time;


/*******************************
*�ඨ��
********************************
*
*Rectangle��������
*	
*UnboundArea���ޱ߽�������
*
*BoundArea���б߽�������
*
*******************************/

//����
class Rectangle {
public:
	Rectangle() :height(0), width(0), area(0), x(0), y(0) {}
	Rectangle(int h, int w) {
		int min = h < w ? h : w;
		int max = h > w ? h : w;
		height = min;
		width = max;
		area = height * width;
		x = 0;
		y = 0;
	}
	void SetPosition(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int height;
	int width;
	int area;
	int x;
	int y;
};

class BoundArea;
class UnboundArea {
public:
	UnboundArea() :x(0), y(0), width(0) {}
	~UnboundArea() {}

	void Set(int x, int y, int w);

	void Pack(Rectangle& item, UnboundArea& S1, BoundArea& S2);

	int x;
	int y;
	int width;
};

class BoundArea {
public:
	//functions
	BoundArea() :x(0), y(0), height(0), width(0), area(0) {}
	~BoundArea() {}

	void Set(int x, int y, int h, int w);

	bool CanPack(Rectangle& item);

	void Pack(Rectangle& item, BoundArea& S3, BoundArea& S4);


	//varibles
	int x;
	int y;
	int height;
	int width;
	int area;
};

//����
void UnboundArea::Set(int x, int y, int w) {
	this->x = x;
	this->y = y;
	this->width = w;
}

void UnboundArea::Pack(Rectangle& item, UnboundArea& S1, BoundArea& S2) {
	//Pack item
	item.SetPosition(x, y);

	//Divided into S1 and S2
	S1.Set(x, y + item.height, width);
	S2.Set(x + item.width, y, item.height, width - item.width);
}

void BoundArea::Set(int x, int y, int h, int w) {
	this->x = x;
	this->y = y;
	this->height = h;
	this->width = w;
	this->area = h * w;
}

bool BoundArea::CanPack(Rectangle& item) {
	int h = item.height;
	int w = item.width;
	if (h <= height && w <= width) {
		return true;
	}
	if (w <= height && h <= width) {
		item.height = w;
		item.width = h;
		return true;
	}
	return false;
}

void BoundArea::Pack(Rectangle& item, BoundArea& S3, BoundArea& S4) {
	//Pack item
	item.SetPosition(x, y);

	//Divide
	S3.Set(x, y + item.height, height - item.height, item.width);
	S4.Set(x + item.width, y, height, width - item.width);
}




/******************************
*һЩ���ߺ���
*******************************
*
*myCompare: ����sort���Զ���ȽϺ���
*
*Swap: ���ڽ�����������
*
*******************************/
bool myCompare(Rectangle& a, Rectangle& b) {
	return a.area < b.area;
}

void Swap(Rectangle& a, Rectangle& b) {
	Rectangle temp;
	temp = a;
	a = b;
	b = temp;
}




/******************************
*��Ҫ�Ĺ��ܺ���
*******************************
*
*RecursivePacking:���ڵݹ�Pack�б߽�ռ�
*
*Packing:����Pack�ޱ߽�ռ�
*
*******************************/
void RecursivePacking(vector<Rectangle>& unpacked_recs, BoundArea& S2, vector<Rectangle>& packed_recs) {
	int can = -1;
	for (int i = 0; i < unpacked_recs.size(); i++) {
		if (S2.CanPack(unpacked_recs[i]))
			can = i;
		if (S2.area < unpacked_recs[i].area)
			break;
	}
	if (can == -1)
		return;
	BoundArea S3, S4;
	S2.Pack(unpacked_recs[can], S3, S4);
	packed_recs.push_back(unpacked_recs[can]);
	unpacked_recs.erase(unpacked_recs.begin() + can);
	if (S3.area > S4.area) {
		RecursivePacking(unpacked_recs, S3, packed_recs);
		RecursivePacking(unpacked_recs, S4, packed_recs);
	}
	else {
		RecursivePacking(unpacked_recs, S4, packed_recs);
		RecursivePacking(unpacked_recs, S3, packed_recs);
	}
}

void Packing(vector<Rectangle>& unpacked_recs, UnboundArea& S, vector<Rectangle>& packed_recs) {

	while (unpacked_recs.size() != 0) {
		UnboundArea S1;
		BoundArea S2;
		S.Pack(unpacked_recs[unpacked_recs.size() - 1], S1, S2);
		packed_recs.push_back(unpacked_recs[unpacked_recs.size() - 1]);
		unpacked_recs.pop_back();
		S = S1;
		RecursivePacking(unpacked_recs, S2, packed_recs);
	}
}

/******************************
*������
*******************************
*
*���룺��h w�ĸ�ʽ�����װ����εĸߺͿ���ʼ�����β�����unpacked_recs������
*
*���򣺰����������unpacked_recs�е�Ԫ����С��������
*
*װ�䣺����Packing�����Գ�ʼ�����ޱ߽�����S�ʹ�װ�����unpacked_recsװ��
*
*���1����ѡ��װ�õľ��ε�λ����Ϣ�ʹ�С��Ϣ��x y h w�ĸ�ʽ�����out.txt�ļ���
*���2��Ҳ��ѡ���ڿ���̨���װ���ĸ߶�
*
*/

int main() {
	int N;
	cin >> N;
	int area_width;
	cin >> area_width;

	//����
	vector<Rectangle> unpacked_recs, packed_recs;
	int h, w;
	for (int i = 0; i < N; i++) {
		cin >> h >> w;
		unpacked_recs.push_back(Rectangle(h, w));
	}
	UnboundArea S;
	S.Set(0, 0, area_width);


	start_time = clock();

	//���������
	sort(unpacked_recs.begin(), unpacked_recs.end(), myCompare);


	//װ��
	Packing(unpacked_recs, S, packed_recs);


	end_time = clock();
	cout << "The Time: " << (float)(end_time - start_time) / CLOCKS_PER_SEC << endl;
	

	/********���λ����Ϣ���ļ�********
	*ofstream outfile(".\\out.txt");
	*for (int i = 0; i < N; i++) {
	*	outfile << packed_recs[i].x << " " << packed_recs[i].y << " " << packed_recs[i].height << " " << packed_recs[i].width << endl;
	*}
	*outfile.close();
	********************************/

	//�����С�߶ȵ�����̨
	int height = 0;
	for (int i = 0; i < N; i++) {
		if (packed_recs[i].y + packed_recs[i].height > height)
			height = packed_recs[i].y + packed_recs[i].height;
	}
	cout << "The packed height: " << height << endl;


	system("pause");
}