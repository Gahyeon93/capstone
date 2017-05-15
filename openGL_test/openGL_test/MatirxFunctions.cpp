#include"Matrix.h"
#include"Physics.h"
#include<iostream>
#include<stdio.h>
#define PI 3.141592
using namespace std;
class Vector3;

Matrix::Matrix(Vector3 vector)
{
	m_nRows = 3;
	m_nCols = 1;
	m_pfMat = new float[3];
	m_pfMat[0] = vector.x;
	m_pfMat[1] = vector.y;
	m_pfMat[2] = vector.z;
}

Vector3 Matrix::GetVector()
{
	if (!((m_nCols == 3 && m_nRows == 1) || (m_nCols == 1 && m_nRows == 3)))
	{
		printf("Not available\n");
		exit(1);
	}

	Vector3 vector(m_pfMat[0], m_pfMat[1], m_pfMat[2]);
	return vector.normalize();
}

Matrix::Matrix()
{
	m_nCols = 0;
	m_nRows = 0;
	m_pfMat = 0;
}


Matrix::Matrix(Matrix &mxInput)
{
	m_nCols = mxInput.m_nCols;
	m_nRows = mxInput.m_nRows;

	int index = m_nCols *m_nRows;

	m_pfMat = new float[m_nCols * m_nRows];

	for (int i = 0; i < index; i++) m_pfMat[i] = mxInput.m_pfMat[i];
}

Matrix::~Matrix()
{
	if (m_pfMat)
	{
		delete(m_pfMat);
		m_pfMat = 0;
	}
}

void    Matrix::Show()
{
	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			printf("%7.2f", m_pfMat[i*m_nCols + j]);
		}
		cout << "\n";
	}

	cout << endl;
}

void    Matrix::SetValue(int* pnValue, int nRows, int nCols)
{

	m_nCols = nCols;
	m_nRows = nRows;

	m_pfMat = new float[nRows * nCols];

	int i = 0;
	int j;

	for (; i < m_nRows; i++)
	{
		for (j = 0; j < m_nCols; j++)
		{
			m_pfMat[i*m_nCols + j] = (float)pnValue[i*m_nCols + j];
		}
	}
}

void    Matrix::SetValue(float* pfValue, int nRows, int nCols)
{
	m_nCols = nCols;
	m_nRows = nRows;
	m_pfMat = new float[nRows * nCols];

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			m_pfMat[i*nCols + j] = pfValue[i*nCols + j];
		}
	}
}

float   Matrix::GetValue(int nRows, int nCols)
{
	return m_pfMat[m_nCols*(nRows - 1) + nCols - 1];
}

Matrix&   Matrix::operator =(Matrix &mxInput)
{
	m_nCols = mxInput.m_nCols;
	m_nRows = mxInput.m_nRows;
	if (m_pfMat)
	{
		delete(m_pfMat);
	}

	int index = m_nCols * m_nRows;
	m_pfMat = new float[index];

	for (int i = 0; i < index; i++)
	{
		m_pfMat[i] = mxInput.m_pfMat[i];
	}

	return *this;
}

Matrix    Matrix::operator +(const Matrix &mxInput) const
{
	if (m_nCols != mxInput.m_nCols || m_nRows != mxInput.m_nRows)
		cout << "Matrix Error" << endl;

	Matrix Sum;

	Sum.m_nCols = m_nCols;
	Sum.m_nRows = m_nRows;
	Sum.m_pfMat = new float[m_nCols * m_nRows];

	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			int index = i*m_nCols + j;
			Sum.m_pfMat[index] = m_pfMat[index] + mxInput.m_pfMat[index];
		}
	}

	return Sum;
}

Matrix    Matrix::operator -(const Matrix &mxInput) const
{
	if (m_nCols != mxInput.m_nCols || m_nRows != mxInput.m_nRows)
		cout << "Matrix Error" << endl;

	Matrix sub;

	sub.m_nCols = m_nCols;
	sub.m_nRows = m_nRows;
	sub.m_pfMat = new float[m_nCols * m_nRows];

	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			int index = i*m_nCols + j;
			sub.m_pfMat[index] = m_pfMat[index] - mxInput.m_pfMat[index];
		}
	}

	return sub;
}

Matrix    Matrix::operator * (const int nInput) const
{
	Matrix Dot;

	Dot.m_nCols = m_nCols;
	Dot.m_nRows = m_nRows;
	Dot.m_pfMat = new float[m_nCols * m_nRows];

	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			int index = i*m_nCols + j;
			Dot.m_pfMat[index] = m_pfMat[index] * nInput;
		}
	}

	return Dot;
}

Matrix    Matrix::operator *(const float fInput) const
{
	Matrix Dot;

	Dot.m_nCols = m_nCols;
	Dot.m_nRows = m_nRows;
	Dot.m_pfMat = new float[m_nCols * m_nRows];

	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			int index = i*m_nCols + j;
			Dot.m_pfMat[index] = m_pfMat[index] * fInput;
		}
	}
	return Dot;
}

Matrix    Matrix::operator *(const double dInput) const
{
	Matrix Dot;

	Dot.m_nCols = m_nCols;
	Dot.m_nRows = m_nRows;
	Dot.m_pfMat = new float[m_nCols * m_nRows];

	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			int index = i*m_nCols + j;
			Dot.m_pfMat[index] = m_pfMat[index] * dInput;
		}
	}
	return Dot;

}

Matrix  Matrix::operator *(const Matrix &mxInput)const
{
	Matrix MatrixMul;

	MatrixMul.m_nCols = mxInput.m_nCols;
	MatrixMul.m_nRows = m_nRows;
	MatrixMul.m_pfMat = new float[MatrixMul.m_nCols * MatrixMul.m_nRows];

	for (int i = 0; i < MatrixMul.m_nRows; i++)
	{
		for (int j = 0; j < MatrixMul.m_nCols; j++)
		{
			int index = i*MatrixMul.m_nCols + j;
			MatrixMul.m_pfMat[index] = 0;

			for (int k = 0; k < m_nCols; k++)
			{
				MatrixMul.m_pfMat[index] += m_pfMat[i*m_nCols + k] * mxInput.m_pfMat[k*mxInput.m_nCols + j];
			}
		}
	}
	return MatrixMul;
}

Matrix operator *(int nInput, Matrix &mxInput)
{
	return mxInput*nInput;
}
Matrix operator *(float fInput, Matrix &mxInput)
{
	return mxInput*fInput;
}
Matrix operator *(double dInput, Matrix &mxInput)
{
	return mxInput*dInput;
}

Matrix    Matrix::Trans()
{
	Matrix trans;

	trans.m_pfMat = new float[m_nCols * m_nRows];

	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			trans.m_pfMat[j*m_nRows + i] = m_pfMat[i*m_nCols + j];
		}
	}

	trans.m_nCols = m_nRows;
	trans.m_nRows = m_nCols;

	return trans;
}

Matrix Matrix::Duel()
{
	if (m_nCols != 1 && m_nRows != 3)
	{
		cout << "Not Vector!" << endl;
		exit(0);
	}

	Matrix duel;

	float values[9] = { 0,-1.0f* GetValue(3,1), GetValue(2,1), GetValue(3,1), 0, -1.0f * GetValue(1,1), -1.0f * GetValue(2,1),GetValue(1,1),0 };
	duel.SetValue(values, 3, 3);

	return duel;
}


Matrix Matrix::Rotate(float theta)
{
	theta *= PI / 180.0f;
	Matrix rotate;

	float ivalue[9] = { 1,0,0,0,1,0,0,0,1 };
	Matrix imx;
	imx.SetValue(ivalue, 3, 3);

	Matrix vvt = (*this)*Trans();
	Matrix cosPart = ((imx - vvt)*cos(theta));

	Matrix sinPart = sin(theta)*(*this).Duel();
	Matrix rotation;


	rotation = (vvt + cosPart + sinPart);
	return rotation;
}


Vector3   Matrix::operator *(const Vector3 &vector) const
{
	Matrix vec(vector);
	Matrix result = (*this) * vec;
	return result.GetVector().normalize();
}