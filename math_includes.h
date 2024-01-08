#pragma once

#define EPSILON 0.000001f
#define RAD2DEG 57.2958f
#define DEG2RAD 0.0174533f

/*
It takes two numbers, the row of "a" and the column of "b", to dot together and the result is the dot product of the two.
*/
#define M4D(aRow, bCol) \
a.v[0 * 4 + aRow] * b.v[bCol * 4 + 0] + \
a.v[1 * 4 + aRow] * b.v[bCol * 4 + 1] + \
a.v[2 * 4 + aRow] * b.v[bCol * 4 + 2] + \
a.v[3 * 4 + aRow] * b.v[bCol * 4 + 3]

/*
It takes the row of a matrix and performs a dot product of that row against the provided column vector.
*/
#define M4V4D(mRow, x, y, z, w) \
x * m.v[0 * 4 + mRow] + \
y * m.v[1 * 4 + mRow] + \
z * m.v[2 * 4 + mRow] + \
w * m.v[3 * 4 + mRow]

#define M4SWAP(x, y) \
{float t = x; x = y; y = t; }

/*
It finds the minor of one element in the matrix, given an array of floats, and three rows and three columns to cut from the matrix.
*/
#define M4_3X3MINOR(x, c0, c1, c2, r0, r1, r2) \
(x[c0*4+r0]*(x[c1*4+r1]*x[c2*4+r2]-x[c1*4+r2]* \
x[c2*4+r1])-x[c1*4+r0]*(x[c0*4+r1]*x[c2*4+r2]- \
x[c0*4+r2]*x[c2*4+r1])+x[c2*4+r0]*(x[c0*4+r1]* \
x[c1*4+r2]-x[c0*4+r2]*x[c1*4+r1]))