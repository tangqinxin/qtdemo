#include"MatIO.h"

//�ú���������ʹ������
void WriteMatToFile(fstream& file, Mat& mat) {
	switch (mat.type())
	{
	case(CV_32FC1):
		for (int i = 0; i< mat.rows; i++) {
			for (int j = 0; j < mat.cols; j++) {
				file << mat.at<float>(i, j) << " ";
			}
			file << endl;
		}
		break;
	case(CV_64FC1):
		for (int i = 0; i< mat.rows; i++) {
			for (int j = 0; j < mat.cols; j++) {
				file << mat.at<double>(i, j) << " ";
			}
			file << endl;
		}
	default:
		break;
	}

}

//�˺���ʹ��index��Ϊλ���жϣ���ʹ������
void ReadMatFromFile(fstream& file, Mat& mat, int Index) {
	bool flag_ok = false;//�ж��Ƿ񵽴�ָ������
	switch (mat.type())
	{
	case(CV_32FC1):
		float ftemp;
		for (int k = 0; k < Index; k++) {
			if (k == Index - 1) {
				flag_ok = true;
			}
			for (int i = 0; i < mat.rows; i++) {
				for (int j = 0; j < mat.cols; j++) {
					if (flag_ok) {
						file >> mat.at<float>(i, j);
					}
					else {
						file >> ftemp;//���û�е��ֱ�Ӷ���temp����
					}
				}
			}
		}
		break;
	case(CV_64FC1):
		double dtemp;
		for (int k = 0; k < Index; k++) {
			if (k == Index - 1) {
				flag_ok = true;
			}
			for (int i = 0; i < mat.rows; i++) {
				for (int j = 0; j < mat.cols; j++) {
					if (flag_ok) {
						file >> mat.at<double>(i, j);
					}
					else {
						file >> dtemp;//���û�е��ֱ�Ӷ���temp����
					}
				}
			}
		}
		break;
	default:
		break;
	}
	
}


// �����ļ�������ȡindex����mat
void ReadMatFromFileName(string FileName, Mat& mat, int Index) {
	fstream file;
	file.open(FileName);
	ReadMatFromFile(file, mat, Index);
	file.close();
}