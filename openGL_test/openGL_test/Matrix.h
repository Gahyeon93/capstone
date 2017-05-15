#pragma once
class Vector3;

class Matrix {
private:
	int      m_nRows;
	int      m_nCols;
	float    *m_pfMat;

public:
	Matrix();
	Matrix(Vector3 vector3);

	~Matrix();

	Matrix(Matrix &mxInput);
	void    Show();
	void    SetValue(int* pnValue, int nRows, int nCols);
	void    SetValue(float* pfValue, int nRows, int nCols);
	float   GetValue(int nRows, int nCols);

	Matrix&   operator =(Matrix &mxInput);
	Matrix    operator +(const Matrix &mxInput) const;
	Matrix    operator -(const Matrix &mxInput) const;
	Matrix    operator *(const int nInput) const;
	Matrix    operator *(const float fInput) const;
	Matrix    operator *(const double dInput) const;
	Matrix    operator *(const Matrix &mxInput) const;
	Vector3   operator *(const Vector3 &vector) const;

	friend    Matrix operator *(const int nInput, Matrix &mxInput);
	friend    Matrix operator *(const float fInput, Matrix &mxInput);
	friend    Matrix operator *(const double dInput, Matrix &mxInput);

	Matrix    Trans();
	Matrix	  Duel();
	Matrix	  Rotate(float theta);

	Vector3 GetVector();
	//Matrix    Inv();
};