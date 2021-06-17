#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
using namespace cv;
using namespace std;
const int imageWidth = 2592;                             //����ͷ�ķֱ���  
const int imageHeight = 1944;
Size imageSize = Size(imageWidth, imageHeight);

Mat rgbImageL, grayImageL;
Mat rgbImageR, grayImageR;
Mat rectifyImageL, rectifyImageR;

Rect validROIL;//ͼ��У��֮�󣬻��ͼ����вü��������validROI����ָ�ü�֮�������  
Rect validROIR;

Mat mapLx, mapLy, mapRx, mapRy;     //ӳ���  
//Mat Rl, Rr, Pl, Pr, Q;              //У����ת����R��ͶӰ����P ��ͶӰ����Q
Mat Rl = (Mat_<double>(3, 3) << 9.8744454972499607e-01, 1.5367921134590315e-01, -3.6550803253840441e-02, -1.5316487180681099e-01, 9.8806296581186392e-01, 1.6495382246756785e-02, 3.8649492400326307e-02, -1.0689976200380005e-02, 9.9919564708120723e-01);
Mat Rr = (Mat_<double>(3, 3) << 9.8759107269478863e-01, 1.5376889436435140e-01, -3.1921783463986018e-02, -1.5419113706118934e-01, 9.8797753832421464e-01, -1.1201652494275618e-02, 2.9815539326569086e-02, 1.5984708092108994e-02, 9.9942759753864907e-01);
Mat Pl = (Mat_<double>(3, 4) << 1.9874812287654918e+03, 0.0, 1.1568657989501953e+03, 0.0, 0.0, 1.9874812287654918e+03, 1.0283108215332031e+03, 0.0, 0.0, 0.0, 1.0, 0.0);
Mat Pr = (Mat_<double>(3, 4) << 1.9874812287654918e+03, 0.0, 1.1568657989501953e+03, -2.6856675061789752e+04, 0.0, 1.9874812287654918e+03, 1.0283108215332031e+03, 0.0, 0.0, 0.0, 1.0, 0.0);
Mat Q = (Mat_<double>(4, 4) << 1.0, 0.0, 0.0, -1.1568657989501953e+03, 0.0, 1.0, 0.0, -1.0283108215332031e+03, 0.0, 0.0, 0.0, 1.9874812287654918e+03, 0.0, 0.0, 7.4003249627619547e-02, 0.0);

Mat xyz;              //��ά����

Point origin;         //��갴�µ���ʼ��
Rect selection;      //�������ѡ��
bool selectObject = false;    //�Ƿ�ѡ�����


Ptr<StereoSGBM> sgbm = StereoSGBM::create(0, 16, 3);

/*
���ȱ궨�õ�����Ĳ���
fx 0 cx
0 fy cy
0 0  1
*/
Mat cameraMatrixL = (Mat_<double>(3, 3) << 2.6569861182035270e+03, 0, 1.0353951278310813e+03,
    0, 2.6569861182035270e+03, 9.9966105606385088e+02,
    0, 0, 1.0);
Mat distCoeffL = (Mat_<double>(1, 14) << -2.0385227775148645e-01, 3.7264672265277188e-02, 0.0, 0.0, 0.0, 0.0, 0.0, -1.7820328178889719e-01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

Mat cameraMatrixR = (Mat_<double>(3, 3) << 2.6569861182035270e+03, 0, 1.0360143419277833e+03,
    0, 2.6569861182035270e+03, 9.9733343911860959e+02,
    0, 0, 1.0);
Mat distCoeffR = (Mat_<double>(1, 14) << -4.0372736431497286e-01, -3.2929285008962944e-01, 0.0, 0.0, 0.0, 0.0, 0.0, -2.3254422354827882e+00, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

Mat T = (Mat_<double>(3, 1) << -1.3345239265360572e+01, -2.0778667847440291e+00, 4.3135650967511280e-01);//Tƽ������
//Mat rec = (Mat_<double>(3, 1) << 0.04345, -0.05236, -0.01810);//rec��ת����
Mat R = (Mat_<double>(3, 3) << 9.9996044329186884e-01, -8.9706240839532115e-04, -8.8491316277174888e-03, 1.1326045058079777e-03, 9.9964422293849819e-01, 2.6648541286897889e-02, 8.8220779050430902e-03, -2.6657509724682119e-02, 9.9960569632066210e-01);//R ��ת����


static void saveXYZ(const char* filename, const Mat& mat)
{
    const double max_z = 16.0e4;
    FILE* fp = fopen(filename, "wt");
    printf("%d %d \n", mat.rows, mat.cols);
    for (int y = 0; y < mat.rows; y++)
    {
        for (int x = 0; x < mat.cols; x++)
        {
            Vec3f point = mat.at<Vec3f>(y, x);
            if (fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);

        }
    }
    fclose(fp);
}

/*�����ͼ��ɫ*/
void GenerateFalseMap(cv::Mat& src, cv::Mat& disp)
{
    // color map  
    float max_val = 255.0f;
    float map[8][4] = { { 0,0,0,114 },{ 0,0,1,185 },{ 1,0,0,114 },{ 1,0,1,174 },
    { 0,1,0,114 },{ 0,1,1,185 },{ 1,1,0,114 },{ 1,1,1,0 } };
    float sum = 0;
    for (int i = 0; i < 8; i++)
        sum += map[i][3];

    float weights[8]; // relative   weights  
    float cumsum[8];  // cumulative weights  
    cumsum[0] = 0;
    for (int i = 0; i < 7; i++) {
        weights[i] = sum / map[i][3];
        cumsum[i + 1] = cumsum[i] + map[i][3] / sum;
    }

    int height_ = src.rows;
    int width_ = src.cols;
    // for all pixels do  
    for (int v = 0; v < height_; v++) {
        for (int u = 0; u < width_; u++) {

            // get normalized value  
            float val = std::min(std::max(src.data[v * width_ + u] / max_val, 0.0f), 1.0f);

            // find bin  
            int i;
            for (i = 0; i < 7; i++)
                if (val < cumsum[i + 1])
                    break;

            // compute red/green/blue values  
            float   w = 1.0 - (val - cumsum[i]) * weights[i];
            uchar r = (uchar)((w * map[i][0] + (1.0 - w) * map[i + 1][0]) * 255.0);
            uchar g = (uchar)((w * map[i][1] + (1.0 - w) * map[i + 1][1]) * 255.0);
            uchar b = (uchar)((w * map[i][2] + (1.0 - w) * map[i + 1][2]) * 255.0);
            //rgb�ڴ��������  
            disp.data[v * width_ * 3 + 3 * u + 0] = b;
            disp.data[v * width_ * 3 + 3 * u + 1] = g;
            disp.data[v * width_ * 3 + 3 * u + 2] = r;
        }
    }
}

/*****����ƥ��*****/
void stereo_match(int, void*)
{
    sgbm->setPreFilterCap(63);
    int sgbmWinSize = 5;//����ʵ������Լ��趨
    int NumDisparities = 1 * 16;//����ʵ������Լ��趨
    int UniquenessRatio = 15;//����ʵ������Լ��趨
    sgbm->setBlockSize(sgbmWinSize);
    int cn = rectifyImageL.channels();

    sgbm->setP1(8 * cn * sgbmWinSize * sgbmWinSize);
    sgbm->setP2(32 * cn * sgbmWinSize * sgbmWinSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(NumDisparities);
    sgbm->setUniquenessRatio(UniquenessRatio);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(10);
    sgbm->setDisp12MaxDiff(1);
    sgbm->setMode(StereoSGBM::MODE_SGBM);
    Mat disp, dispf, disp8;
    sgbm->compute(rectifyImageL, rectifyImageR, disp);
    cout << "disp: " << disp.rows << "*" << disp.cols << endl;

    //ȥ�ڱ�
    Mat img1p, img2p;
    copyMakeBorder(rectifyImageL, img1p, 0, 0, NumDisparities, 0, IPL_BORDER_REPLICATE);
    copyMakeBorder(rectifyImageR, img2p, 0, 0, NumDisparities, 0, IPL_BORDER_REPLICATE);
    dispf = disp.colRange(NumDisparities, img2p.cols - NumDisparities);

    cout << "dispf: " << dispf.rows << "*" << dispf.cols << endl;

    dispf.convertTo(disp8, CV_8U, 255 / (NumDisparities * 16.));
    reprojectImageTo3D(dispf, xyz, Q, true); //��ʵ�������ʱ��ReprojectTo3D������X / W, Y / W, Z / W��Ҫ����16(Ҳ����W����16)�����ܵõ���ȷ����ά������Ϣ��
    xyz = xyz * 16;

    cout << "disp8: " << disp8.rows << "*" << disp8.cols << endl;

   // imshow("disparity", disp8);
    imwrite("./images/A4/dragon/mod/dispmap.bmp", disp8);
    Mat color(dispf.size(), CV_8UC3);
    GenerateFalseMap(disp8, color);//ת�ɲ�ͼ

    cout << "color: " << color.rows << "*" << color.cols << endl;

  //  imshow("disparity_color", color);
    imwrite("./images/A4/dragon/mod/dispmap_color.bmp", color);


    //MODIFY: using disp not dispf 
    disp.convertTo(disp8, CV_8U, 255 / (NumDisparities * 16.));
    reprojectImageTo3D(disp, xyz, Q, true); //��ʵ�������ʱ��ReprojectTo3D������X / W, Y / W, Z / W��Ҫ����16(Ҳ����W����16)�����ܵõ���ȷ����ά������Ϣ��
    xyz = xyz * 16;

    cout << "disp8: " << disp8.rows << "*" << disp8.cols << endl;

   // imshow("disparity", disp8);
    imwrite("./images/A4/dragon/mod/dispmap_mod.bmp", disp8);
    Mat color_mod(disp.size(), CV_8UC3);
    GenerateFalseMap(disp8, color_mod);//ת�ɲ�ͼ

    cout << "color_mod: " << color_mod.rows << "*" << color_mod.cols << endl;

   // imshow("disparity_color_mod", color_mod);
    imwrite("./images/A4/dragon/mod/dispmap_color_mod.bmp", color_mod);
    //MODIFY FINISHED


    //saveXYZ("xyz.xls", xyz);
}



/*****�������������ص�*****/
static void onMouse(int event, int x, int y, int, void*)
{
    if (selectObject)
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);
    }

    switch (event)
    {
    case EVENT_LBUTTONDOWN:   //�����ť���µ��¼�
        origin = Point(x, y);
        selection = Rect(x, y, 0, 0);
        selectObject = true;
        cout << origin << "in world coordinate is: " << xyz.at<Vec3f>(origin) << endl;
        break;
    case EVENT_LBUTTONUP:    //�����ť�ͷŵ��¼�
        selectObject = false;
        if (selection.width > 0 && selection.height > 0)
            break;
    }
}


/*****������*****/
int main()
{
    /*  ����У��    */
    //Rodrigues(rec, R); //Rodrigues�任
    //stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY,
        //0, imageSize, &validROIL, &validROIR);
    initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pl, imageSize, CV_16SC2, mapLx, mapLy);
    initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_16SC2, mapRx, mapRy);

    /*  ��ȡͼƬ    */
    rgbImageL = imread("./images/A4/dragon/mod/left1_mod.bmp");//CV_LOAD_IMAGE_COLOR
    rgbImageR = imread("./images/A4/dragon/mod/right1_mod.bmp");//CV_LOAD_IMAGE_COLOR


    /*  ����remap֮�����������ͼ���Ѿ����沢���ж�׼�� */
    remap(rgbImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);//INTER_LINEAR
    remap(rgbImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);

    //imwrite("./images/A4/dragon/mod/rectifyImageL.bmp", rectifyImageL);
    cout << "rectifyImageL.bmp: " << rectifyImageL.rows << "*" << rectifyImageL.cols << endl;

    /*  ��У�������ʾ����*/

    //��ʾ��ͬһ��ͼ��
    Mat canvas;
    double sf;
    int w, h;
    sf = 700. / MAX(imageSize.width, imageSize.height);
    w = cvRound(imageSize.width * sf);
    h = cvRound(imageSize.height * sf);
    canvas.create(h, w * 2, CV_8UC3);   //ע��ͨ��

                                        //��ͼ�񻭵�������
    Mat canvasPart = canvas(Rect(w * 0, 0, w, h));                                //�õ�������һ����  
    resize(rectifyImageL, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);     //��ͼ�����ŵ���canvasPartһ����С  
    Rect vroiL(cvRound(validROIL.x * sf), cvRound(validROIL.y * sf),                //��ñ���ȡ������    
        cvRound(validROIL.width * sf), cvRound(validROIL.height * sf));
    //rectangle(canvasPart, vroiL, Scalar(0, 0, 255), 3, 8);                      //����һ������  
    cout << "Painted ImageL" << endl;

    //��ͼ�񻭵�������
    canvasPart = canvas(Rect(w, 0, w, h));                                      //��û�������һ����  
    resize(rectifyImageR, canvasPart, canvasPart.size(), 0, 0, INTER_LINEAR);
    Rect vroiR(cvRound(validROIR.x * sf), cvRound(validROIR.y * sf),
        cvRound(validROIR.width * sf), cvRound(validROIR.height * sf));
    //rectangle(canvasPart, vroiR, Scalar(0, 0, 255), 3, 8);
    cout << "Painted ImageR" << endl;

    //���϶�Ӧ������
    for (int i = 0; i < canvas.rows; i += 16)
        line(canvas, Point(0, i), Point(canvas.cols, i), Scalar(0, 255, 0), 1, 8);
    //imshow("rectified", canvas);

    /*  ����ƥ��    */
    namedWindow("disparity", CV_WINDOW_NORMAL);
    //�����Ӧ����setMouseCallback(��������, ���ص�����, �����ص������Ĳ�����һ��ȡ0)
    //setMouseCallback("disparity", onMouse, 0);//disparity
    stereo_match(0, 0);

    waitKey(0);
    return 0;
}